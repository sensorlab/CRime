/**
 * \addtogroup rime
 * @{
 */

/**
 * \defgroup rimeuc Single-hop unicast
 * @{
 *
 * The unicast module sends a packet to an identified single-hop
 * neighbor.  The unicast primitive uses the broadcast primitive and
 * adds the single-hop receiver address attribute to the outgoing
 * packets. For incoming packets, the unicast module inspects the
 * single-hop receiver address attribute and discards the packet if
 * the address does not match the address of the node.
 *
 * \section channels Channels
 *
 * The unicast module uses 1 channel.
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
 * $Id: unicast.h,v 1.6 2010/02/23 18:38:05 adamdunkels Exp $
 */

/**
 * \file
 *         Header file for Rime's single-hop unicast
 * \author
 *         Adam Dunkels <adam@sics.se>
 */

#ifndef __C_UNICAST_H__
#define __C_UNICAST_H__

//turtle crime:c_unicast rdf:type cpan:Module .
//turtle crime:c_unicast rdfs:comment The unicast module sends a packet to an identified single-hop neighbor. .

//turtle crime:c_unicast_open rdf:type crime:c_open .
//turtle crime:c_unicast_close rdf:type crime:c_close .
//turtle crime:c_unicast_send rdf:type crime:c_send .
//turtle crime:c_unicast_sent rdf:type crime:c_sent .
//turtle crime:c_unicast_recv rdf:type crime:c_recv .

//turtle crime:c_unicast_open crime:implements crime:top_interface .
//turtle crime:c_unicast_close crime:implements crime:top_interface .
//turtle crime:c_unicast_send crime:implements crime:top_interface .
//turtle crime:c_unicast_sent crime:implements crime:top_interface .
//turtle crime:c_unicast_recv crime:implements crime:top_interface .

//turtle crime:c_unicast_open crime:implements crime:bottom_interface .
//turtle crime:c_unicast_close crime:implements crime:bottom_interface .
//turtle crime:c_unicast_send crime:implements crime:bottom_interface .
//turtle crime:c_unicast_recv crime:implements crime:bottom_interface .
//turtle crime:c_unicast_sent crime:implements crime:bottom_interface .

//turtle crime:c_unicast crime:hasScope crime:singlehop .

//turtle crime:c_unicast crime:defines crime:c_unicast_open .
//turtle crime:c_unicast crime:defines crime:c_unicast_close .
//turtle crime:c_unicast crime:defines crime:c_unicast_send .
//turtle crime:c_unicast crime:defines crime:c_unicast_sent .
//turtle crime:c_unicast crime:defines crime:c_unicast_recv .

//turtle crime:channel_no rdf:type owls:Parameter .
//turtle crime:c_unicast crime:hasParameter crime:channel_no .
//turtle crime:attrlist rdf:type owls:Parameter .
//turtle crime:c_unicast crime:hasParameter crime:attrlist .
//turtle crime:num_tx rdf:type owls:Parameter .
//turtle crime:c_unicast crime:hasParameter crime:num_tx .
//turtle crime:sender rdf:type owls:Parameter .
//turtle crime:c_broadcast crime:hasParameter crime:sender .
//turtle crime:receiver rdf:type owls:Parameter .
//turtle crime:c_unicast crime:hasParameter crime:receiver .
//turtle crime:receiver crime:isUserSetBy crime:c_unicast .

//turtle crime:time_trigger_flg rdf:type owls:Parameter .
//turtle crime:c_unicast crime:hasParameter crime:time_trigger_flg .
//turtle crime:time_trigger_flg crime:isUserSetByOptional crime:c_unicast .
//turtle crime:trigger_interval rdf:type owls:Parameter .
//turtle crime:c_unicast crime:hasParameter crime:trigger_interval .
//turtle crime:trigger_interval crime:isUserSetByOptional crime:c_unicast .
//turtle crime:trigger_no rdf:type owls:Parameter .
//turtle crime:c_unicast crime:hasParameter crime:trigger_no .
//turtle crime:trigger_no crime:isUserSetByOptional crime:c_unicast .
//turtle crime:trigger_th rdf:type owls:Parameter .
//turtle crime:c_unicast crime:hasParameter crime:trigger_th .
//turtle crime:trigger_th crime:isUserSetByOptional crime:c_unicast .


void c_unicast_open(struct pipe *p, struct stackmodule_i *module);

void c_unicast_close(struct pipe *p, struct stackmodule_i *module);

int c_unicast_send(struct pipe *p, struct stackmodule_i *module);

void c_unicast_recv(struct pipe *p, struct stackmodule_i *module);

void c_unicast_sent(struct pipe *p, struct stackmodule_i *module);

#endif /* __C_UNICAST_H__ */
/** @} */
/** @} */
