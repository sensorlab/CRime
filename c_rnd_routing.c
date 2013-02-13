/*
 * c_rnd_routing.c
 *
 *  Created on: Apr 6, 2012
 *      Author: carolina
 */

#include "c_rnd_routing.h"

#define DEBUG 0
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

struct example_neighbor {
  struct example_neighbor *next;
  rimeaddr_t addr;
  struct ctimer ctimer;
};

static struct announcement example_announcement;
LIST(neighbor_table);
MEMB(neighbor_mem, struct example_neighbor, MAX_NEIGHBORS);

/*---------------------------------------------------------------------------*/
/*
 * This function is called by the ctimer present in each neighbor
 * table entry. The function removes the neighbor from the table
 * because it has become too old.
 */
static void
remove_neighbor(void *n)
{
  struct example_neighbor *e = n;

  list_remove(neighbor_table, e);
  memb_free(&neighbor_mem, e);
}

void c_register_announcement(int stackid){
	static struct ctimer at;
	/* Register an announcement with the same announcement ID as the
	Rime channel we use to open the multihop connection above. */
	announcement_register(&example_announcement,
				   stack[stackid].pip->channel_no,
				   c_received_announcement);
	announcement_set_value(&example_announcement, rimeaddr_node_addr.u8[0]);
	ctimer_set(&at, CLOCK_SECOND * 140, c_register_announcement, 0);
}

/*---------------------------------------------------------------------------*/
/*
 * This function is called when an incoming announcement arrives. The
 * function checks the neighbor table to see if the neighbor is
 * already present in the list. If the neighbor is not present in the
 * list, a new neighbor table entry is allocated and is added to the
 * neighbor table.
 */

void
c_received_announcement(struct announcement *a, const rimeaddr_t *from,
		      uint16_t id, uint16_t value)
{
  struct example_neighbor *e;

  PRINTF("Got announcement from %d.%d, id %d, value %d\n",
      from->u8[0], from->u8[1], id, value);

  /* We received an announcement from a neighbor so we need to update
     the neighbor list, or add a new entry to the table. */
  for(e = list_head(neighbor_table); e != NULL; e = e->next) {
    if(rimeaddr_cmp(from, &e->addr)) {
      /* Our neighbor was found, so we update the timeout. */
      ctimer_set(&e->ctimer, NEIGHBOR_TIMEOUT, remove_neighbor, e);
      return;
    }
  }

  /* The neighbor was not found in the list, so we add a new entry by
     allocating memory from the neighbor_mem pool, fill in the
     necessary fields, and add it to the list. */
  e = memb_alloc(&neighbor_mem);
  if(e != NULL) {
    rimeaddr_copy(&e->addr, from);
    list_add(neighbor_table, e);
    ctimer_set(&e->ctimer, NEIGHBOR_TIMEOUT, remove_neighbor, e);
  }
}

/*---------------------------------------------------------------------------*/
/*
 * This function is called to forward a packet. The function picks a
 * random neighbor from the neighbor list and returns its address. The
 * multihop layer sends the packet to this address. If no neighbor is
 * found, the function returns NULL to signal to the multihop layer
 * that the packet should be dropped.
 */

rimeaddr_t* c_rnd_forward(struct pipe *p, struct stackmodule_i *module){
	 /* Find a random neighbor to send to. */
	  int num, i;
	  struct example_neighbor *n;

	  if(list_length(neighbor_table) > 0) {
	    num = random_rand() % list_length(neighbor_table);
	    i = 0;
	    for(n = list_head(neighbor_table); n != NULL && i != num; n = n->next) {
	      ++i;
	    }
	    if(n != NULL) {
	    	//rimeaddr_t *tmpaddr = get_node_addr(module->stack_id, 0, 3);
	    	PRINTF("%d.%d: Forwarding packet from %d.%d to %d.%d (%d in list), hops %d\n",
		     rimeaddr_node_addr.u8[0], rimeaddr_node_addr.u8[1],
		     tmpaddr->u8[0], tmpaddr->u8[1],
		     n->addr.u8[0], n->addr.u8[1], num,
		     packetbuf_attr(PACKETBUF_ATTR_HOPS));
	      return &n->addr;
	    }
	  }
	  PRINTF("%d.%d: did not find a neighbor to foward to\n",
		 rimeaddr_node_addr.u8[0], rimeaddr_node_addr.u8[1]);
	  return NULL;
}

void c_rnd_init() {
	/* Initialize the memory for the neighbor table entries. */
	  	  memb_init(&neighbor_mem);
	   /* Initialize the list used for the neighbor table. */
	  	  list_init(neighbor_table);
}

