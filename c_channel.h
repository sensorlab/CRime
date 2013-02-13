/*
 * Copyright (c) 2007, Swedish Institute of Computer Science.
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
 * $Id: channel.h,v 1.3 2010/06/14 19:19:17 adamdunkels Exp $
 */

/**
 * \file
 *         Header file for Rime's channel abstraction
 * \author
 *         Adam Dunkels <adam@sics.se>
 */

#ifndef __C_CHANNEL_H__
#define __C_CHANNEL_H__

//turtle crime:c_channel rdf:type cpan:Module .
//turtle crime:c_channel rdfs:comment The channel module is the basic module of any stack and ensures the proper opening and closing of a communication channel. The value of the communication channel must be provided by the user as input. .

//turtle crime:c_channel_open rdf:type crime:c_open .
//turtle crime:c_channel_close rdf:type crime:c_close .
//turtle crime:c_abc_input rdf:type crime:c_recv .
//turtle crime:c_rime_output rdf:type crime:c_send .

//turtle crime:c_channel_open crime:implements crime:top_interface .
//turtle crime:c_channel_close crime:implements crime:top_interface .
//turtle crime:c_abc_input crime:implements crime:bottom_interface .
//turtle crime:c_rime_output crime:implements crime:top_interface .

//turtle crime:c_channel crime:hasScope crime:singlehop .

//turtle crime:c_channel crime:defines crime:c_channel_open .
//turtle crime:c_channel crime:defines crime:c_channel_close .
//turtle crime:c_channel crime:defines crime:c_abc_input .
//turtle crime:c_channel crime:defines crime:c_rime_output .

//turtle crime:channel_no rdf:type owls:Parameter .
//turtle crime:c_channel crime:hasParameter crime:channel_no .
//turtle crime:channel_no crime:isUserSetBy crime:c_channel .

#include "contiki-conf.h"
#include "net/packetbuf.h"
#include "net/rime/chameleon.h"
#include "net/rime/crime/amodule.h"

void c_channel_open(struct pipe *p, struct stackmodule_i *module);
void c_channel_close(struct pipe *p, struct stackmodule_i *module);

#endif /* __C_CHANNEL_H__ */
