/**
 * \addtogroup rimenetflood
 * @{
 */

/*
 * Copyright (c) 2006, Swedish Institute of Computer Science.
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
 * $Id: netflood.c,v 1.6 2010/01/25 13:54:06 adamdunkels Exp $
 */

/**
 * \file
 *         Best-effort network flooding (netflood)
 * \author
 *         Adam Dunkels <adam@sics.se>
 */

#include "net/rime/crime/c_netflood.h"

#include <string.h>

#define HOPS_MAX 16

struct netflood_hdr {
  uint16_t originator_seq_no;
  rimeaddr_t originator;
  uint16_t hop_no;
};

#define DEBUG 0
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

/*---------------------------------------------------------------------------*/
void
c_netflood_recv(struct pipe *p, struct stackmodule_i *module)
{
  PRINTF("c_netflood_recv \n");
  printaddr(module->stack_id);

  struct netflood_hdr hdr;

  uint8_t hops;

  struct queuebuf *queuebuf;

  memcpy(&hdr, packetbuf_dataptr(), sizeof(struct netflood_hdr));
  hops = hdr.hop_no;
  //rimeaddr_copy(&p->esender, &hdr.originator);
  set_node_addr(module->stack_id, 1, 1, &hdr.originator);

  /* Remember packet if we need to forward it. */
  queuebuf = queuebuf_new_from_packetbuf();

  packetbuf_hdrreduce(sizeof(struct netflood_hdr));
  if(!(rimeaddr_cmp(&hdr.originator, &p->netflood_param.last_originator) &&
       hdr.originator_seq_no <= p->netflood_param.last_originator_seq_no)) {
    if(queuebuf != NULL) {
      queuebuf_to_packetbuf(queuebuf);
      queuebuf_free(queuebuf);
      queuebuf = NULL;
      memcpy(&hdr, packetbuf_dataptr(), sizeof(struct netflood_hdr));

      /* Rebroadcast received packet. */
      if(hops < HOPS_MAX) {
        PRINTF
          ("%d.%d: netflood rebroadcasting %d.%d/%d (%d.%d/%d) hops %d\n",
           rimeaddr_node_addr.u8[0], rimeaddr_node_addr.u8[1],
           hdr.originator.u8[0], hdr.originator.u8[1], hdr.originator_seq_no,
           p->netflood_param.last_originator.u8[0],
           p->netflood_param.last_originator.u8[1],
           p->netflood_param.last_originator_seq_no, hops);
        //rimeaddr_copy(&p->esender, &hdr.originator);
        set_node_addr(module->stack_id, 1, 1, &hdr.originator);
        printaddr(module->stack_id);

        hdr.hop_no++;
        memcpy(packetbuf_dataptr(), &hdr, sizeof(struct netflood_hdr));
        stack_send(&stack[module->stack_id], module->module_id);        //c_send(p, amodule, );
        rimeaddr_copy(&p->netflood_param.last_originator, &hdr.originator);
        p->netflood_param.last_originator_seq_no = hdr.originator_seq_no;
      }
    }
  }
  if(queuebuf != NULL) {
    queuebuf_free(queuebuf);
  }
}

/*---------------------------------------------------------------------------*/
void
c_netflood_sent(struct pipe *p, struct stackmodule_i *module)
{
  PRINTF("netflood packet sent\n");
}

/*---------------------------------------------------------------------------*/
void
c_netflood_dropped(struct pipe *p, struct stackmodule_i *module)
{
  PRINTF("netflood packet dropped\n");
}

/*---------------------------------------------------------------------------*/
void
c_netflood_open(struct pipe *p, struct stackmodule_i *module)
{
  channel_set_attributes(p->channel_no, p->attrlist);
  PRINTF("~netflood_open\n");
}

/*---------------------------------------------------------------------------*/
void
c_netflood_close(struct pipe *p, struct stackmodule_i *module)
{
}

/*---------------------------------------------------------------------------*/
int
c_netflood_send(struct pipe *p, struct stackmodule_i *module)
{

  if(packetbuf_hdralloc(sizeof(struct netflood_hdr))) {
    struct netflood_hdr *hdr = packetbuf_hdrptr();

    rimeaddr_copy(&hdr->originator, &rimeaddr_node_addr);
    rimeaddr_copy(&p->netflood_param.last_originator, &hdr->originator);
    p->netflood_param.last_originator_seq_no =
      hdr->originator_seq_no = p->seq_no;
    hdr->hop_no = 0;
    /*PRINTF("%d.%d: netflood sending %d.%d seqno %d hops %d %s\n",
       rimeaddr_node_addr.u8[0], rimeaddr_node_addr.u8[1],
       hdr->originator.u8[0], hdr->originator.u8[1],
       hdr->originator_seq_no, hdr->hop_no = 0,
       (char *)packetbuf_hdrptr()); */
    printaddr(module->stack_id);
    return 1;
  }
  return 0;
}

/*---------------------------------------------------------------------------*/
void
c_netflood_cancel(struct pipe *p, struct stackmodule_i *module)
{
}

/*---------------------------------------------------------------------------*/
/** @} */
