/**
 * \addtogroup crimecabc
 * @{
 */

/*
 * Copyright (c) 2013, Jozef Stefan Institute.
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
 * Author: Carolina Fortuna <carolina.fortuna@ijs.si>
 *
 * $Id: c_abc.c,v 0.0 2013/03/06 18:38:05 adamdunkels Exp $
 */

/**
 * \file
 *         Composable Anonymous best-effort local area Broad Cast (c_abc)
 * \author
 *         Carolina Fortuna <carolina.fortuna@ijs.si>
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

#define EVAL 1
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

/**
 * \brief      Set up an anonymous best-effort broadcast connection
 * \param c    A pointer to a struct abc_conn
 * \param channel The channel on which the connection will operate
 * \param u    A struct abc_callbacks with function pointers to functions that will be called when a packet has been received
 *
 *             This function sets up an abc connection on the
 *             specified channel. The caller must have allocated the
 *             memory for the struct abc_conn, usually by declaring it
 *             as a static variable.
 *
 *             The struct abc_callbacks pointer must point to a structure
 *             containing a pointer to a function that will be called
 *             when a packet arrives on the channel.
 *
 */
void c_abc_open(struct pipe *p, struct stackmodule_i *module) {
	int start_tm = START_TM;
	channel_set_attributes(p->channel->channelno, p->channel->attrlist);
	p->channel->hdrsize = chameleon_hdrsize(p->channel->attrlist);
	PRINTF("~c_abc_open \n");
	PRINTFE("\n %d \n", DURATION_TM(start_tm));
}

/*---------------------------------------------------------------------------*/
void c_abc_close(struct pipe *p, struct stackmodule_i *module) {
	int start_tm = START_TM;
	PRINTF("~c_abc_close \n");
	PRINTFE("\n %d \n", DURATION_TM(start_tm));
}

/*---------------------------------------------------------------------------*/
int c_abc_send(struct pipe *p, struct stackmodule_i *module) {
	int start_tm = START_TM;
	PRINTF("~c_abc_send: packet '%s' on channel %d hdrsize %d\n",
			(char *)packetbuf_dataptr(), p->channel->channelno,
			p->channel->hdrsize);
	PRINTFE("\n %d \n", DURATION_TM(start_tm));
	return 1;
}

/*---------------------------------------------------------------------------*/
void c_abc_recv(struct pipe *p, struct stackmodule_i *module) {
	int start_tm = START_TM;
	rimeaddr_t tmpaddr;

	rimeaddr_copy(&tmpaddr, packetbuf_addr(PACKETBUF_ADDR_SENDER));
	set_node_addr(module->stack_id, 1, 0, &tmpaddr);
	PRINTF("~c_abc_receive: packet %.*s (%d) on channel %d from %d.%d\n",
			packetbuf_datalen(), (char *)packetbuf_dataptr(),
			packetbuf_datalen(), p->channel->channelno,
			tmpaddr.u8[0], tmpaddr.u8[1]);
	PRINTFE("\n %d \n", DURATION_TM(start_tm));
}

/*---------------------------------------------------------------------------*/
void c_abc_sent(struct pipe *p, struct stackmodule_i *module) {
	PRINTF("~c_abc_sent: after %d transmissions\n", p->num_tx);
}

/** @} */
