/**
 * \addtogroup rimemh
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
 * $Id: multihop.c,v 1.7 2009/11/08 19:40:17 adamdunkels Exp $
 */

/**
 * \file
 *         Multihop forwarding
 * \author
 *         Adam Dunkels <adam@sics.se>
 */

#include "contiki.h"
#include "net/rime.h"
#include "net/rime/crime/c_multihop.h"
#include "net/rime/crime/c_rnd_routing.h"
#include "net/rime/crime/amodule.h"

#include <string.h>

#define DEBUG 0
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

/*---------------------------------------------------------------------------*/
void
c_multihop_open(struct pipe *p, struct stackmodule_i *module)
{
	channel_set_attributes(p->channel_no, p->attrlist);
	c_rnd_init();
}
/*---------------------------------------------------------------------------*/
void
c_multihop_close(struct pipe *p, struct stackmodule_i *module)
{
}
/*---------------------------------------------------------------------------*/
int
c_multihop_send(struct pipe *p, struct stackmodule_i *module)
{
	PRINTF("c_multihop_send \n");
	//printaddr(module->stack_id);
  packetbuf_compact();
  rimeaddr_t *nexthop = (rimeaddr_t *) c_forward(stack[module->stack_id].pip,
		  stack[module->stack_id].amodule,
		  module->module_id);

  if(nexthop == NULL) {
    PRINTF("multihop_send: no route\n");
    return 0;
  } else {
	  set_node_addr(module->stack_id, 0, 2, nexthop);
    PRINTF("multihop_send: sending data towards %d.%d\n",
	   nexthop->u8[0], nexthop->u8[1]);

    //rimeaddr_copy(&p->receiver, nexthop);
    set_node_addr(module->stack_id, 0, 2, nexthop);
    rimeaddr_t *tmpaddr = get_node_addr(module->stack_id, 0, 3);
    packetbuf_set_addr(PACKETBUF_ADDR_ERECEIVER, tmpaddr);
    tmpaddr = get_node_addr(module->stack_id, 0, 1);
    packetbuf_set_addr(PACKETBUF_ADDR_ESENDER, tmpaddr);
    packetbuf_set_attr(PACKETBUF_ATTR_HOPS, 1);
    return 1;
  }
}

void c_multihop_recv(struct pipe *p, struct stackmodule_i *module){
	PRINTF("c_multihop_recv \n");
	printaddr(module->stack_id);

	  rimeaddr_t *nexthop = NULL;
	  rimeaddr_t sender, receiver;

	  /* Copy the packet attributes to avoid them being overwritten or
	     cleared by an application program that uses the packet buffer for
	     its own needs. */
	  rimeaddr_copy(&sender, packetbuf_addr(PACKETBUF_ADDR_ESENDER));
	  rimeaddr_copy(&receiver, packetbuf_addr(PACKETBUF_ADDR_ERECEIVER));
	  set_node_addr(module->stack_id, 1, 1, &sender);
	  set_node_addr(module->stack_id, 1, 3, &receiver);
	  //rimeaddr_copy(&p->in_esender, packetbuf_addr(PACKETBUF_ADDR_ESENDER));
	  //rimeaddr_copy(&p->in_ereceiver, packetbuf_addr(PACKETBUF_ADDR_ERECEIVER));

	  if(rimeaddr_cmp(packetbuf_addr(PACKETBUF_ADDR_ERECEIVER),
					 &rimeaddr_node_addr)) {
	    PRINTF("for us!\n");
	    p->multihop_param.hop_no = packetbuf_attr(PACKETBUF_ATTR_HOPS);
	  } else {
	    nexthop = c_forward(p, module, stack[module->stack_id].modno);

	    packetbuf_set_attr(PACKETBUF_ATTR_HOPS,
				 packetbuf_attr(PACKETBUF_ATTR_HOPS) + 1);
	    if(nexthop) {
	    	set_node_addr(module->stack_id, 0, 2, nexthop);
	    	PRINTF("forwarding to %d.%d\n", nexthop->u8[0], nexthop->u8[1]);
	    	if (module->stack_id < STACKNO) {
	    		stack_send(&stack[module->stack_id], module->module_id);
	    	}
	    }
	  }
}

rimeaddr_t * c_multihop_forward(struct pipe *p, struct stackmodule_i *module){
	PRINTF("multihop forward \n");
	  return NULL;
}

void
c_multihop_sent(struct pipe *p, struct stackmodule_i *module) {
	PRINTF("multihop sent \n");
}
/*---------------------------------------------------------------------------*/
/** @} */
