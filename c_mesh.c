/**
 * \addtogroup rimemesh
 * @{
 */

/*
 * Copyright (c) 2007, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 * $Id: mesh.c,v 1.20 2009/12/18 14:57:15 nvt-se Exp $
 */

/**
 * \file
 *         A mesh routing protocol
 * \author
 *         Adam Dunkels <adam@sics.se>
 */

#include "contiki.h"
#include "net/rime.h"
//#include "net/rime/route.h"
#include "net/rime/crime/c_mesh.h"

#include <stddef.h> /* For offsetof */

#define DEBUG 0
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

/*---------------------------------------------------------------------------*/
rimeaddr_t *
c_mesh_forward(struct pipe *p, struct stackmodule_i *module)
//(struct multihop_conn *multihop,   const rimeaddr_t *originator,
//const rimeaddr_t *dest,  const rimeaddr_t *prevhop, uint8_t hops)
{
	PRINTF("c_mesh_forward \n");
  struct route_entry *rt;

  rimeaddr_t *tmpaddr = get_node_addr(module->stack_id, 0, 3);
  rt = route_lookup(tmpaddr);
  if(rt == NULL) {
	 int module_id = stack[RREQ_STACK_ID].modno;
	 c_send(stack[RREQ_STACK_ID].pip,
	 	stack[RREQ_STACK_ID].amodule,
	 	module_id - 1);
	  //stack_discover(&stack[module->stack_id + 1]);
    return NULL;
  } else {
    route_refresh(rt);
  }

  PRINTF("~c_mesh_forward \n");

  return &rt->nexthop;
}
/*---------------------------------------------------------------------------*/
static void
found_route(struct route_discovery_conn *rdc, const rimeaddr_t *dest)
{
  /*struct mesh_conn *c = (struct mesh_conn *)
    ((char *)rdc - offsetof(struct mesh_conn, route_discovery_conn));

  if(c->queued_data != NULL &&
     rimeaddr_cmp(dest, &c->queued_data_dest)) {
    queuebuf_to_packetbuf(c->queued_data);
    queuebuf_free(c->queued_data);
    c->queued_data = NULL;
    if(multihop_send(&c->multihop, dest)) {
      c->cb->sent(c);
    } else {
      c->cb->timedout(c);
    }
  }*/
}
/*---------------------------------------------------------------------------*/
void
c_mesh_timedout(struct pipe *p, struct stackmodule_i *module)
{
	PRINTF("c_mesh_timedout\n");
	if(p->mesh_param.queued_data != NULL) {
	    queuebuf_free(p->mesh_param.queued_data);
	    p->mesh_param.queued_data = NULL;
	  }
}
/*---------------------------------------------------------------------------*/

void
c_mesh_open(struct pipe *p, struct stackmodule_i *module)
{
  route_init();
  PRINTF("~c_mesh_open\n");
}
/*---------------------------------------------------------------------------*/
void
c_mesh_close(struct pipe *p, struct stackmodule_i *module)
{
}
/*---------------------------------------------------------------------------*/
int
c_mesh_send(struct pipe *p, struct stackmodule_i *module)
{
	PRINTF("c_mesh_send\n");
	int could_send;

	rimeaddr_t *tmpaddr = get_node_addr(module->stack_id, 0, 3);
	PRINTF("%d.%d: mesh_send to %d.%d\n",
	 rimeaddr_node_addr.u8[0], rimeaddr_node_addr.u8[1],
	 tmpaddr->u8[0], tmpaddr->u8[1]);
	printaddr(module->stack_id);

	could_send = stack_send(stack, module->module_id - 1);

	if(!could_send) {
	  if(p->mesh_param.queued_data != NULL) {
	    queuebuf_free(p->mesh_param.queued_data);
	  }
    PRINTF("mesh_send: queueing data, sending rreq\n");

    p->mesh_param.queued_data = queuebuf_new_from_packetbuf();
    tmpaddr = get_node_addr(module->stack_id, 0, 3);
	rimeaddr_copy(&p->mesh_param.queued_data_dest, tmpaddr);

    int module_id = stack[RREQ_STACK_ID].modno;
    set_node_addr(module->stack_id + 1, 0, 0,
    		get_node_addr(module->stack_id, 0, 0));
    set_node_addr(module->stack_id + 1, 0, 1,
        		get_node_addr(module->stack_id, 0, 1));
    set_node_addr(module->stack_id + 1, 0, 2,
        		get_node_addr(module->stack_id, 0, 2));
    set_node_addr(module->stack_id + 1, 0, 3,
        		get_node_addr(module->stack_id, 0, 3));

    c_send(stack[module->stack_id + 1].pip,
    		stack[module->stack_id + 1].amodule,
    		module_id - 1);
  }
  PRINTF("~c_mesh_send\n");
  return 0;
}
/*---------------------------------------------------------------------------*/
void
c_mesh_recv(struct pipe *p, struct stackmodule_i *module)
{
	PRINTF("c_mesh_recv\n");
	printaddr(module->stack_id);
 /*PRINTF("Data received from %d.%d: %.*s (%d)\n",
	p->esender.u8[0], p->esender.u8[1],
	packetbuf_datalen(), (char *)packetbuf_dataptr(), packetbuf_datalen());*/

  struct route_entry *rt;

  /* Refresh the route when we hear a packet from a neighbor. */
  rimeaddr_t *tmpaddr = get_node_addr(module->stack_id, 1, 1);
  rt = route_lookup(tmpaddr);
  if(rt != NULL) {
    route_refresh(rt);
  }
}

void c_mesh_sent(struct pipe *p, struct stackmodule_i *module) {
	PRINTF("c_mesh_sent\n");
}

/** @} */
