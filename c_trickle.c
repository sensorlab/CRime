/**
 * \addtogroup trickle
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
 * $Id: trickle.c,v 1.18 2009/11/08 19:40:18 adamdunkels Exp $
 */

/**
 * \file
 *         Trickle (reliable single source flooding) for Rime
 * \author
 *         Adam Dunkels <adam@sics.se>
 */

#include "net/rime/crime/stack.h"
#include "net/rime/crime/amodule.h"
#include "net/rime/crime/c_trickle.h"
#include "lib/random.h"
#include "sys/ctimer.h"
#include "net/queuebuf.h"

#define INTERVAL_MIN 1
#define INTERVAL_MAX 4

#define DUPLICATE_THRESHOLD 1

#define SEQNO_LT(a, b) ((signed char)((a) - (b)) < 0)

#define DEBUG 1
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

//static struct trigger_param *param;
static int c_run_trickle(struct pipe *p, struct stackmodule_i *module);

/*---------------------------------------------------------------------------*/
static void
timer_callback(void *ptr)
{
  struct trigger_param *param = ptr;

  //c_run_trickle(param->pip, param->amodule);
}

/*---------------------------------------------------------------------------*/
static void
reset_interval(struct pipe *p, struct stackmodule_i *module)
{
  PT_INIT(&p->trickle_param.pt);
  c_run_trickle(p, module);
}

/*---------------------------------------------------------------------------*/
static void
set_timer(struct trigger_param *param, struct ctimer *t, clock_time_t i)
{
  ctimer_set(t, i, timer_callback, param);
}

/*---------------------------------------------------------------------------*/
static int
c_run_trickle(struct pipe *p, struct stackmodule_i *module)
{
  PRINTF("runtrickle\n");
  clock_time_t interval;

  PT_BEGIN(&p->trickle_param.pt);
  PRINTF("while\n");

  while(1) {
    interval = p->trickle_param.interval << p->trickle_param.interval_scaling;
    //param = (struct trigger_param*) malloc(sizeof(struct trigger_param));
    //param->pip = p;
    //param->amodule = module;
    PRINTF("before setting timers!\n");
    set_timer(param, &p->trickle_param.interval_timer, interval);
    PRINTF("timer1\n");
    set_timer(param, &p->trickle_param.timer,
              interval / 2 + (random_rand() % (interval / 2)));
    PRINTF("timer2\n");
    p->trickle_param.duplicate_no = 0;
    PT_YIELD(&p->trickle_param.pt);     /* Wait until listen timeout */
    PRINTF("sending\n");
    if(p->trickle_param.duplicate_no < DUPLICATE_THRESHOLD) {
      if(module->stack_id < STACKNO) {
        stack_send(&stack[module->stack_id], -1);
      }
    }
    PT_YIELD(&p->trickle_param.pt);     /* Wait until interval timer expired. */
    if(p->trickle_param.interval_scaling < INTERVAL_MAX) {
      p->trickle_param.interval_scaling++;
    }
  }

  PT_END(&p->trickle_param.pt);
}

/*---------------------------------------------------------------------------*/
void
c_trickle_open(struct pipe *p, struct stackmodule_i *module)
{
  p->buf = NULL;
}

/*---------------------------------------------------------------------------*/
void
c_trickle_close(struct pipe *p, struct stackmodule_i *module)
{
  ctimer_stop(&p->trickle_param.timer);
  ctimer_stop(&p->trickle_param.interval_timer);
}

/*---------------------------------------------------------------------------*/
void
c_trickle_recv(struct pipe *p, struct stackmodule_i *module)
{
  uint16_t seq_no = packetbuf_attr(PACKETBUF_ATTR_EPACKET_ID);

  /*PRINTF("%d.%d: trickle recv seqno %d from %d.%d our %d data len %d channel %d\n",
     rimeaddr_node_addr.u8[0], rimeaddr_node_addr.u8[1],
     seq_no,
     p->sender.u8[0], p->sender.u8[1],
     p->trickle_param.seq_no,
     packetbuf_datalen(),
     packetbuf_attr(PACKETBUF_ATTR_CHANNEL)); */

  module->time_trigger_flg = 0;
  if(seq_no == p->trickle_param.seq_no) {
    /*    c->cb->recv(c); */
    ++p->trickle_param.duplicate_no;
    PRINTF("if1\n");
  } else if(SEQNO_LT(seq_no, p->trickle_param.seq_no)) {
    p->trickle_param.interval_scaling = 0;
    if(module->stack_id < STACKNO) {
      stack_send(&stack[module->stack_id], -1);
    }
  } else {                      /* hdr->seqno > c->seqno */
    PRINTF("if3\n");
    p->trickle_param.seq_no = seq_no;
    /* Store the incoming data in the queuebuf */
    if(p->buf != NULL) {
      queuebuf_free(p->buf);
    }
    p->buf = queuebuf_new_from_packetbuf();
    p->trickle_param.interval_scaling = 0;
    reset_interval(p, module);
    module->time_trigger_flg = 1;
    module->trigger_init_flg = 1;
    module->trigger_th = 1;
    module->trigger_no = 1;
    module->timer = p->trickle_param.first_transmission_timer;
    p->trickle_param.interval_scaling =
      random_rand() % p->trickle_param.interval_scaling;

    //param = (struct trigger_param*) malloc(sizeof(struct trigger_param));
    //param->pip = p;
    //param->amodule = stack[module->stack_id].amodule;
    //param->modidx = stack[module->stack_id].modno-1;
    //param->rxmittime = module->trigger_interval;

    ctimer_set(&p->trickle_param.timer,
               random_rand() % p->trickle_param.interval_scaling,
               c_triggered_send, param);
    PRINTF("scheduled triggered send");
  }
}

/*---------------------------------------------------------------------------*/
int
c_trickle_send(struct pipe *p, struct stackmodule_i *module)
{
  PRINTF("c_trickle_send");
  if(p->buf != NULL) {
    queuebuf_free(p->buf);
  }
  p->trickle_param.seq_no++;
  packetbuf_set_attr(PACKETBUF_ATTR_EPACKET_ID, p->trickle_param.seq_no);
  p->buf = queuebuf_new_from_packetbuf();
  PRINTF("%d.%d: trickle send seqno %d\n",
         rimeaddr_node_addr.u8[0], rimeaddr_node_addr.u8[1],
         p->trickle_param.seq_no);
  reset_interval(p, module);
  PRINTF("interval reset\n");

  if(p->buf != NULL) {
    queuebuf_to_packetbuf(p->buf);
    PRINTF("~trickle_send\n");
    return 1;
  } else {
    PRINTF("%d.%d: trickle send but c->q == NULL\n",
           rimeaddr_node_addr.u8[0], rimeaddr_node_addr.u8[1]);
    return 0;
  }
}

/*---------------------------------------------------------------------------*/
/** @} */
