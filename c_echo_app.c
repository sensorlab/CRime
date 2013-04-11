/**
 * \addtogroup crime
 * @{
 */

/**
 * \defgroup crimec_echo_app Application that prints information related to packets.
 * @{
 *
 * The c_echo_app prints informative messages such as sending or receiving packets.
 *
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
 *         File for the CRime module Composable Anonymous BroadCast (c_abc).
 *         This module is kept for compatibility (with Rime) reasons, having no
 *         practical value for CRime.
 * \author
 *         Carolina Fortuna <carolina.fortuna@ijs.si>
 */

#include "contiki.h"
#include "net/rime.h"
#include "net/rime/crime/c_echo_app.h"

#include <stddef.h>             /* For offsetof */

#define DEBUG 0
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

/*---------------------------------------------------------------------------*/
rimeaddr_t *
c_echo_app_forward(struct pipe *p, struct stackmodule_i *module)
{
  PRINTF("~c_echo_app_forward \n");
  return NULL;
}

/*---------------------------------------------------------------------------*/
void
c_echo_app_timedout(struct pipe *p, struct stackmodule_i *module)
{
  PRINTF("~c_echo_app_timedout\n");
}

/*---------------------------------------------------------------------------*/

void
c_echo_app_open(struct pipe *p, struct stackmodule_i *module)
{
  PRINTF("~c_echo_app_open\n");
}

/*---------------------------------------------------------------------------*/
void
c_echo_app_close(struct pipe *p, struct stackmodule_i *module)
{
}

/*---------------------------------------------------------------------------*/
int
c_echo_app_send(struct pipe *p, struct stackmodule_i *module)
{
  PRINTF("~c_echo_app_send\n");
  printf("----------- sending %s \n", (char *)packetbuf_dataptr());
  return 1;
}

/*---------------------------------------------------------------------------*/
void
c_echo_app_recv(struct pipe *p, struct stackmodule_i *module)
{
  PRINTF("c_echo_app_recv\n");
  printf("----------- received %s \n", (char *)packetbuf_dataptr());
}

void
c_echo_app_sent(struct pipe *p, struct stackmodule_i *module)
{
  PRINTF("~c_echo_app_sent\n");
}
