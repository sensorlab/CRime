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
 *         Header file for the CRime Echo App (c_echo_app) service.
 *
 * \author
 *         Carolina Fortuna <carolina.fortuna@ijs.si>
 */

#ifndef C_ECHO_APP_H_
#define C_ECHO_APP_H_

/**
 * \brief      Set up a connection
 * \param p    A pointer to a pipe struct.
 * \param module Pointer to an abstract module struct.
 *
 *             This function sets up a connection on the
 *             specified channel.
 */
void c_echo_app_open(struct pipe *p, struct stackmodule_i *module);

/**
 * \brief      Close a connection
 * \param p    A pointer to a pipe struct.
 * \param module Pointer to an abstract module struct.
 *
 *             This function closes a connection on the
 *             specified channel.
 */
void c_echo_app_close(struct pipe *p, struct stackmodule_i *module);

/**
 * \brief      Send a message.
 * \param p    A pointer to a pipe struct.
 * \param module Pointer to an abstract module struct.
 *
 *             This function requests the sending of a message.
 */
int c_echo_app_send(struct pipe *p, struct stackmodule_i *module);

/**
 * \brief      Receive a message.
 * \param p    A pointer to a pipe struct.
 * \param module Pointer to an abstract module struct.
 *
 *             This function handles the reception of a message.
 */
void c_echo_app_recv(struct pipe *p, struct stackmodule_i *module);

/**
 * \brief      Message sent notification function.
 * \param p    A pointer to a pipe struct.
 * \param module Pointer to an abstract module struct.
 *
 *             This function handles the notifications for a successfully
 *             sent message.
 */
void c_echo_app_sent(struct pipe *p, struct stackmodule_i *module);

/**
 * \brief      Forward a message.
 * \param p    A pointer to a pipe struct.
 * \param module Pointer to an abstract module struct.
 *
 *             This function handles forwarding messages.
 */
rimeaddr_t *c_echo_app_forward(struct pipe *p, struct stackmodule_i *module);

/**
 * \brief      Handle a message send timeout.
 * \param p    A pointer to a pipe struct.
 * \param module Pointer to an abstract module struct.
 *
 *             This function handles a timeout for an attempt to
 *             send a message.
 */
void c_echo_app_timedout(struct pipe *p, struct stackmodule_i *module);

#endif /* C_ECHO_APP_H_ */
/** @} */
/** @} */

//turtle crime:c_echo_app rdf:type cpan:Module .
//turtle crime:c_echo_app rdfs:comment  The echo app prints the content of the received packet. .

//turtle crime:c_echo_app_open rdf:type crime:c_open .
//turtle crime:c_echo_app_close rdf:type crime:c_close .
//turtle crime:c_echo_app_send rdf:type crime:c_send .
//turtle crime:c_echo_app_sent rdf:type crime:c_sent .
//turtle crime:c_echo_app_recv rdf:type crime:c_recv .
//turtle crime:c_echo_app_forward rdf:type crime:c_forward .
//turtle crime:c_echo_app_timedout rdf:type crime:c_timed_out .

//turtle crime:c_echo_app_open crime:implements crime:bottom_interface .
//turtle crime:c_echo_app_close crime:implements crime:bottom_interface .
//turtle crime:c_echo_app_send crime:implements crime:bottom_interface .
//turtle crime:c_echo_app_sent crime:implements crime:bottom_interface .
//turtle crime:c_echo_app_recv crime:implements crime:bottom_interface .
//turtle crime:c_echo_app_timedout crime:implements crime:bottom_interface .
//turtle crime:c_echo_app_forward crime:implements crime:bottom_interface .

//turtle crime:c_echo_app crime:hasScope crime:input .

//turtle crime:c_echo_app crime:defines crime:c_echo_app_open .
//turtle crime:c_echo_app crime:defines crime:c_echo_app_close .
//turtle crime:c_echo_app crime:defines crime:c_echo_app_send .
//turtle crime:c_echo_app crime:defines crime:c_echo_app_sent .
//turtle crime:c_echo_app crime:defines crime:c_echo_app_recv .
//turtle crime:c_echo_app crime:defines crime:c_echo_app_forward .
//turtle crime:c_echo_app crime:defines crime:c_echo_app_timedout .

//turtle crime:time_trigger_flg rdf:type owls:Parameter .
//turtle crime:c_echo_app crime:hasParameter crime:time_trigger_flg .
//turtle crime:time_trigger_flg crime:isUserSetByOptional crime:c_echo_app .
//turtle crime:trigger_interval rdf:type owls:Parameter .
//turtle crime:c_echo_app crime:hasParameter crime:trigger_interval .
//turtle crime:trigger_interval crime:isUserSetByOptional crime:c_echo_app .
//turtle crime:trigger_no rdf:type owls:Parameter .
//turtle crime:c_echo_app crime:hasParameter crime:trigger_no .
//turtle crime:trigger_no crime:isUserSetByOptional crime:c_echo_app .
//turtle crime:trigger_th rdf:type owls:Parameter .
//turtle crime:c_echo_app crime:hasParameter crime:trigger_th .
//turtle crime:trigger_th crime:isUserSetByOptional crime:c_echo_app .
