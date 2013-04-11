/**
 * \addtogroup crime
 * @{
 */

/**
 * \defgroup crimec_broadcast Composable Identified best-effort local area broadcast
 * @{
 *
 * The c_broadcast module sends packets to all local area neighbors with a
 * header that identifies the sender.
 *
 * The broadcast module sends a packet to all local neighbors.  The
 * module adds the single-hop sender address as a packet attribute to
 * outgoing packets.
 *
 * \section channels Channels
 *
 * The broadcast module uses 1 channel.
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
 *         Header file for the CRime module Composable Identified BroadCast (c_broadcast).
 *
 * \author
 *         Carolina Fortuna <carolina.fortuna@ijs.si>
 */


#ifndef __C_BROADCAST_H__
#define __C_BROADCAST_H__

#include "net/rime/rimeaddr.h"

/**
 * \brief      Set up an identified best-effort broadcast connection
 * \param p    A pointer to a pipe struct.
 * \param module Pointer to an abstract module struct.
 *
 *             This function sets up c_broadcast connection on the
 *             specified channel.
 */
void c_broadcast_open(struct pipe *p, struct stackmodule_i *module);

/**
 * \brief      Close identified best-effort broadcast connection
 * \param p    A pointer to a pipe struct.
 * \param module Pointer to an abstract module struct.
 *
 *             This function is called when a c_broadcast
 *             connection is not needed anymore or by an exit handler.
 */
void c_broadcast_close(struct pipe *p, struct stackmodule_i *module);

/**
 * \brief      Send a packet using identified best-effort broadcast connection
 * \param p    A pointer to a pipe struct.
 * \param module Pointer to an abstract module struct.
 *
 *             This function is called when a packet needs to be sent by
 *             c_broadcast.
 */
int c_broadcast_send(struct pipe *p, struct stackmodule_i *module);

/**
 * \brief      Receive a packet using identified best-effort broadcast connection
 * \param p    A pointer to a pipe struct.
 * \param module Pointer to an abstract module struct.
 *
 *             This function is called when a packet has to be sent by c_broadcast.
 */
void c_broadcast_recv(struct pipe *p, struct stackmodule_i *module);

/**
 * \brief      Notification for having sent a packet with identified best-effort broadcast connection
 * \param p    A pointer to a pipe struct.
 * \param module Pointer to an abstract module struct.
 *
 *             This function is called when a packet has been successfully sent by c_broadcast.
 */
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
