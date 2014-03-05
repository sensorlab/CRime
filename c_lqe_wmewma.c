/*
 * c_lqe_wmewma.c
 *
 *  Created on: Feb 5, 2014
 *      Author: carolina
 */

#include "lib/list.h"
#include "lib/memb.h"
#include "net/packetbuf.h"
#include "net/rime/rimeaddr.h"
#include "amodule.h"
#include "c_lqe_wmewma.h"

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

  for(n = list_head(p->neighbor_list); n != NULL; n = list_item_next(n)) {
	  if(rimeaddr_cmp(&new_n->addr, &n->addr)) {
		  memcpy(new_n, n, sizeof (struct c_neighbor));
	 	 PRINTF("node found \n");
	 	 if (n->rate > 0.9) {
 	 		  l = (uint16_t)((clock_time() - n->last_time_stamp) / (clock_time()));
	 	 } else {
	 		  l = (uint16_t)((clock_time() - n->last_time_stamp) * n->rate);
	 	 }
	 	 uint8_t lost_p;
	 	 for(lost_p = 0; lost_p < l; lost_p++) { n->cost *= alpha; }
	 	 double mean = ((double)n->count) / (n->count + n->m);
	 	 new_n->cost = n->cost * alpha + (1 - alpha) * mean;
	 	 //PRINTF("\n\n-------------------l %d n-count %d n-m %d n-cost %f alpha %f mean %f newn-cost %f \n",
	 	 //  l, n->count, n->m, n->cost, alpha, mean, new_n->cost);
	 	 new_n->k = 0;
	 	 new_n->m = 0;
	 	 new_n->count = 0;

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
c_lqe_wmewma_open(struct pipe *p, struct stackmodule_i *module){
	PRINTF("c_lqe_wmewma_open\n");
	seqno = 1;
	clock_init();
	ctimer_set(&t, CLOCK_SECOND * NEIGHBOR_LIFETIME, periodic, p);
}
/*---------------------------------------------------------------------------*/

void
c_lqe_wmewma_close(struct pipe *p, struct stackmodule_i *module) {
	PRINTF("c_lqe_wmewma_close\n");
}
/*---------------------------------------------------------------------------*/

int
c_lqe_wmewma_send(struct pipe *p, struct stackmodule_i *module) {
	PRINTF("c_lqe_wmewma_send \n");
	packetbuf_set_attr(PACKETBUF_ATTR_PACKET_ID, seqno++);
	return 1;
}
/*---------------------------------------------------------------------------*/

void
c_lqe_wmewma_recv(struct pipe *p, struct stackmodule_i *module){
	PRINTF("c_lqe_ewma_recv seqno %u \n", packetbuf_attr(PACKETBUF_ATTR_PACKET_ID));

	uint16_t crtseqno = packetbuf_attr(PACKETBUF_ATTR_PACKET_ID);
	int16_t l = 0;

	struct c_neighbor *n;
	struct c_neighbor *new_n = memb_alloc(&p->neighbor_mem);
	rimeaddr_copy(&new_n->addr, get_node_addr(0, 1, 0));
	double alpha = p->lqe_ewma_param.alpha;
	uint8_t list_len = list_length(p->neighbor_list);

	double highest_cost = 100.0;
	/* Find the neighbor. */
	for(n = list_head(p->neighbor_list); n != NULL; n = list_item_next(n)) {
	    if(rimeaddr_cmp(&new_n->addr, &n->addr)) {
	    	if (highest_cost > n->cost) { highest_cost = n->cost; }

	    	PRINTF("node found \n");
	    	//PRINTF("n-lastseqno %d n-m %d n-k %d\n", n->last_seq_no, n->m, n->k);

	    	new_n->m = n->m + crtseqno - n->last_seq_no - 1; //the current seq number - last seq number - crt packet no
	    	if (n->m > n->k) { l = n->m - n->k; new_n->k = n->k;
	    	} else { l = 0; new_n->k = 0; }

	    	//PRINTF("newn-lastseqno %d newn-m %d newn-k %d\n", new_n->last_seq_no, new_n->m, new_n->k);

	    	if (n->count_lock == 1) {
	    		new_n->count = n->count;
	    	} else { new_n->count = n->count + 1; }
	    	new_n->cost = n->cost;
   		    new_n->first_time_stamp = n->first_time_stamp;
   		    new_n->last_seq_no = crtseqno;
   		 	new_n->last_time_stamp = clock_time();
   		    new_n->rate = (new_n->count + new_n->m) / (new_n->last_time_stamp - new_n->first_time_stamp);
   			new_n->cost_lock = 1;

	    	list_remove(p->neighbor_list, n);
	    	memb_free(&p->neighbor_mem, n);
	    	list_push(p->neighbor_list, new_n);
	    	print_list(p->neighbor_list);
	    	return;
	    }
	 }

	new_n->count = 1;
	new_n->cost = 1;
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
	list_push(p->neighbor_list, new_n);
	print_list(p->neighbor_list);
}
/*---------------------------------------------------------------------------*/

void
c_lqe_wmewma_sent(struct pipe *p, struct stackmodule_i *module) {
	PRINTF("c_lqe_wmewma_sent\n");
}
/*---------------------------------------------------------------------------*/





