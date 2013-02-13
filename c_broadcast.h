/**
 * \addtogroup rime
 * @{
 */

/**
 * \defgroup rimeibc Best-effort local area broadcast
 * @{
 *
 * The broadcast module sends packets to all local area neighbors with an a
 * header that identifies the sender.
 *
 * The broadcast module sends a packet to all local neighbors.  The
 * module adds the single-hop sender address as a packet attribute to
 * outgoing packets.  All Rime primitives that need the identity of
 * the sender in the outgoing packets use the broadcast primitive,
 * either directly or indirectly through any of the other
 * communication primitives that are based on the broadcast primitive.
 *
 * \section channels Channels
 *
 * The broadcast module uses 1 channel.
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
 * $Id: broadcast.h,v 1.5 2010/02/23 18:38:05 adamdunkels Exp $
 */

/**
 * \file
 *         Header file for identified best-effort local area broadcast
 * \author
 *         Adam Dunkels <adam@sics.se>
 */

#ifndef __C_BROADCAST_H__
#define __C_BROADCAST_H__

#include "net/rime/rimeaddr.h"

void c_broadcast_open(struct pipe *p, struct stackmodule_i *module);

/**
 * \brief      Close a broadcast connection
 * \param c    A pointer to a struct broadcast_conn
 *
 *             This function closes a broadcast connection that has
 *             previously been opened with broadcast_open().
 *
 *             This function typically is called as an exit handler.
 *
 */
void c_broadcast_close(struct pipe *p, struct stackmodule_i *module);

/**
 * \brief      Send an identified best-effort broadcast packet
 * \param c    The broadcast connection on which the packet should be sent
 * \retval     Non-zero if the packet could be sent, zero otherwise
 *
 *             This function sends an identified best-effort broadcast
 *             packet. The packet must be present in the packetbuf
 *             before this function is called.
 *
 *             The parameter c must point to a broadcast connection that
 *             must have previously been set up with broadcast_open().
 *
 */
int c_broadcast_send(struct pipe *p, struct stackmodule_i *module);
void c_broadcast_recv(struct pipe *p, struct stackmodule_i *module);
void c_broadcast_sent(struct pipe *p, struct stackmodule_i *module);

#endif /* __C_BROADCAST_H__ */
/** @} */
/** @} */


//turtle crime:c_broadcast rdf:type cpan:Module .
//turtle crime:c_broadcast rdfs:comment The broadcast module sends packets to all local area neighbors with an a header that identifies the sender. .

//turtle crime:c_broadcast_open rdf:type crime:c_open .
//turtle crime:c_broadcast_close rdf:type crime:c_close .
//turtle crime:c_broadcast_send rdf:type crime:c_send .
//turtle crime:c_broadcast_sent rdf:type crime:c_sent .
//turtle crime:c_broadcast_recv rdf:type crime:c_recv .

//turtle crime:c_broadcast_open crime:implements crime:top_interface .
//turtle crime:c_broadcast_close crime:implements crime:top_interface .
//turtle crime:c_broadcast_send crime:implements crime:top_interface .
//turtle crime:c_broadcast_sent crime:implements crime:top_interface .
//turtle crime:c_broadcast_recv crime:implements crime:top_interface .

//turtle crime:c_broadcast_open crime:implements crime:bottom_interface .
//turtle crime:c_broadcast_close crime:implements crime:bottom_interface .
//turtle crime:c_broadcast_send crime:implements crime:bottom_interface .
//turtle crime:c_broadcast_recv crime:implements crime:bottom_interface .
//turtle crime:c_broadcast_sent crime:implements crime:bottom_interface .

//turtle crime:c_broadcast crime:hasScope crime:singlehop .

//turtle crime:c_broadcast crime:defines crime:c_broadcast_open .
//turtle crime:c_broadcast crime:defines crime:c_broadcast_close .
//turtle crime:c_broadcast crime:defines crime:c_broadcast_send .
//turtle crime:c_broadcast crime:defines crime:c_broadcast_sent .
//turtle crime:c_broadcast crime:defines crime:c_broadcast_recv .

//turtle crime:channel_no rdf:type owls:Parameter .
//turtle crime:c_broadcast crime:hasParameter crime:channel_no .
//turtle crime:attrlist rdf:type owls:Parameter .
//turtle crime:c_broadcast crime:hasParameter crime:attrlist .
//turtle crime:num_tx rdf:type owls:Parameter .
//turtle crime:c_broadcast crime:hasParameter crime:num_tx .
//turtle crime:sender rdf:type owls:Parameter .
//turtle crime:c_broadcast crime:hasParameter crime:sender .
//turtle crime:sender crime:isUserSetBy crime:c_broadcast .

//turtle crime:time_trigger_flg rdf:type owls:Parameter .
//turtle crime:c_broadcast crime:hasParameter crime:time_trigger_flg .
//turtle crime:time_trigger_flg crime:isUserSetByOptional crime:c_broadcast .
//turtle crime:trigger_interval rdf:type owls:Parameter .
//turtle crime:c_broadcast crime:hasParameter crime:trigger_interval .
//turtle crime:trigger_interval crime:isUserSetByOptional crime:c_broadcast .
//turtle crime:trigger_no rdf:type owls:Parameter .
//turtle crime:c_broadcast crime:hasParameter crime:trigger_no .
//turtle crime:trigger_no crime:isUserSetByOptional crime:c_broadcast .
//turtle crime:trigger_th rdf:type owls:Parameter .
//turtle crime:c_broadcast crime:hasParameter crime:trigger_th .
//turtle crime:trigger_th crime:isUserSetByOptional crime:c_broadcast .
