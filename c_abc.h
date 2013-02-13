/**
 * \addtogroup crime
 * @{
 */

/**
 * \defgroup crime c_abc Anonymous best-effort local area broadcast
 * @{
 *
 * The c_abc module sends packets to all local area neighbors. The abc
 * module adds no headers to outgoing packets.
 *
 * \section channels Channels
 *
 * The c_abc module uses 1 channel.
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
 *         Header file for the CRime module Composable Anonymous BroadCast (c_abc)
 * \author
 *         Carolina Fortuna <carolina.fortuna@ijs.si>
 */

#ifndef __C_ABC_H__
#define __C_ABC_H__


void c_abc_open(struct pipe *p, struct stackmodule_i *module);
void c_abc_close(struct pipe *p, struct stackmodule_i *module);
int c_abc_send(struct pipe *p, struct stackmodule_i *module);
void c_abc_input(struct pipe *p, struct stackmodule_i *module);
void c_abc_sent(struct pipe *p, struct stackmodule_i *module);
void c_abc_recv(struct pipe *p, struct stackmodule_i *module);

void c_start_tm();
int c_get_start_tm();


#endif /* __C_ABC_H__ */
/** @} */
/** @} */

//turtle crime:c_abc rdf:type cpan:Module .
//turtle crime:c_abc rdfs:comment The abc module is designed to send annonymous broadcast messages on the selected channel. It uses three parameters chammelno, attrlist and num_tx. Channel_no configures the channel, attrlist determines the header of the packet while num_tx stores the number of attempted transmissions. All parameters are set in other modules. .

//turtle crime:c_abc_open rdf:type crime:c_open .
//turtle crime:c_abc_close rdf:type crime:c_close .
//turtle crime:c_abc_send rdf:type crime:c_send .
//turtle crime:c_abc_sent rdf:type crime:c_sent .
//turtle crime:c_abc_recv rdf:type crime:c_recv .

//turtle crime:c_abc_open crime:implements crime:top_interface .
//turtle crime:c_abc_close crime:implements crime:top_interface .
//turtle crime:c_abc_send crime:implements crime:top_interface .
//turtle crime:c_abc_sent crime:implements crime:top_interface .
//turtle crime:c_abc_recv crime:implements crime:top_interface .

//turtle crime:c_abc_open crime:implements crime:bottom_interface .
//turtle crime:c_abc_close crime:implements crime:bottom_interface .
//turtle crime:c_abc_send crime:implements crime:bottom_interface .
//turtle crime:c_abc_recv crime:implements crime:bottom_interface .

//turtle crime:c_abc crime:hasScope crime:singlehop .

//turtle crime:c_abc crime:defines crime:c_abc_open .
//turtle crime:c_abc crime:defines crime:c_abc_close .
//turtle crime:c_abc crime:defines crime:c_abc_send .
//turtle crime:c_abc crime:defines crime:c_abc_sent .
//turtle crime:c_abc crime:defines crime:c_abc_recv .

//turtle crime:channel_no rdf:type owls:Parameter .
//turtle crime:c_abc crime:hasParameter crime:channel_no .
//turtle crime:attrlist rdf:type owls:Parameter .
//turtle crime:c_abc crime:hasParameter crime:attrlist .
//turtle crime:num_tx rdf:type owls:Parameter .
//turtle crime:c_abc crime:hasParameter crime:num_tx .

//turtle crime:time_trigger_flg rdf:type owls:Parameter .
//turtle crime:c_abc crime:hasParameter crime:time_trigger_flg .
//turtle crime:time_trigger_flg crime:isUserSetByOptional crime:c_abc .
//turtle crime:trigger_interval rdf:type owls:Parameter .
//turtle crime:c_abc crime:hasParameter crime:trigger_interval .
//turtle crime:trigger_interval crime:isUserSetByOptional crime:c_abc .
//turtle crime:trigger_no rdf:type owls:Parameter .
//turtle crime:c_abc crime:hasParameter crime:trigger_no .
//turtle crime:trigger_no crime:isUserSetByOptional crime:c_abc .
//turtle crime:trigger_th rdf:type owls:Parameter .
//turtle crime:c_abc crime:hasParameter crime:trigger_th .
//turtle crime:trigger_th crime:isUserSetByOptional crime:c_abc .
