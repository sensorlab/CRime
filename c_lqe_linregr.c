/*
 * c_lqe_linregr.c
 *
 *  Created on: Feb 3, 2014
 *      Author: carolina
 */


#include "lib/list.h"
#include "lib/memb.h"
#include "net/packetbuf.h"
#include "net/rime/rimeaddr.h"
#include "amodule.h"
#include "c_lqe_linregr.h"

#define DEBUG 1
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif


static void
print_list(list_t neighbor_list){
	struct c_neighbor *n;
	PRINTF("list length %d \n", list_length(neighbor_list));
	for(n = list_head(neighbor_list); n != NULL; n = list_item_next(n)) {
		PRINTF("address %d.%d count %d cost %d \n", n->addr.u8[1],
				n->addr.u8[0], n->count, n->cost);
	}
}
/*---------------------------------------------------------------------------*/

void
c_lqe_linregr_open(struct pipe *p, struct stackmodule_i *module){
	PRINTF("c_linregr_open\n");
	clock_init();
};
/*---------------------------------------------------------------------------*/

void
c_lqe_linregr_close(struct pipe *p, struct stackmodule_i *module) {
	PRINTF("c_linregr_close\n");
}
/*---------------------------------------------------------------------------*/

int
c_lqe_linregr_send(struct pipe *p, struct stackmodule_i *module) {
	PRINTF("c_linregr_send \n"); return 1;
}
/*---------------------------------------------------------------------------*/


/*
 * Solution:
 * http://www.prenhall.com/divisions/bp/app/russellcd/PROTECT/CHAPTERS/CHAP10/REVIEWIT.HTM
 * y = a + bx - the regression
 * a = (sum (xy) - n * x_mean * y_mean) / (sum x*x - n * x_mean * x_mean)
 * b = y_mean - b x_mean
 *
 * We define:
 * y = estimated prr (packet received ratio) = cost
 * x = current rssi
 * x_mean = mean of rssi over a pre-defined time window
 * y_mean = mean prr over a pre-defined time wondow
 *
 *
 * */
void
c_lqe_linregr_recv(struct pipe *p, struct stackmodule_i *module){
	PRINTF("c_linregr_recv \n");

	uint16_t crtseqno = packetbuf_attr(PACKETBUF_ATTR_PACKET_ID);
	uint16_t rssi = packetbuf_attr(PACKETBUF_ATTR_RSSI);
	int16_t l = 0;

	uint8_t list_len = list_length(p->neighbor_list);
	struct c_neighbor *n;
	clock_time_t last_heard = 100000;
	struct c_neighbor *new_n = memb_alloc(&p->neighbor_mem);;
	rimeaddr_copy(&new_n->addr, get_node_addr(0, 1, 0));

	/* Find the neighbor. */
	for(n = list_head(p->neighbor_list); n != NULL; n = list_item_next(n)) {
	    if(rimeaddr_cmp(&new_n->addr, &n->addr)) {
	    	memcpy(new_n, n, sizeof (struct c_neighbor));
	    	PRINTF("node found \n");

	    	new_n->m = crtseqno - n->last_seq_no - 1; //the current seq number - last seq number - crt packet no
	    	if (new_n->m > n->k) {
	    		l = new_n->m - n->k;
	    		new_n->k = n->k;
	    	} else { l = 0; new_n->k = 0; }
	    	uint8_t lost_p; double delta;
	    	for(lost_p = 0; lost_p < l; lost_p++) {
	    		delta = 0 - n->prr;
	    		n->prr = n->prr + delta / (n->count + lost_p);
	        }
	    	delta = 1 - n->prr;
	        new_n->prr = n->prr + delta / (n->count + l);

	        new_n->xy = n->xy + rssi * n->count / (n->count + n->m );
	    	new_n->x_2 = n->x_2 + rssi * rssi;

	    	double b = (new_n->xy - new_n->count * new_n->rssi * new_n->prr) /
	    			(new_n->x_2 - new_n->count * new_n->rssi);
	    	double a = new_n->prr - b * n->rssi;

	    	new_n->cost = a + b * rssi;

	    	PRINTF("a %f b %f new_n_cost %f rssi %d\n", a, b, new_n->cost, rssi);

	    	list_remove(p->neighbor_list, n);
	    	memb_free(&p->neighbor_mem, n);
	    	list_push(p->neighbor_list, new_n);
	    	return;
	    }
	 }

	/*If neighbor is not in the list and list is full, remove from list before adding*/
	if (list_len >= NUM_NEIGHBOR_ENTRIES) {
		for(n = list_head(p->neighbor_list); n != NULL; n = list_item_next(n)) {
			if (last_heard == n->last_time_stamp) {
				list_remove(p->neighbor_list, n);
				memb_free(&p->neighbor_mem, n);
			}
		}
	}
	list_push(p->neighbor_list, new_n);
}
/*---------------------------------------------------------------------------*/

void
c_lqe_linregr_sent(struct pipe *p, struct stackmodule_i *module) {
	PRINTF("c_linregr_sent\n");
}
/*---------------------------------------------------------------------------*/





