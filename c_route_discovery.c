/**
 * \addtogroup routediscovery
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
 * $Id: route-discovery.c,v 1.20 2010/01/27 08:12:56 adamdunkels Exp $
 */

/**
 * \file
 *         Route discovery protocol
 * \author
 *         Adam Dunkels <adam@sics.se>
 */


//:c_route_discovery rdf:type cpan:Module .

#include "contiki.h"
#include "net/rime.h"
#include "net/rime/crime/stack.h"
#include "net/rime/crime/amodule.h"
#include "net/rime/route.h"
#include "net/rime/crime/c_route_discovery.h"

#include <stddef.h>             /* For offsetof */
#include <stdio.h>

struct route_msg {
  rimeaddr_t dest;
  uint8_t rreq_id;
  uint8_t pad;
};

struct rrep_hdr {
  uint8_t rreq_id;
  uint8_t hops;
  rimeaddr_t dest;
  rimeaddr_t originator;
};

#if CONTIKI_TARGET_NETSIM
#include "ether.h"
#endif


#define DEBUG 0
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

/*---------------------------------------------------------------------------*/
static char rrep_pending;       /* A reply for a request is pending. */

/*---------------------------------------------------------------------------*/

static void
send_rreq(struct pipe *p, struct stackmodule_i *module)
{
  const rimeaddr_t *dest = get_node_addr(module->stack_id, 0, 3);

  rimeaddr_t dest_copy;

  struct route_msg *msg;

  rimeaddr_copy(&dest_copy, dest);
  dest = &dest_copy;

  packetbuf_clear();
  msg = packetbuf_dataptr();
  packetbuf_set_datalen(sizeof(struct route_msg));

  msg->pad = 0;
  msg->rreq_id = p->route_discovery_param.rreq_id;
  rimeaddr_copy(&msg->dest, dest);

  //netflood_send(&c->rreqconn, c->rreq_id);
  //c->rreq_id++;
  p->route_discovery_param.rreq_id++;
  p->seq_no = p->route_discovery_param.rreq_id;
}

/*---------------------------------------------------------------------------*/
static void
send_rrep(struct pipe *p, struct stackmodule_i *module)
                //struct route_discovery_conn *c, const rimeaddr_t *dest)
{
  const rimeaddr_t *dest = get_node_addr(module->stack_id, 1, 1);

  struct rrep_hdr *rrepmsg;

  struct route_entry *rt;

  rimeaddr_t saved_dest;

  set_node_addr(module->stack_id + 1, 0, 0,
                get_node_addr(module->stack_id, 0, 0));
  set_node_addr(module->stack_id + 1, 0, 1,
                get_node_addr(module->stack_id, 0, 1));
  set_node_addr(module->stack_id + 1, 0, 2,
                get_node_addr(module->stack_id, 1, 0));
  set_node_addr(module->stack_id + 1, 0, 3,
                get_node_addr(module->stack_id, 0, 1));

  rimeaddr_copy(&saved_dest, dest);

  packetbuf_clear();
  dest = &saved_dest;
  rrepmsg = packetbuf_dataptr();
  packetbuf_set_datalen(sizeof(struct rrep_hdr));
  rrepmsg->hops = 0;
  rimeaddr_copy(&rrepmsg->dest, dest);
  rimeaddr_copy(&rrepmsg->originator, &rimeaddr_node_addr);
  rt = route_lookup(dest);
  if(rt != NULL) {
    PRINTF("%d.%d: send_rrep to %d.%d via %d.%d\n",
           rimeaddr_node_addr.u8[0], rimeaddr_node_addr.u8[1],
           dest->u8[0], dest->u8[1], rt->nexthop.u8[0], rt->nexthop.u8[1]);
    //unicast_send(&c->rrepconn, &rt->nexthop);
    c_send(stack[RREP_STACK_ID].pip,
           stack[RREP_STACK_ID].amodule, stack[RREP_STACK_ID].modno - 2);
  } else {
    PRINTF("%d.%d: no route for rrep to %d.%d\n",
           rimeaddr_node_addr.u8[0], rimeaddr_node_addr.u8[1],
           dest->u8[0], dest->u8[1]);
  }
}

/*---------------------------------------------------------------------------*/
static void
insert_route(const rimeaddr_t * originator, const rimeaddr_t * last_hop,
             uint8_t hops)
{
  PRINTF
    ("%d.%d: Inserting %d.%d into routing table, next hop %d.%d, hop count %d\n",
     rimeaddr_node_addr.u8[0], rimeaddr_node_addr.u8[1], originator->u8[0],
     originator->u8[1], last_hop->u8[0], last_hop->u8[1], hops);

  route_add(originator, last_hop, hops, 0);
}

/*---------------------------------------------------------------------------*/
static void
rrep_packet_received(struct pipe *p, struct stackmodule_i *module)
                //struct unicast_conn *uc, const rimeaddr_t *from)
{
  PRINTF("rrep_packet_received\n");
  struct rrep_hdr *msg = packetbuf_dataptr();

  set_node_addr(module->stack_id, 1, 0,
                get_node_addr(module->stack_id + 1, 1, 0));
  set_node_addr(module->stack_id, 1, 1,
                get_node_addr(module->stack_id + 1, 1, 1));

  struct route_entry *rt;

  rimeaddr_t dest;

  const rimeaddr_t *from = get_node_addr(module->stack_id, 1, 0);

  PRINTF("%d.%d: rrep_packet_received from %d.%d towards %d.%d len %d\n",
         rimeaddr_node_addr.u8[0], rimeaddr_node_addr.u8[1],
         from->u8[0], from->u8[1],
         msg->dest.u8[0], msg->dest.u8[1], packetbuf_datalen());

  PRINTF("from %d.%d hops %d rssi %d lqi %d\n",
         from->u8[0], from->u8[1],
         msg->hops,
         packetbuf_attr(PACKETBUF_ATTR_RSSI),
         packetbuf_attr(PACKETBUF_ATTR_LINK_QUALITY));

  insert_route(&msg->originator, from, msg->hops);

  if(rimeaddr_cmp(&msg->dest, &rimeaddr_node_addr)) {
    PRINTF("rrep for us!\n");
    rrep_pending = 0;
    ctimer_stop(&p->route_discovery_param.timer);
  } else {
    rimeaddr_copy(&dest, &msg->dest);

    rt = route_lookup(&msg->dest);
    if(rt != NULL) {
      PRINTF("forwarding to %d.%d\n", rt->nexthop.u8[0], rt->nexthop.u8[1]);
      msg->hops++;
      //unicast_send(&c->rrepconn, &rt->nexthop);
      //rimeaddr_copy(&stack[RREP_STACK_ID].pip->receiver, &rt->nexthop);
      set_node_addr(module->stack_id, 0, 2, &rt->nexthop);
      set_node_addr(module->stack_id + 1, 0, 2, &rt->nexthop);
      set_node_addr(module->stack_id + 1, 0, 3, &dest);
      c_send(stack[RREP_STACK_ID].pip,
             stack[RREP_STACK_ID].amodule, stack[RREP_STACK_ID].modno - 2);
    } else {
      PRINTF("%d.%d: no route to %d.%d\n", rimeaddr_node_addr.u8[0],
             rimeaddr_node_addr.u8[1], msg->dest.u8[0], msg->dest.u8[1]);
    }
  }
  PRINTF("~rrep_packet_received\n");
}

/*---------------------------------------------------------------------------*/
static int
rreq_packet_received(struct pipe *p, struct stackmodule_i *module)
                //struct netflood_conn *nf, const rimeaddr_t *from,
                  //   const rimeaddr_t *originator, uint8_t seqno, uint8_t hops)
{
  PRINTF("rreq_packet_received\n");
  struct route_msg *msg = packetbuf_dataptr();

  set_node_addr(module->stack_id, 1, 2, &msg->dest);
  const rimeaddr_t *from = get_node_addr(module->stack_id, 1, 0);

  const rimeaddr_t *originator = get_node_addr(module->stack_id, 1, 1);

  uint8_t hops = p->hop_no;

  rimeaddr_t *last_rreq_originator
    = &p->route_discovery_param.last_rreq_originator;
  uint16_t last_rreq_id = p->route_discovery_param.last_rreq_id;

  PRINTF
    ("%d.%d: rreq_packet_received from %d.%d hops %d rreq_id %d last %d.%d/%d\n",
     rimeaddr_node_addr.u8[0], rimeaddr_node_addr.u8[1], from->u8[0],
     from->u8[1], hops, msg->rreq_id, last_rreq_originator->u8[0],
     last_rreq_originator->u8[1], last_rreq_id);

  if(!(rimeaddr_cmp(last_rreq_originator, originator) &&
       p->route_discovery_param.last_rreq_id == msg->rreq_id)) {

    PRINTF
      ("%d.%d: rreq_packet_received: request for %d.%d originator %d.%d / %d\n",
       rimeaddr_node_addr.u8[0], rimeaddr_node_addr.u8[1], msg->dest.u8[0],
       msg->dest.u8[1], originator->u8[0], originator->u8[1], msg->rreq_id);

    rimeaddr_copy(last_rreq_originator, originator);
    p->route_discovery_param.last_rreq_id = msg->rreq_id;

    if(rimeaddr_cmp(&msg->dest, &rimeaddr_node_addr)) {
      PRINTF("%d.%d: route_packet_received: route request for our address\n",
             rimeaddr_node_addr.u8[0], rimeaddr_node_addr.u8[1]);
      PRINTF("from %d.%d hops %d rssi %d lqi %d\n",
             from->u8[0], from->u8[1],
             hops,
             packetbuf_attr(PACKETBUF_ATTR_RSSI),
             packetbuf_attr(PACKETBUF_ATTR_LINK_QUALITY));

      insert_route(originator, from, hops);

      /* Send route reply back to source. */
      send_rrep(p, module);
      return 0;                 /* Don't continue to flood the rreq packet. */
    } else {
      /*      PRINTF("route request for %d\n", msg->dest_id); */
      PRINTF("from %d.%d hops %d rssi %d lqi %d\n",
             from->u8[0], from->u8[1],
             hops,
             packetbuf_attr(PACKETBUF_ATTR_RSSI),
             packetbuf_attr(PACKETBUF_ATTR_LINK_QUALITY));
      insert_route(originator, from, hops);
    }

    return 1;
  }
  PRINTF("~rreq_packet_received\n");
  return 0;                     /* Don't forward packet. */
}

/*---------------------------------------------------------------------------*/
void
c_route_discovery_open(struct pipe *p, struct stackmodule_i *module)
{
  PRINTF("~c_route_discovery_open\n");
}

/*---------------------------------------------------------------------------*/
void
c_route_discovery_close(struct pipe *p, struct stackmodule_i *module)
{
  ctimer_stop(&module->timer);
}

/*---------------------------------------------------------------------------*/
int
c_route_discovery_discover(struct pipe *p, struct stackmodule_i *module)
{
  PRINTF("c_route_discovery_discover\n");
  if(rrep_pending) {
    PRINTF
      ("route_discovery_send: ignoring request because of pending response\n");
    return 0;
  }

  PRINTF("route_discovery_send: sending route request\n");
  ctimer_set(&p->route_discovery_param.timer,
             p->route_discovery_param.timeout, stack_timedout, module);
  rrep_pending = 1;
  send_rreq(p, module);

  PRINTF("~c_route_discovery_discover\n");
  return 1;
}

void
c_route_discovery_recv(struct pipe *p, struct stackmodule_i *module)
{
  PRINTF("c_route_discovery_recv\n");
  if(module->stack_id == RREQ_STACK_ID) {
    rreq_packet_received(p, module);
  } else if(module->stack_id == RREP_STACK_ID) {
    rrep_packet_received(stack[RREQ_STACK_ID].pip,
                         stack[RREQ_STACK_ID].amodule);
  }
  PRINTF("~c_route_discovery_recv\n");
}

void
c_route_discovery_timedout(struct pipe *p, struct stackmodule_i *module)
{
  PRINTF("route_discovery: timeout, timed out\n");
  rrep_pending = 0;
}

/*---------------------------------------------------------------------------*/
/** @} */
