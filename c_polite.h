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
 *         Header file for the Polite anonymous best effort local broadcast
 *
 * \author
 *         Carolina Fortuna <carolina.fortuna@ijs.si>
 */

#ifndef C_POLITE_H_
#define C_POLITE_H_

#include "net/queuebuf.h"
#include "sys/ctimer.h"


/**
 * \brief      Set up a polite connection
 * \param p    A pointer to a pipe struct.
 * \param module Pointer to an abstract module struct.
 *
 *             This function sets up c_polite connection on the
 *             specified channel.
 */
void c_polite_open(struct pipe *p, struct stackmodule_i *module);

/**
 * \brief      Close a polite connection
 * \param p    A pointer to a pipe struct.
 * \param module Pointer to an abstract module struct.
 *
 *             This function is called when a c_polite
 *             connection is not needed anymore or by an exit handler.
 */
void c_polite_close(struct pipe *p, struct stackmodule_i *module);

/**
 * \brief      Send a packet using a polite connection
 * \param p    A pointer to a pipe struct.
 * \param module Pointer to an abstract module struct.
 *
 *             This function is called when a packet needs to be sent by
 *             c_polite.
 *
 */
int c_polite_send(struct pipe *p, struct stackmodule_i *module);

/**
 * \brief      Receive a packet using a polite connection
 * \param p    A pointer to a pipe struct.
 * \param module Pointer to an abstract module struct.
 *
 *             This function is called when a packet is received by c_polite.
 */
void c_polite_recv(struct pipe *p, struct stackmodule_i *module);

/**
 * \brief      Notification for having sent a packet on the polite connection
 * \param p    A pointer to a pipe struct.
 * \param module Pointer to an abstract module struct.
 *
 *             This function is called when a packet has been
 *             successfully sent by c_polite.
 */
void c_polite_sent(struct pipe *p, struct stackmodule_i *module);

/**
 * \brief      Cancelling a packet to be sent by c_polite.
 * \param p    A pointer to a pipe struct.
 * \param module Pointer to an abstract module struct.
 *
 *             This function is called when a packet to be sent by
 *             c_polite has to be cancelled (probabbly because a similar
 *             packet has been received).
 */
void c_polite_cancel(struct pipe *p, struct stackmodule_i *module);

/**
 * \brief      Notification for dropping a packet to be sent by c_polite.
 * \param p    A pointer to a pipe struct.
 * \param module Pointer to an abstract module struct.
 *
 *             This function is called when a packet to be sent by
 *             c_polite was dropped.
 */
void c_polite_dropped(struct pipe *p, struct stackmodule_i *module);

#endif /* C_POLITE_H_ */
/** @} */
/** @} */

//turtle crime:c_polite rdf:type cpan:Module .
//turtle crime:c_polite rdfs:comment The polite module sends one local area broadcast packet within one time interval. If a packet with the same header is received from a neighbor within the interval, the packet is not sent. .

//turtle crime:c_polite_open rdf:type crime:c_open .
//turtle crime:c_polite_close rdf:type crime:c_close .
//turtle crime:c_polite_send rdf:type crime:c_send .
//turtle crime:c_polite_sent rdf:type crime:c_sent .
//turtle crime:c_polite_recv rdf:type crime:c_recv .
//turtle crime:c_polite_dropped rdf:type crime:c_dropped .

//turtle crime:c_polite_open crime:implements crime:top_interface .
//turtle crime:c_polite_close crime:implements crime:top_interface .
//turtle crime:c_polite_send crime:implements crime:top_interface .
//turtle crime:c_polite_sent crime:implements crime:top_interface .
//turtle crime:c_polite_recv crime:implements crime:top_interface .
//turtle crime:c_polite_dropped crime:implements crime:top_interface .

//turtle crime:c_polite_open crime:implements crime:bottom_interface .
//turtle crime:c_polite_close crime:implements crime:bottom_interface .
//turtle crime:c_polite_send crime:implements crime:bottom_interface .
//turtle crime:c_polite_recv crime:implements crime:bottom_interface .
//turtle crime:c_polite_sent crime:implements crime:bottom_interface .

//turtle crime:c_polite crime:hasScope crime:singlehop .

//turtle crime:c_polite crime:defines crime:c_polite_open .
//turtle crime:c_polite crime:defines crime:c_polite_close .
//turtle crime:c_polite crime:defines crime:c_polite_send .
//turtle crime:c_polite crime:defines crime:c_polite_sent .
//turtle crime:c_polite crime:defines crime:c_polite_recv .
//turtle crime:c_polite crime:defines crime:c_polite_dropped .

//turtle crime:channel_no rdf:type owls:Parameter .
//turtle crime:c_polite crime:hasParameter crime:channel_no .
//turtle crime:attrlist rdf:type owls:Parameter .
//turtle crime:c_polite crime:hasParameter crime:attrlist .
//turtle crime:num_tx rdf:type owls:Parameter .
//turtle crime:c_polite crime:hasParameter crime:num_tx .

//turtle crime:time_trigger_flg rdf:type owls:Parameter .
//turtle crime:c_polite crime:hasParameter crime:time_trigger_flg .
//turtle crime:time_trigger_flg crime:isUserSetByOptional crime:c_polite .
//turtle crime:trigger_interval rdf:type owls:Parameter .
//turtle crime:c_polite crime:hasParameter crime:trigger_interval .
//turtle crime:trigger_interval crime:isUserSetByOptional crime:c_polite .
//turtle crime:trigger_no rdf:type owls:Parameter .
//turtle crime:c_polite crime:hasParameter crime:trigger_no .
//turtle crime:trigger_no crime:isUserSetByOptional crime:c_polite .
//turtle crime:trigger_th rdf:type owls:Parameter .
//turtle crime:c_polite crime:hasParameter crime:trigger_th .
//turtle crime:trigger_th crime:isUserSetByOptional crime:c_polite .
