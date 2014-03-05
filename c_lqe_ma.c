/*
 * c_lqe_ma.c
 *
 *  Created on: Feb 5, 2014
 *      Author: carolina
 */
#include "lib/list.h"
#include "lib/memb.h"
#include "net/packetbuf.h"
#include "net/rime/rimeaddr.h"
#include "amodule.h"
#include "c_lqe_ma.h"

#define DEBUG 1
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

#ifdef NEIGHBOR_LIFETIME
#define DEFAULT_LIFETIME NEIGHBOR_LIFETIME
#else /* NEIGHBOR_LIFETIME */
#define NEIGHBOR_LIFETIME 60
#endif /* NEIGHBOR_LIFETIME */

static uint16_t seqno;
static struct ctimer t;

static void
print_list(list_t neighbor_list){
	struct c_neighbor *n;
	PRINTF("list length %d \n", list_length(neighbor_list));
	for(n = list_head(neighbor_list); n != NULL; n = list_item_next(n)) {
		PRINTF("address %d.%d count %d cost %f \n", n->addr.u8[1],
				n->addr.u8[0], n->count, n->cost);
	}
}
/*---------------------------------------------------------------------------*/

static void
periodic(struct pipe* p)
{
	uint16_t seqno = packetbuf_attr(PACKETBUF_ATTR_PACKET_ID);
	int16_t l = 0;

	struct c_neighbor *n;
	struct c_neighbor *new_n = memb_alloc(&p->neighbor_mem);;
	double alpha = p->lqe_ewma_param.alpha;

	PRINTF("**********************************Periodic \n");

	for(n = list_head(p->neighbor_list); n != NULL; n = list_item_next(n)) {
		if(rimeaddr_cmp(&new_n->addr, &n->addr)) {
			memcpy(new_n, n, sizeof (struct c_neighbor));
			if (n->rate > 0.9) {
 	 		   l = (uint16_t)((clock_time() - n->last_time_stamp) / (clock_time()));
			} else {
	 	       l = (uint16_t)((clock_time() - n->last_time_stamp) * n->rate);
			}
			new_n->cost = 0;
			new_n->count = 0;
			new_n->k = l;

			list_remove(p->neighbor_list, n);
	    	memb_free(&p->neighbor_mem, n);
	    	list_push(p->neighbor_list, new_n);
	    	print_list(p->neighbor_list);
		}
	}
	ctimer_set(&t, CLOCK_SECOND * NEIGHBOR_LIFETIME, periodic, p);
}
/*---------------------------------------------------------------------------*/

void
c_lqe_ma_open(struct pipe *p, struct stackmodule_i *module){
	PRINTF("c_lqe_ma_open\n");
	seqno = 1;
	clock_init();
	ctimer_set(&t, CLOCK_SECOND * NEIGHBOR_LIFETIME, periodic, p);
};
/*---------------------------------------------------------------------------*/

void
c_lqe_ma_close(struct pipe *p, struct stackmodule_i *module) {
	PRINTF("c_lqe_ma_close\n");
};
/*---------------------------------------------------------------------------*/

int
c_lqe_ma_send(struct pipe *p, struct stackmodule_i *module) {
	PRINTF("c_lqe_ma_send \n");
	packetbuf_set_attr(PACKETBUF_ATTR_PACKET_ID, seqno++);
	return 1;
};
/*---------------------------------------------------------------------------*/

void
c_lqe_ma_recv(struct pipe *p, struct stackmodule_i *module){
	PRINTF("c_lqe_ma_recv seqno %u \n", packetbuf_attr(PACKETBUF_ATTR_PACKET_ID));

	uint16_t crtseqno = packetbuf_attr(PACKETBUF_ATTR_PACKET_ID);
	int16_t l = 0;
	uint8_t list_len = list_length(p->neighbor_list);

	struct c_neighbor *n;
	struct c_neighbor *new_n = memb_alloc(&p->neighbor_mem);;
	rimeaddr_copy(&new_n->addr, get_node_addr(0, 1, 0));

	double highest_cost = 100.0;
	/* Find the neighbor. */
	for(n = list_head(p->neighbor_list); n != NULL; n = list_item_next(n)) {
	    if(rimeaddr_cmp(&new_n->addr, &n->addr)) {
	    	if (highest_cost > n->cost) { highest_cost = n->cost; }

	    	PRINTF("node found \n");
	    	PRINTF("n-count %d n-lastseqno %d n-m %d n-k %d\n", n->count, n->last_seq_no, n->m, n->k);

	    	new_n->m = crtseqno - n->last_seq_no - 1; //the current seq number - last seq number - crt packet no
	    	if (new_n->m > n->k) { l = new_n->m - n->k;
	    	} else { l = 0; new_n->k = 0; }

	    	uint8_t lost_p; double delta;
	    	for(lost_p = 0; lost_p < l; lost_p++) {
	    		delta = 0 - n->cost;
	    		n->cost = n->cost + delta / (n->count + lost_p);
	    	}
	    	delta = 1 - n->cost;
	    	new_n->cost = n->cost + delta / (n->count + l);
	    	PRINTF("l %d cost %f\n", l, new_n->cost);

	    	if (n->count_lock == 1) {
	    		new_n->count = n->count;
	    	} else { new_n->count = n->count + 1; }
	    	//PRINTF("n-count %d newn-count %d \n", n->count, new_n->count);

	    	new_n->count_lock = n->count_lock;
	    	new_n->first_time_stamp = n->first_time_stamp;
	    	new_n->rate = (new_n->count + new_n->m) / (new_n->last_time_stamp - new_n->first_time_stamp);
	    	new_n->last_seq_no = crtseqno;

	    	new_n->first_time_stamp = n->first_time_stamp;
	    	new_n->last_time_stamp = clock_time();
   			new_n->cost_lock = 1;

	    	PRINTF("n-cost %f new-n-count %d delta %f new-n-m %d new-n-cost %f\n",
	    			n->cost, new_n->count, delta, new_n->m, new_n->cost);

	    	list_remove(p->neighbor_list, n);
	    	memb_free(&p->neighbor_mem, n);
	    	list_push(p->neighbor_list, new_n);
	    	print_list(p->neighbor_list);
	    	return;
	    }
	}

	new_n->cost = 0;
	new_n->m = 0;
	new_n->k = 0;
	new_n->last_seq_no = crtseqno;
	new_n->rate = 1 / clock_time();
	new_n->last_time_stamp = clock_time();
	new_n->cost_lock = 1;
	new_n->first_time_stamp = new_n->last_time_stamp;

	/*If neighbor is not in the list and list is full, remove from list before adding*/
	if (list_len >= NUM_NEIGHBOR_ENTRIES) {
		for(n = list_head(p->neighbor_list); n != NULL; n = list_item_next(n)) {
			if (highest_cost == n->cost) {
				list_remove(p->neighbor_list, n);
				memb_free(&p->neighbor_mem, n);
			}
		}
	}
	//push to the list in the place of the node deleted above
	//also push if list is empty
	list_push(p->neighbor_list, new_n);
}
/*---------------------------------------------------------------------------*/

void
c_lqe_ma_sent(struct pipe *p, struct stackmodule_i *module) {
	PRINTF("c_lqe_ma_sent\n");
}






