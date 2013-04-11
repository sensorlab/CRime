/**
 * \addtogroup crime
 * @{
 */

/**
 * \defgroup crimec_mesh Composable Mesh routing
 * @{
 *
 * The c_mesh module sends packets using multi-hop routing to a specified
 * receiver somewhere in the network.
 *
 * \section channels Channels
 *
 * The mesh module uses 3 channel; one for the multi-hop forwarding
 * (\ref crimec_multihop "c_multihop") and two for the route disovery (\ref
 * c_route_discovery "route-discovery").
 *
 */

/*
 * Copyright (c) 2012, Jozef Stefan Institute.
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
 *
 */

/**
 * \file
 *         File for the CRime module Composeable Mesh routing protocol.
 *
 * \author
 *         Carolina Fortuna <carolina.fortuna@ijs.si>
 */

#include "contiki.h"
#include "net/rime.h"
#include "net/rime/crime/c_mesh.h"

#include <stddef.h>             /* For offsetof */

#define DEBUG 0
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

#define EVAL 0
#if EVAL
#include <stdio.h>
#define START_TM vsnTime_freeRunTime()
#define DURATION_TM(x) vsnTime_freeRunTimeDiff(x)
#define PRINTFE(...) printf(__VA_ARGS__)
#else
#define START_TM 0
#define DURATION_TM(x) 0
#define PRINTFE(...)
#endif

/*---------------------------------------------------------------------------*/
rimeaddr_t *
c_mesh_forward(struct pipe *p, struct stackmodule_i *module) {
	PRINTF("c_mesh_forward \n");
	struct route_entry *rt;
	rimeaddr_t *tmpaddr = get_node_addr(module->stack_id, 0, 3);

	rt = route_lookup(tmpaddr);
	if (rt == NULL) {
		int module_id = stack[RREQ_STACK_ID].modno;

		c_send(stack[RREQ_STACK_ID].pip, stack[RREQ_STACK_ID].amodule,
				module_id - 1);
		return NULL;
	} else {
		route_refresh(rt);
	} PRINTF("~c_mesh_forward \n");

	return &rt->nexthop;
}

/*---------------------------------------------------------------------------*/
static void found_route(struct route_discovery_conn *rdc,
		const rimeaddr_t * dest) {
}

/*---------------------------------------------------------------------------*/
void c_mesh_timedout(struct pipe *p, struct stackmodule_i *module) {
	PRINTF("c_mesh_timedout\n");
	if (p->mesh_param.queued_data != NULL) {
		queuebuf_free(p->mesh_param.queued_data);
		p->mesh_param.queued_data = NULL;
	}
}

/*---------------------------------------------------------------------------*/

void c_mesh_open(struct pipe *p, struct stackmodule_i *module) {
	route_init();
	PRINTF("~c_mesh_open\n");
}

/*---------------------------------------------------------------------------*/
void c_mesh_close(struct pipe *p, struct stackmodule_i *module) {
}

/*---------------------------------------------------------------------------*/
int c_mesh_send(struct pipe *p, struct stackmodule_i *module) {
	PRINTF("c_mesh_send\n");
	int could_send;

	rimeaddr_t *tmpaddr = get_node_addr(module->stack_id, 0, 3);

	PRINTF("%d.%d: mesh_send to %d.%d\n",
			rimeaddr_node_addr.u8[0], rimeaddr_node_addr.u8[1],
			tmpaddr->u8[0], tmpaddr->u8[1]);
	could_send = stack_send(stack, module->module_id - 1);

	if (!could_send) {
		if (p->mesh_param.queued_data != NULL) {
			queuebuf_free(p->mesh_param.queued_data);
		} PRINTF("mesh_send: queueing data, sending rreq\n");

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
				stack[module->stack_id + 1].amodule, module_id - 1);
	} PRINTF("~c_mesh_send\n");
	return 0;
}

/*---------------------------------------------------------------------------*/
void c_mesh_recv(struct pipe *p, struct stackmodule_i *module) {
	PRINTF("c_mesh_recv\n");
	/*PRINTF("Data received from %d.%d: %.*s (%d)\n",
	 p->esender.u8[0], p->esender.u8[1],
	 packetbuf_datalen(), (char *)packetbuf_dataptr(), packetbuf_datalen()); */

	struct route_entry *rt;
	/* Refresh the route when we hear a packet from a neighbor. */
	rimeaddr_t *tmpaddr = get_node_addr(module->stack_id, 1, 1);

	rt = route_lookup(tmpaddr);
	if (rt != NULL) {
		route_refresh(rt);
	}
}

void c_mesh_sent(struct pipe *p, struct stackmodule_i *module) {
	PRINTF("c_mesh_sent\n");
}

/** @} */
/** @} */
