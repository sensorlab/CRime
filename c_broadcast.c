/**
 * \addtogroup rimebroadcast
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
 * $Id: broadcast.c,v 1.3 2010/02/23 18:38:05 adamdunkels Exp $
 */

/**
 * \file
 *         Identified best-effort local area broadcast (broadcast)
 * \author
 *         Adam Dunkels <adam@sics.se>
 */

#include "contiki-net.h"
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
c_broadcast_open(struct pipe *p, struct stackmodule_i *module)
{
  channel_set_attributes(p->channel_no, p->attrlist);
  PRINTF("c_broadcast_open\n");
}

/*---------------------------------------------------------------------------*/
void
c_broadcast_close(struct pipe *p, struct stackmodule_i *module)
{
  PRINTF("c_broadcast_close\n");
}

/*---------------------------------------------------------------------------*/
int
c_broadcast_send(struct pipe *p, struct stackmodule_i *module)
{
  /*PRINTF("c_broadcast_send: %d.%d: broadcast packet\n",
     rimeaddr_node_addr.u8[0],rimeaddr_node_addr.u8[1]); */
  packetbuf_set_addr(PACKETBUF_ADDR_SENDER, &rimeaddr_node_addr);
  PRINTF("c_broadcast_send \n");
  printaddr(module->stack_id);
  return 1;
}

/*---------------------------------------------------------------------------*/
void
c_broadcast_recv(struct pipe *p, struct stackmodule_i *module)
{
  PRINTF("c_broadcast_recv \n");
  PRINTF("c_broadcast_recv: %d.%d received packet %s from %d.%d\n",
         rimeaddr_node_addr.u8[0], rimeaddr_node_addr.u8[1],
         (char *)packetbuf_dataptr(), p->in_sender.u8[0], p->in_sender.u8[1]);
  PRINTF("~c_broadcast_recv \n");
  printaddr(module->stack_id);

}

/*---------------------------------------------------------------------------*/
void
c_broadcast_sent(struct pipe *p, struct stackmodule_i *module)
{
  PRINTF("%d.%d: sent status %d num_tx %d\n",
         rimeaddr_node_addr.u8[0], rimeaddr_node_addr.u8[1],
         p->status, p->num_tx);
}

/** @} */
