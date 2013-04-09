/**
 * \addtogroup rime
 * @{
 */

/**
 * \defgroup routediscovery Rime route discovery protocol
 * @{
 *
 * The route-discovery module does route discovery for Rime.
 *
 * \section channels Channels
 *
 * The ibc module uses 2 channels; one for the flooded route request
 * packets and one for the unicast route replies.
 *
 */
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
 * $Id: route-discovery.h,v 1.11 2010/06/18 08:28:56 nifi Exp $
 */

/**
 * \file
 *         Header file for the Rime mesh routing protocol
 * \author
 *         Adam Dunkels <adam@sics.se>
 */

#ifndef __C_ROUTE_DISCOVERY_H__
#define __C_ROUTE_DISCOVERY_H__

//turtle crime:c_route_discovery rdf:type cpan:Module .
//turtle crime:c_route_discovery rdfs:comment The route-discovery module does route discovery for Rime. It uses 2 channels; one for the flooded route request packets and one for the unicast route replies. .

//turtle crime:c_route_discovery_open rdf:type crime:c_open .
//turtle crime:c_route_discovery_close rdf:type crime:c_close .
//turtle crime:c_route_discovery_discover rdf:type crime:c_send .
//turtle crime:c_route_discovery_recv rdf:type crime:c_recv .
//turtle crime:c_route_discovery_timedout rdf:type crime:c_timed_out .

//turtle crime:c_route_discovery_open crime:implements crime:top_interface .
//turtle crime:c_route_discovery_close crime:implements crime:top_interface .
//turtle crime:c_route_discovery_discover crime:implements crime:top_interface .
//turtle crime:c_route_discovery_recv crime:implements crime:top_interface .
//turtle crime:c_route_discovery_timedout crime:implements crime:top_interface .

//turtle crime:c_route_discovery_open crime:implements crime:bottom_interface .
//turtle crime:c_route_discovery_close crime:implements crime:bottom_interface .
//turtle crime:c_route_discovery_send crime:implements crime:bottom_interface .
//turtle crime:c_route_discovery_recv crime:implements crime:bottom_interface .
//turtle crime:c_route_discovery_timedout crime:implements crime:bottom_interface .

//turtle crime:c_route_discovery crime:hasScope crime:multihop .

//turtle crime:c_route_discovery crime:defines crime:c_route_discovery_open .
//turtle crime:c_route_discovery crime:defines crime:c_route_discovery_close .
//turtle crime:c_route_discovery crime:defines crime:c_route_discovery_discover .
//turtle crime:c_route_discovery crime:defines crime:c_route_discovery_sent .
//turtle crime:c_route_discovery crime:defines crime:c_route_discovery_recv .
//turtle crime:c_route_discovery crime:defines crime:c_route_discovery_timedout .

//turtle crime:channel_no rdf:type owls:Parameter .
//turtle crime:c_route_discovery crime:hasParameter crime:channel_no .
//turtle crime:attrlist rdf:type owls:Parameter .
//turtle crime:c_route_discovery crime:hasParameter crime:attrlist .
//turtle crime:num_tx rdf:type owls:Parameter .
//turtle crime:c_route_discovery crime:hasParameter crime:num_tx .

//turtle crime:time_trigger_flg rdf:type owls:Parameter .
//turtle crime:c_route_discovery crime:hasParameter crime:time_trigger_flg .
//turtle crime:time_trigger_flg crime:isUserSetByOptional crime:c_route_discovery .
//turtle crime:trigger_interval rdf:type owls:Parameter .
//turtle crime:c_route_discovery crime:hasParameter crime:trigger_interval .
//turtle crime:trigger_interval crime:isUserSetByOptional crime:c_route_discovery .
//turtle crime:trigger_no rdf:type owls:Parameter .
//turtle crime:c_route_discovery crime:hasParameter crime:trigger_no .
//turtle crime:trigger_no crime:isUserSetByOptional crime:c_route_discovery .
//turtle crime:trigger_th rdf:type owls:Parameter .
//turtle crime:c_route_discovery crime:hasParameter crime:trigger_th .
//turtle crime:trigger_th crime:isUserSetByOptional crime:c_route_discovery .



#include "net/rime/unicast.h"
#include "net/rime/netflood.h"
#include "sys/ctimer.h"

#define ROUTE_DISCOVERY_ENTRIES 8

void c_route_discovery_open(struct pipe *p, struct stackmodule_i *module);

int c_route_discovery_discover(struct pipe *p, struct stackmodule_i *module);

void c_route_discovery_close(struct pipe *p, struct stackmodule_i *module);

void c_route_discovery_recv(struct pipe *p, struct stackmodule_i *module);

void c_route_discovery_timedout(struct pipe *p, struct stackmodule_i *module);

#endif /* __C_ROUTE_DISCOVERY_H__ */
/** @} */
/** @} */
