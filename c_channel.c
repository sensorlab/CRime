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
 * $Id: channel.c,v 1.3 2010/06/15 19:22:25 adamdunkels Exp $
 */

/**
 * \file
 *         Rime's channel abstraction
 * \author
 *         Adam Dunkels <adam@sics.se>
 */

#include "net/rime/chameleon.h"
#include "net/rime/crime/amodule.h"
#include "net/rime.h"
#include "lib/list.h"

#include "vsntime.h"

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
void
c_channel_open(struct pipe *p, struct stackmodule_i *module)
{
  int startTm = vsnTime_freeRunTime();

  channel_open(p->channel, p->channel_no);
  PRINTF("~c_channel_open: chno %d %d \n", p->channel->channelno,
         p->channel_no);

  int stopTm = vsnTime_freeRunTimeDiff(startTm);

  printf("cho %d, ", stopTm);
}

/*---------------------------------------------------------------------------*/
void
c_channel_close(struct pipe *p, struct stackmodule_i *module)
{
  int startTm = vsnTime_freeRunTime();

  channel_close(p->channel);
  PRINTF("~c_channel_close \n");

  int stopTm = vsnTime_freeRunTimeDiff(startTm);

  printf("chc%d, ", stopTm);
}

/*---------------------------------------------------------------------------*/
void c_abc_input(struct pipe *p, struct stackmodule_i *module) {
	int start_tm = START_TM;
	PRINTF("~c_abc_input: packet '%s' on channel %d\n",
			(char *)packetbuf_dataptr(), p->channel->channelno);
	PRINTFE("\n %d \n", DURATION_TM(start_tm));
}
