/*
 * c_rnd_routing.h
 *
 *  Created on: Apr 6, 2012
 *      Author: carolina
 */

#include "contiki.h"
#include "net/rime.h"
#include "net/rime/crime/stack.h"
#include "lib/list.h"
#include "lib/memb.h"
#include "lib/random.h"

#ifndef C_RND_ROUTING_H_
#define C_RND_ROUTING_H_

#define NEIGHBOR_TIMEOUT 180 * CLOCK_SECOND
#define MAX_NEIGHBORS 16

/*
rimeaddr_t * c_rnd_forward(struct multihop_conn *c,
	const rimeaddr_t *originator, const rimeaddr_t *dest,
	const rimeaddr_t *prevhop, uint8_t hops);
*/

//!!!! Does it make sense to always wait for announcement for routing
//table update as long as we receive packets from a node? we can extend
//the timer when getting a packet rather than wait for announcements all
//the time and loose routes this way.

//!!! fix my bad code with respect to static variables

void c_rnd_init();

rimeaddr_t *c_rnd_forward(struct pipe *p, struct stackmodule_i *module);

void c_register_announcement(int i);

void c_received_announcement(struct announcement *a, const rimeaddr_t * from,
                             uint16_t id, uint16_t value);

#endif /* C_RND_ROUTING_H_ */
