/**
 * \addtogroup rimeabc
 * @{
 */


/*
 * Copyright (c) 2004, Swedish Institute of Computer Science.
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
 * Author: Adam Dunkels <adam@sics.se>
 *
 * $Id: abc.c,v 1.20 2010/02/23 18:38:05 adamdunkels Exp $
 */

/**
 * \file
 *         Anonymous best-effort local area Broad Cast (abc)
 * \author
 *         Adam Dunkels <adam@sics.se>
 */

#include "contiki-net.h"
#include "net/rime.h"

#include "vsntime.h"


#define DEBUG 0
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

//int startTm1 = 0;

void
c_abc_open(struct pipe *p, struct stackmodule_i *module)
{
  //int startTm = vsnTime_freeRunTime();

  channel_set_attributes(p->channel->channelno, p->channel->attrlist);
  p->channel->hdrsize = chameleon_hdrsize(p->channel->attrlist);
  PRINTF("~c_abc_open \n");

  //int stopTm = vsnTime_freeRunTimeDiff(startTm);
  //printf("abco %d, ", stopTm);
}
/*---------------------------------------------------------------------------*/
void c_abc_close(struct pipe *p, struct stackmodule_i *module) {

	 //int startTm = vsnTime_freeRunTime();
	PRINTF("~c_abc_close \n");
	//int stopTm = vsnTime_freeRunTimeDiff(startTm);
	  //printf("abc %d, ", stopTm);
}
/*---------------------------------------------------------------------------*/
int
c_abc_send(struct pipe *p, struct stackmodule_i *module)
{
	//int startTm = vsnTime_freeRunTime();

  PRINTF("~c_abc_send: packet '%s' on channel %d hdrsize %d\n",
	 (char *) packetbuf_dataptr(), p->channel->channelno, p->channel->hdrsize);
    printaddr(module->stack_id);

    //int stopTm = vsnTime_freeRunTimeDiff(startTm);
    //printf("%d, ", stopTm);
  return 1;
}
/*---------------------------------------------------------------------------*/
void
c_abc_input(struct pipe *p, struct stackmodule_i *module)
{
	 //int startTm = vsnTime_freeRunTime();

  PRINTF("~c_abc_input: packet '%s' on channel %d\n",
     (char *) packetbuf_dataptr(), p->channel->channelno);

  //int stopTm = vsnTime_freeRunTimeDiff(startTm);
   //printf("abcin %d, ", stopTm);
}
/*---------------------------------------------------------------------------*/
void
c_abc_sent(struct pipe *p, struct stackmodule_i *module)
{
  PRINTF("~c_abc_sent: after %d transmissions\n",
	 p->num_tx);
}
/*---------------------------------------------------------------------------*/
void
c_abc_recv(struct pipe *p, struct stackmodule_i *module)
{
	 //int startTm = vsnTime_freeRunTime();

	rimeaddr_t tmpaddr;
	rimeaddr_copy(&tmpaddr, packetbuf_addr(PACKETBUF_ADDR_SENDER));
	set_node_addr(module->stack_id, 1, 0, &tmpaddr);
	 PRINTF("~c_abc_receive: packet %.*s (%d) on channel %d from %d.%d\n",
			 packetbuf_datalen(), (char *)packetbuf_dataptr(),
			 packetbuf_datalen(), p->channel->channelno,
	     tmpaddr.u8[0], tmpaddr.u8[1]);
	 //printaddr(module->stack_id);


	 //int stopTm = vsnTime_freeRunTimeDiff(startTm);
	  //printf("abcr %d, ", stopTm);
}

void c_start_tm(){
	//startTm1 = vsnTime_freeRunTime();
}
int c_get_start_tm(){
	//return startTm1;
}

/** @} */
