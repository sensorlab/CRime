/*
 * c_link_stats.c
 *
 *  This module computes statistics for hardware link estimators, see  "Radio Link Quality Estimation
 *  in Wireless Sensor Networks: a Survey" Section 5.1 of http://www.sicstus.se/~luca/papers/baccour11link.pdf
 *  It computes RSSI and LQI averages of all neighbors based for a pre-specified value of received packets
 *  (incoming packet number based).
 *
 *  Created on: Feb 3, 2014
 *      Author: carolina
 */

#include "lib/list.h"
#include "lib/memb.h"
#include "net/packetbuf.h"
#include "net/rime/rimeaddr.h"
#include "amodule.h"
#include "c_link_stats.h"

#define DEBUG 1
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif


void
print_list(list_t neighbor_list){
	struct c_neighbor *n;
	PRINTF("list length %d \n", list_length(neighbor_list));
	for(n = list_head(neighbor_list); n != NULL; n = list_item_next(n)) {
		PRINTF("address %d.%d count %d rssi %d lqi %d cost %f\n", n->addr.u8[1],
				n->addr.u8[0], n->count, n->rssi, n->lqi, n->cost);
	}
}
/*---------------------------------------------------------------------------*/

void
c_link_stats_open(struct pipe *p, struct stackmodule_i *module){
	PRINTF("c_link_stats_open\n");
	clock_init();
};
/*---------------------------------------------------------------------------*/

void
c_link_stats_close(struct pipe *p, struct stackmodule_i *module) {
	PRINTF("c_link_stats_close\n");
};
/*---------------------------------------------------------------------------*/

int
c_link_stats_send(struct pipe *p, struct stackmodule_i *module) {
	PRINTF("c_link_stats_send \n"); return 1;
};
/*---------------------------------------------------------------------------*/

void c_link_stats_recv(struct pipe *p, struct stackmodule_i *module){
	PRINTF("c_link_stats_recv \n");

	uint16_t rssi = packetbuf_attr(PACKETBUF_ATTR_RSSI);
	uint16_t lqi = packetbuf_attr(PACKETBUF_ATTR_LINK_QUALITY);
	PRINTF("-------rssi %u lqi %u \n", packetbuf_attr(PACKETBUF_ATTR_RSSI),
			packetbuf_attr(PACKETBUF_ATTR_LINK_QUALITY));
	uint8_t list_len = list_length(p->neighbor_list);

	struct c_neighbor *n;
	clock_time_t last_heard = 100000;
	struct c_neighbor *new_n = memb_alloc(&p->neighbor_mem);
	rimeaddr_copy(&new_n->addr, get_node_addr(0, 1, 0));

	/* Find the neighbor in the list. */
	for(n = list_head(p->neighbor_list); n != NULL; n = list_item_next(n)) {
		if (last_heard > n->last_time_stamp) { last_heard = n->last_time_stamp; }
	    if(rimeaddr_cmp(&new_n->addr, &n->addr)) {
	    	memcpy(new_n, n, sizeof (struct c_neighbor));
	    	PRINTF("node found \n");
	    	int16_t delta = 0;

	    	// update the packet count and the cost as moving average of received packets
	    	// does not take into account lost packets
	    	if(n->count == p->link_stats_param.maxpr_pts) {
	    		new_n->count = 1;
	    		if (n->cost_lock == 0) { new_n->cost = 1; }
	    	} else {
	    		new_n->count = n->count + 1;
	    		if (n->cost_lock == 0) { new_n->cost = n->cost + 1.0 / new_n->count; }
	    	}
	    	//update the rssi count and the corresponding cost based on moving average
	    	if(n->rssi_count == p->link_stats_param.maxrssi_pts) {
	    		new_n->rssi_count = 1;
	    		new_n->rssi = rssi;
	    	} else {
	    		new_n->rssi_count = n->rssi_count + 1;
	    		delta = rssi - n->rssi;
	    		new_n->rssi = n->rssi + delta/new_n->rssi_count;
	    	}
	    	//update the lqi count and the corresponding cost based on moving average
	    	if(n->lqi_count == p->link_stats_param.maxlqi_pts) {
	    		new_n->lqi_count = 1;
	    		new_n->lqi = lqi;
	    	} else {
	    		new_n->lqi_count = n->lqi_count + 1;
	    		delta = lqi - n->lqi;
	    		new_n->lqi = n->lqi + delta/new_n->lqi_count;
	    	}
	    	// update the node in the neighbor list
	    	list_remove(p->neighbor_list, n);
	    	memb_free(&p->neighbor_mem, n);
	    	list_push(p->neighbor_list, new_n);

	    	print_list(p->neighbor_list);

	    	return;
	    }
	 }

	/*Prepare the neighbor to be added*/
	new_n->count = 1;
	new_n->cost = 0;
	new_n->rssi = rssi;
	new_n->lqi = lqi;
	new_n->rssi_count = 1;
	new_n->lqi_count = 1;
	new_n->first_time_stamp = clock_time();
	new_n->count_lock = 1;

	/*If neighbor is not in the list and list is full, remove from list before adding*/
	// Remove the node that hasn't been heard for a long time
	if (list_len >= NUM_NEIGHBOR_ENTRIES) {
		for(n = list_head(p->neighbor_list); n != NULL; n = list_item_next(n)) {
			if (last_heard == n->last_time_stamp) {
				list_remove(p->neighbor_list, n);
				memb_free(&p->neighbor_mem, n);
			}
		}
	}
	//push to the list in the place of the node deleted above
	//also push if list is empty
	list_push(p->neighbor_list, new_n);

	print_list(p->neighbor_list);
}
/*---------------------------------------------------------------------------*/

void
c_link_stats_sent(struct pipe *p, struct stackmodule_i *module) {
	PRINTF("c_link_stats_sent\n");
}



