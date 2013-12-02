/**
 * \addtogroup rime
 * @{
 */
/**
 * \defgroup rimeroute Rime route table
 * @{
 *
 * The route module handles the route table in Rime.
 */

/*
 * Copyright (c) 2005, Swedish Institute of Computer Science.
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
 * $Id: route.h,v 1.12 2009/08/20 20:27:17 oliverschmidt Exp $
 */

/**
 * \file
 *         Header file for the Rime route table
 * \author
 *         Adam Dunkels <adam@sics.se>
 */

#ifndef __CROUTE_H__
#define __CROUTE_H__

#include "net/rime/rimeaddr.h"

struct c_route_entry {
  struct c_route_entry *next;
  rimeaddr_t dest;
  rimeaddr_t nexthop;
  uint8_t seqno;
  uint8_t cost;
  uint8_t lqi;
  uint8_t time;
  uint8_t decay;
  uint8_t time_last_decay;
};

void c_route_init(void);
void c_route_output(void);
//int c_route_add(const rimeaddr_t *dest, const rimeaddr_t *nexthop,
	//      uint8_t cost, uint8_t seqno);
int (*c_route_add) (const rimeaddr_t *dest, const rimeaddr_t *nexthop,
	      uint8_t cost, uint8_t seqno, uint8_t lqi);
//struct c_route_entry *c_route_lookup(const rimeaddr_t *dest);
struct c_route_entry* (*c_route_lookup) (const rimeaddr_t *dest);
void c_route_refresh(struct c_route_entry *e);
void c_route_decay(struct c_route_entry *e);
void c_route_remove(struct c_route_entry *e);
void c_route_flush_all(void);
void c_route_set_lifetime(int seconds);

int c_route_num(void);
struct c_route_entry *c_route_get(int num);

/*---------------------------------------------------*/
int c_route_hc_add(const rimeaddr_t *dest, const rimeaddr_t *nexthop,
		uint8_t cost, uint8_t seqno, uint8_t lqi);

struct c_route_entry *c_sp_lookup(const rimeaddr_t *dest);
struct c_route_entry *c_sprssi_lookup(const rimeaddr_t *dest);
struct c_route_entry *c_rnd_lookup(const rimeaddr_t *dest);

#endif /* __CROUTE_H__ */
/** @} */
/** @} */
