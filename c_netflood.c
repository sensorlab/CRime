/**
 * \addtogroup crime
 * @{
 */

/**
 * \defgroup crimec_netflood Composable best-effort network flooding
 * @{
 *
 * The netflood module does best-effort flooding.
 *
 * The c_netflood primitive sends a single packet to all nodes in the
 * network. The netflood primitive may use c_polite broadcasts at every hop
 * to reduce the number of redundant transmissions.  The netflood
 * primitive does not perform retransmissions of flooded packets and
 * packets are not tagged with version numbers.  Instead, the netflood
 * primitive sets the end-to-end sender and end-to-end packet ID
 * attributes on the packets it sends.  A forwarding node saves the
 * end-to-end sender and packet ID of the last packet it forwards and
 * does not forward a packet if it has the same end-to-end sender and
 * packet ID as the last packet.  This reduces the risk of routing
 * loops, but does not eliminate them entirely as the netflood
 * primitive saves the attributes of the latest packet seen only.
 * Therefore, the netflood primitive also uses the time to live
 * attribute, which is decreased by one before forwarding a packet.
 * If the time to live reaches zero, the primitive does not forward
 * the packet.
 *
 * \section channels Channels
 *
 * The netflood module uses 1 channel.
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

#include "net/rime/crime/c_netflood.h"

#include <string.h>

#define HOPS_MAX 16

#define DEBUG 1
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
void c_netflood_recv(struct pipe *p, struct stackmodule_i *module) {
  PRINTF("c_netflood_recv \n");
  memcpy(&p->netflood_param.hdr, packetbuf_dataptr(), sizeof(struct netflood_hdr));
  p->netflood_param.hops = p->netflood_param.hdr.hop_no;
  p->hop_no=p->netflood_param.hops;
  //rimeaddr_copy(&p->esender, &p->netflood_param.hdr.originator);
  set_node_addr(module->stack_id, 1, 1, &p->netflood_param.hdr.originator);
  printaddr(module->stack_id);
  /*rimeaddr_t esender;
  rimeaddr_copy(&esender, get_node_addr(module->stack_id, 1, 1));
  PRINTF("esender: %d.%d \n", esender.u8[0], esender.u8[1]);*/

  /* Remember packet if we need to forward it. */
  p->netflood_param.queuebuf = queuebuf_new_from_packetbuf();

  packetbuf_hdrreduce(sizeof(struct netflood_hdr));

  PRINTF("originator: %d.%d \n", p->netflood_param.hdr.originator.u8[0], p->netflood_param.hdr.originator.u8[1]);
  PRINTF("last_originator: %d.%d \n", p->netflood_param.last_originator.u8[0], p->netflood_param.last_originator.u8[1]);

  if ((rimeaddr_cmp(&p->netflood_param.hdr.originator, &p->netflood_param.last_originator)
      && p->netflood_param.hdr.originator_seq_no<= p->netflood_param.last_originator_seq_no)) {
	PRINTF("no rreq! originator equals to last originator\n");
	stack[module->stack_id].not_dest_flg = 1;
  }
  /*if (p->netflood_param.queuebuf != NULL) {
    queuebuf_free(p->netflood_param.queuebuf);
  }*/
}

/*---------------------------------------------------------------------------*/
void c_netflood_sent(struct pipe *p, struct stackmodule_i *module) {
  PRINTF("netflood packet sent\n");
}

/*---------------------------------------------------------------------------*/
void c_netflood_dropped(struct pipe *p, struct stackmodule_i *module) {
  PRINTF("netflood packet dropped\n");
}

/*---------------------------------------------------------------------------*/
void c_netflood_open(struct pipe *p, struct stackmodule_i *module) {
  channel_set_attributes(p->channel_no, p->attrlist);
  PRINTF("~netflood_open\n");
}

/*---------------------------------------------------------------------------*/
void c_netflood_close(struct pipe *p, struct stackmodule_i *module) {
}

/*---------------------------------------------------------------------------*/
int c_netflood_send(struct pipe *p, struct stackmodule_i *module) {
  if (p->netflood_param.doFlood == 1) {  
      if (p->netflood_param.queuebuf != NULL) {
        queuebuf_to_packetbuf(p->netflood_param.queuebuf);
        queuebuf_free(p->netflood_param.queuebuf);
        p->netflood_param.queuebuf = NULL;
        memcpy(&p->netflood_param.hdr, packetbuf_dataptr(), sizeof(struct netflood_hdr));
        /* Rebroadcast received packet. */
        if (p->netflood_param.hops < HOPS_MAX) {
          PRINTF
          ("%d.%d: netflood rebroadcasting %d.%d/%d (%d.%d/%d) hops %d\n",
           rimeaddr_node_addr.u8[0], rimeaddr_node_addr.u8[1],
           p->netflood_param.hdr.originator.u8[0], p->netflood_param.hdr.originator.u8[1],
           p->netflood_param.hdr.originator_seq_no, p->netflood_param.last_originator.u8[0],
           p->netflood_param.last_originator.u8[1],
           p->netflood_param.last_originator_seq_no, p->netflood_param.hops);
          set_node_addr(module->stack_id, 1, 1, &p->netflood_param.hdr.originator);
          printaddr(module->stack_id);
          p->netflood_param.hdr.hop_no++;
          memcpy(packetbuf_dataptr(), &p->netflood_param.hdr, sizeof(struct netflood_hdr));          
          rimeaddr_copy(&p->netflood_param.last_originator,
                        &p->netflood_param.hdr.originator);
          p->netflood_param.last_originator_seq_no = p->netflood_param.hdr.originator_seq_no;

        }
      }
      p->netflood_param.doFlood = 0;
      return 1;
   
  } else {
    PRINTF("c_netflood_send \n");
    if (packetbuf_hdralloc(sizeof(struct netflood_hdr))) {
      struct netflood_hdr *hdr = packetbuf_hdrptr();
      rimeaddr_copy(&hdr->originator, &rimeaddr_node_addr);
      rimeaddr_copy(&p->netflood_param.last_originator, &hdr->originator);
      p->netflood_param.last_originator_seq_no = hdr->originator_seq_no
          = p->seq_no;
      hdr->hop_no = 0;     
      printaddr(module->stack_id);
      return 1;
    }
    return 0;
  }
  if (p->netflood_param.queuebuf != NULL) {
    queuebuf_free(p->netflood_param.queuebuf);
  }
}

/*---------------------------------------------------------------------------*/
void c_netflood_cancel(struct pipe *p, struct stackmodule_i *module) {
}

/*---------------------------------------------------------------------------*/
/** @} */
