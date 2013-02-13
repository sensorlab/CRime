/*
 * c_learn.c
 *
 *  Created on: Aug 10, 2012
 *      Author: carolina
 */

#include <stdio.h>

#include "lib/list.h"
#include "lib/memb.h"
#include "c_learn.h"

LIST(route_model);
MEMB(model_mem, struct route_model, NUM_RT_ENTRIES);

void
learn_init(void)
{
  list_init(route_model);
  memb_init(&route_mem);
}

struct route_model *
model_lookup(const rimeaddr_t *dest, const rimeaddr_t *nexthop)
{
	struct route_model *m;

  /* Find the model. */
  for(m = list_head(route_model); m != NULL; m = list_item_next(m)) {

    if(rimeaddr_cmp(dest, &m->dest)) {
      if(rimeaddr_cmp(nexthop, &m->nexthop)) {
    	  return route_model;
      }
    }
  }
  return NULL;
}

void
update_model(const rimeaddr_t *dest, const rimeaddr_t *nexthop,
		  uint8_t cost)
{
	struct route_model *m;

	  m = model_lookup(dest);
	  if(m == NULL) {
	    m = memb_alloc(&model_mem);
	    if(m == NULL) {
	      m = list_chop(route_model);
	    }
	  }

	  rimeaddr_copy(&m->dest, dest);
	  rimeaddr_copy(&m->nexthop, nexthop);
	  uint8_t oldcost = e->cost;
	  e->cost = oldcost + cost;

	  /* New entry goes first. */
	  list_push(route_table, e);

	  PRINTF("update_model: new entry to %d.%d with nexthop %d.%d and cost %d, table size %d\n",
		 e->dest.u8[0], e->dest.u8[1],
		 e->nexthop.u8[0], e->nexthop.u8[1],
		 e->cost,
		 list_length(route_table));
}
