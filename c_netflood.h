/**
 * \addtogroup crime
 * @{
 */

/**
 * \defgroup crimec_netflood Composable best-effort network flooding
 * @{
 *
  * The netflood module does best-effort flooding.
 *
 * The c_netflood primitive sends a single packet to all nodes in the
 * network. The netflood primitive may use c_polite broadcasts at every hop
 * to reduce the number of redundant transmissions.  The netflood
 * primitive does not perform retransmissions of flooded packets and
 * packets are not tagged with version numbers.  Instead, the netflood
 * primitive sets the end-to-end sender and end-to-end packet ID
 * attributes on the packets it sends.  A forwarding node saves the
 * end-to-end sender and packet ID of the last packet it forwards and
 * does not forward a packet if it has the same end-to-end sender and
 * packet ID as the last packet.  This reduces the risk of routing
 * loops, but does not eliminate them entirely as the netflood
 * primitive saves the attributes of the latest packet seen only.
 * Therefore, the netflood primitive also uses the time to live
 * attribute, which is decreased by one before forwarding a packet.
 * If the time to live reaches zero, the primitive does not forward
 * the packet.
*
 * \section channels Channels
 *
 * The netflood module uses 1 channel.
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
 *         Header file for the CRime module Composeable Mesh routing protocol.
 *
 * \author
 *         Carolina Fortuna <carolina.fortuna@ijs.si>
 */

#ifndef __C_NETFLOOD_H__
#define __C_NETFLOOD_H__

#include "net/queuebuf.h"
#include "net/rime/crime/amodule.h"
#include "net/rime/crime/stack.h"

/**
 * \brief      Set up the connections required for the netflood service.
 * \param p    A pointer to a pipe struct.
 * \param module Pointer to an abstract module struct.
 *
 *             This function sets up c_netflood connection.
 *
 */
void c_netflood_open(struct pipe *p, struct stackmodule_i *module);

/**
 * \brief      Close the connections required for the netflood service.
 * \param p    A pointer to a pipe struct.
 * \param module Pointer to an abstract module struct.
 *
 *             This function is called when a c_netflood connection is
 *             not needed anymore or by an exit handler.
 */
void c_netflood_close(struct pipe *p, struct stackmodule_i *module);

/**
 * \brief      Send a packet using the netflood primitive.
 * \param p    A pointer to a pipe struct.
 * \param module Pointer to an abstract module struct.
 *
 *             This function is called when a packet has to be sent by c_netflood.
 */
int c_netflood_send(struct pipe *p, struct stackmodule_i *module);

/**
 * \brief      Receive a packet using the netflood primitive.
 * \param p    A pointer to a pipe struct.
 * \param module Pointer to an abstract module struct.
 *
 *             This function is called when a packet has been received by
 *             c_netflood.
 *
 */
void c_netflood_recv(struct pipe *p, struct stackmodule_i *module);

/**
 * \brief      Notification for having sent a packet with the netflood primitive
 * \param p    A pointer to a pipe struct.
 * \param module Pointer to an abstract module struct.
 *
 *             This function is called when a packet has been successfully sent by c_netflood.
 */
void c_netflood_sent(struct pipe *p, struct stackmodule_i *module);

/**
 * \brief      Notification that the netflood primitive dropped a packet
 * \param p    A pointer to a pipe struct.
 * \param module Pointer to an abstract module struct.
 *
 *             This function is called when a packet has been dropped c_netflood.
 */
void c_netflood_dropped(struct pipe *p, struct stackmodule_i *module);

/**
 * \brief      Notification that the netflood primitive canceled the transmission of a packet
 * \param p    A pointer to a pipe struct.
 * \param module Pointer to an abstract module struct.
 *
 *             This function is called when a packet transmission has been
 *             canceled by c_netflood.
 */
void c_netflood_cancel(struct pipe *p, struct stackmodule_i *module);

#endif /* __C_NETFLOOD_H__ */
/** @} */
/** @} */

//turtle crime:c_netflood rdf:type cpan:Module .
//turtle crime:c_netflood rdfs:comment The netflood module does best-effort flooding. .

//turtle crime:c_netflood_open rdf:type crime:c_open .
//turtle crime:c_netflood_close rdf:type crime:c_close .
//turtle crime:c_netflood_send rdf:type crime:c_send .
//turtle crime:c_netflood_sent rdf:type crime:c_sent .
//turtle crime:c_netflood_recv rdf:type crime:c_recv .
//turtle crime:c_netflood_dropped rdf:type crime:c_dropped .
//turtle crime:c_netflood_canceled rdf:type crime:c_dropped .

//turtle crime:c_netflood_open crime:implements crime:top_interface .
//turtle crime:c_netflood_close crime:implements crime:top_interface .
//turtle crime:c_netflood_send crime:implements crime:top_interface .
//turtle crime:c_netflood_sent crime:implements crime:top_interface .
//turtle crime:c_netflood_recv crime:implements crime:top_interface .
//turtle crime:c_netflood_dropped crime:implements crime:top_interface .

//turtle crime:c_netflood_open crime:implements crime:bottom_interface .
//turtle crime:c_netflood_close crime:implements crime:bottom_interface .
//turtle crime:c_netflood_send crime:implements crime:bottom_interface .
//turtle crime:c_netflood_sent crime:implements crime:bottom_interface .
//turtle crime:c_netflood_recv crime:implements crime:bottom_interface .
//turtle crime:c_netflood_dropped crime:implements crime:bottom_interface .

//turtle crime:c_netflood crime:hasScope crime:multihop .

//turtle crime:c_netflood crime:defines crime:c_netflood_open .
//turtle crime:c_netflood crime:defines crime:c_netflood_close .
//turtle crime:c_netflood crime:defines crime:c_netflood_send .
//turtle crime:c_netflood crime:defines crime:c_netflood_sent .
//turtle crime:c_netflood crime:defines crime:c_netflood_recv .
//turtle crime:c_netflood crime:defines crime:c_netflood_dropped .
//turtle crime:c_netflood crime:defines crime:c_netflood_cancelled .

//turtle crime:channel_no rdf:type owls:Parameter .
//turtle crime:c_netflood crime:hasParameter crime:channel_no .
//turtle crime:attrlist rdf:type owls:Parameter .
//turtle crime:c_netflood crime:hasParameter crime:attrlist .

//turtle crime:hop_no rdf:type owls:Parameter .
//turtle crime:c_netflood crime:hasParameter hop_no .
//turtle crime:seq_no rdf:type owls:Parameter .
//turtle crime:c_netflood crime:hasParameter seq_no .
//turtle crime:originator rdf:type owls:Parameter .
//turtle crime:c_netflood crime:hasParameter originator .

//turtle crime:time_trigger_flg rdf:type owls:Parameter .
//turtle crime:c_netflood crime:hasParameter crime:time_trigger_flg .
//turtle crime:time_trigger_flg crime:isUserSetByOptional crime:c_netflood .
//turtle crime:trigger_interval rdf:type owls:Parameter .
//turtle crime:c_netflood crime:hasParameter crime:trigger_interval .
//turtle crime:trigger_interval crime:isUserSetByOptional crime:c_netflood .
//turtle crime:trigger_no rdf:type owls:Parameter .
//turtle crime:c_netflood crime:hasParameter crime:trigger_no .
//turtle crime:trigger_no crime:isUserSetByOptional crime:c_netflood .
//turtle crime:trigger_th rdf:type owls:Parameter .
//turtle crime:c_netflood crime:hasParameter crime:trigger_th .
//turtle crime:trigger_th crime:isUserSetByOptional crime:c_netflood .
