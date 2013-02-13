/**
 * \addtogroup rime
 * @{
 */

/**
 * \defgroup rimemultihop Best-effort multihop forwarding
 * @{
 *
 * The multihop module implements a multihop forwarding mechanism. Routes
 * must have already been setup with the route_add() function. Setting
 * up routes is done with another Rime module such as the \ref
 * routediscovery "route-discovery module".
 *
 * The multihop sends a packet to an identified node in the network by
 * using multi-hop forwarding at each node in the network.  The
 * application or protocol that uses the multihop primitive supplies a
 * routing function for selecting the next-hop neighbor.  If the
 * multihop primitive is requested to send a packet for which no
 * suitable next hop neighbor is found, the caller is immediately
 * notified of this and may choose to initiate a route discovery
 * process.
 *
 *
 * \section channels Channels
 *
 * The multihop module uses 1 channel.
 *
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
 * $Id: multihop.h,v 1.6 2009/03/24 07:15:04 adamdunkels Exp $
 */

/**
 * \file
 *         Multihop forwarding header file
 * \author
 *         Adam Dunkels <adam@sics.se>
 */

#ifndef __C_MULTIHOP_H__
#define __C_MULTIHOP_H__

#include "net/rime/rimeaddr.h"

void c_multihop_open(struct pipe *p, struct stackmodule_i *module);
void c_multihop_close(struct pipe *p, struct stackmodule_i *module);
int c_multihop_send(struct pipe *p, struct stackmodule_i *module);
void c_multihop_recv(struct pipe *p, struct stackmodule_i *module);
rimeaddr_t * c_multihop_forward(struct pipe *p, struct stackmodule_i *module);
void c_multihop_sent(struct pipe *p, struct stackmodule_i *module);

#endif /* __C_MULTIHOP_H__ */
/** @} */
/** @} */

//turtle crime:c_multihop rdf:type cpan:Module .
//turtle crime:c_multihop rdfs:comment The multihop module implements a multihop forwarding mechanism. .

//turtle crime:c_multihop_open rdf:type crime:c_open .
//turtle crime:c_multihop_close rdf:type crime:c_close .
//turtle crime:c_multihop_send rdf:type crime:c_send .
//turtle crime:c_multihop_sent rdf:type crime:c_sent .
//turtle crime:c_multihop_recv rdf:type crime:c_recv .
//turtle crime:c_multihop_forward rdf:type crime:c_forward .

//turtle crime:c_multihop_open crime:implements crime:top_interface .
//turtle crime:c_multihop_close crime:implements crime:top_interface .
//turtle crime:c_multihop_send crime:implements crime:top_interface .
//turtle crime:c_multihop_sent crime:implements crime:top_interface .
//turtle crime:c_multihop_recv crime:implements crime:top_interface .
//turtle crime:c_multihop_forward crime:implements crime:top_interface .

//turtle crime:c_multihop_open crime:implements crime:bottom_interface .
//turtle crime:c_multihop_close crime:implements crime:bottom_interface .
//turtle crime:c_multihop_send crime:implements crime:bottom_interface .
//turtle crime:c_multihop_sent crime:implements crime:bottom_interface .
//turtle crime:c_multihop_recv crime:implements crime:bottom_interface .

//turtle crime:c_multihop crime:hasScope crime:multihop .

//turtle crime:c_multihop crime:defines crime:c_multihop_open .
//turtle crime:c_multihop crime:defines crime:c_multihop_close .
//turtle crime:c_multihop crime:defines crime:c_multihop_send .
//turtle crime:c_multihop crime:defines crime:c_multihop_sent .
//turtle crime:c_multihop crime:defines crime:c_multihop_recv .
//turtle crime:c_multihop crime:defines crime:c_multihop_forward .

//turtle crime:channel_no rdf:type owls:Parameter .
//turtle crime:c_multihop crime:hasParameter crime:channel_no .
//turtle crime:attrlist rdf:type owls:Parameter .
//turtle crime:c_multihop crime:hasParameter crime:attrlist .
//turtle crime:esender rdf:type owls:Parameter .
//turtle crime:c_multihop crime:hasParameter crime:esender .
//turtle crime:esender crime:isUserSetBy crime:c_multihop .
//turtle crime:ereceiver rdf:type owls:Parameter .
//turtle crime:c_multihop crime:hasParameter crime:ereceiver .
//turtle crime:ereceiver crime:isUserSetBy crime:c_multihop .

//turtle crime:hop_no rdf:type owls:Parameter .
//turtle crime:c_multihop crime:hasParameter hop_no .
//turtle crime:seq_no rdf:type owls:Parameter .
//turtle crime:c_multihop crime:hasParameter seq_no .
//turtle crime:originator rdf:type owls:Parameter .
//turtle crime:c_multihop crime:hasParameter originator .

//turtle crime:time_trigger_flg rdf:type owls:Parameter .
//turtle crime:c_multihop crime:hasParameter crime:time_trigger_flg .
//turtle crime:time_trigger_flg crime:isUserSetByOptional crime:c_multihop .
//turtle crime:trigger_interval rdf:type owls:Parameter .
//turtle crime:c_multihop crime:hasParameter crime:trigger_interval .
//turtle crime:trigger_interval crime:isUserSetByOptional crime:c_multihop .
//turtle crime:trigger_no rdf:type owls:Parameter .
//turtle crime:c_multihop crime:hasParameter crime:trigger_no .
//turtle crime:trigger_no crime:isUserSetByOptional crime:c_multihop .
//turtle crime:trigger_th rdf:type owls:Parameter .
//turtle crime:c_multihop crime:hasParameter crime:trigger_th .
//turtle crime:trigger_th crime:isUserSetByOptional crime:c_multihop .

