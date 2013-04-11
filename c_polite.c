/**
 * \addtogroup crime
 * @{
 */

/**
 * \defgroup crimec_abc Polite anonymous best effort local broadcast
 * @{
 *
 * The c_polite module sends one packet within one
 * time interval. If a packet with the same header is received from a
 * neighbor within the interval, the packet is not sent.
 *
 * The polite primitive is a generalization of the polite gossip
 * algorithm from Trickle (Levis et al, NSDI 2004).  The polite gossip
 * algorithm is designed to reduce the total amount of packet
 * transmissions by not repeating a message that other nodes have
 * already sent.  The purpose of the polite primitive is to
 * avoid that multiple copies of a specific set of packet attributes
 * is sent during a time interval.
 *
 *
 * The c_polite module does not add any packet attributes to
 * outgoing packets apart from those added by the upper layer.
 *
 * \section channels Channels
 *
 * The c_polite module uses 1 channel.
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
 *         File for the Polite anonymous best effort local broadcast
 *
 * \author
 *         Carolina Fortuna <carolina.fortuna@ijs.si>
 */

#include "net/rime.h"
#include "net/rime/crime/c_polite.h"
#include "lib/random.h"

#include <string.h>

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

#ifndef MAX
#define MAX(a,b) ((a) > (b)? (a) : (b))
#endif /* MAX */

#ifndef MIN
#define MIN(a, b) ((a) < (b)? (a) : (b))
#endif /* MIN */

/*---------------------------------------------------------------------------*/
void c_polite_sent(struct pipe *p, struct stackmodule_i *module) {

	PRINTF("%d.%d: sent status %d num_tx %d\n",
			rimeaddr_node_addr.u8[0], rimeaddr_node_addr.u8[1],
			p->status, p->num_tx);
}

/*---------------------------------------------------------------------------*/
void c_polite_open(struct pipe *p, struct stackmodule_i *module) {
	channel_set_attributes(p->channel_no, p->attrlist);
	PRINTF("~c_polite_open \n");
}

/*---------------------------------------------------------------------------*/
void c_polite_close(struct pipe *p, struct stackmodule_i *module) {
	if (module->time_trigger_flg == 1) {
		ctimer_stop(&module->timer);
		if (p->buf != NULL) {
			queuebuf_free(p->buf);
			p->buf = NULL;
			module->time_trigger_flg = 0;
		}
	} PRINTF("~c_polite_close \n");
}

/*---------------------------------------------------------------------------*/
int c_polite_send(struct pipe *p, struct stackmodule_i *module) {
	PRINTF("c_polite_send \n");
	printaddr(module->stack_id);

	if (p->buf != NULL) {
		/* If we are already about to send a packet, we cancel the old one. */
		PRINTF("%d.%d: c_polite_send (sender%d.%d): cancel old send\n",
				p->out_sender.u8[0], p->out_sender.u8[1],
				rimeaddr_node_addr.u8[0], rimeaddr_node_addr.u8[1]);
		queuebuf_free(p->buf);
	}
	p->buf = queuebuf_new_from_packetbuf();

	if (module->trigger_interval == 0) {
		PRINTF("%d.%d: c_polite_send: interval 0\n",
				rimeaddr_node_addr.u8[0], rimeaddr_node_addr.u8[1]);
		return 1;
	} else {
		p->buf = queuebuf_new_from_packetbuf();
		if (p->buf != NULL) {
			clock_time_t new_trigger = module->trigger_interval / 2
					+ (random_rand() % (module->trigger_interval / 2));
			module->trigger_interval = new_trigger;
			PRINTF("~c_polite_send '%s'\n", (char *)queuebuf_dataptr(p->buf));
			return 1;
		} PRINTF("%d.%d: ipolite_send: could not allocate queue buffer\n",
				rimeaddr_node_addr.u8[0], rimeaddr_node_addr.u8[1]);
	}
	return 0;
}

/*---------------------------------------------------------------------------*/
void c_polite_recv(struct pipe *p, struct stackmodule_i *module) {
	PRINTF("c_polite_recv %s, hdrlen %d \n", (char *)packetbuf_dataptr(),
			p->channel->hdrsize);
	printaddr(module->stack_id);

	p->polite_param.hdrsize = p->channel->hdrsize;
	if (packetbuf_dataptr() == NULL)
		return;

	if (p->buf == NULL)
		return;
	if (packetbuf_datalen() != queuebuf_datalen(p->buf))
		return;
	if (memcmp(packetbuf_dataptr(), queuebuf_dataptr(p->buf),
			MIN(p->polite_param.hdrsize, packetbuf_datalen())) == 0) {
		/* We received a copy of our own packet, so we do not send out
		 packet. */
		queuebuf_free(p->buf);
		p->buf = NULL;
		ctimer_stop(&module->timer);
		PRINTF("~c_polite_recv: copy of our packet, cancelling \n");
		if (module->stack_id < STACKNO) {
			stack_dropped(&stack[module->stack_id]);
		}
	}
}

/*---------------------------------------------------------------------------*/
void c_polite_dropped(struct pipe *p, struct stackmodule_i *module) {
	PRINTF("polite dropped \n");
}

/*---------------------------------------------------------------------------*/
void c_polite_cancel(struct pipe *p, struct stackmodule_i *module) {
	ctimer_stop(&module->timer);
	if (p->buf != NULL) {
		queuebuf_free(p->buf);
		p->buf = NULL;
	}
}

/*---------------------------------------------------------------------------*/
/** @} */
/** @} */
