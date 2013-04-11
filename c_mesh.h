/**
 * \addtogroup crime
 * @{
 */

/**
 * \defgroup crimec_mesh Composable Mesh routing
 * @{
 *
 * The c_mesh module sends packets using multi-hop routing to a specified
 * receiver somewhere in the network.
 *
 * \section channels Channels
 *
  * The mesh module uses 3 channel; one for the multi-hop forwarding
 * (\ref crimec_multihop "c_multihop") and two for the route disovery (\ref
 * c_route_discovery "route-discovery").
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


#ifndef __C_MESH_H__
#define __C_MESH_H__

#include "net/queuebuf.h"
#include "net/rime/crime/c_multihop.h"
#include "net/rime/crime/c_route_discovery.h"

/**
 * \brief      Set up the connections required for the mesh routing service.
 * \param p    A pointer to a pipe struct.
 * \param module Pointer to an abstract module struct.
 *
 *             This function sets up c_mesh connections on three different
 *             specified channels.
 */
void c_mesh_open(struct pipe *p, struct stackmodule_i *module);

/**
 * \brief      Close the connections required for the mesh routing service.
 * \param p    A pointer to a pipe struct.
 * \param module Pointer to an abstract module struct.
 *
 *             This function is called when a c_mesh connections are
 *             not needed anymore or by an exit handler.
 */
void c_mesh_close(struct pipe *p, struct stackmodule_i *module);

/**
 * \brief      Send a packet using the mesh routing protocol.
 * \param p    A pointer to a pipe struct.
 * \param module Pointer to an abstract module struct.
 *
 *             This function is called when a packet needs to be sent by
 *             c_mesh.
 *
 */
int c_mesh_send(struct pipe *p, struct stackmodule_i *module);

/**
 * \brief      Receive a packet using the mesh routing protocol.
 * \param p    A pointer to a pipe struct.
 * \param module Pointer to an abstract module struct.
 *
 *             This function is called when a packet is received by c_mesh.
 */
void c_mesh_recv(struct pipe *p, struct stackmodule_i *module);

/**
 * \brief      Notification for having sent a packet with the mesh routing protocol
 * \param p    A pointer to a pipe struct.
 * \param module Pointer to an abstract module struct.
 *
 *             This function is called when a packet has been successfully sent by c_mesh.
 */
void c_mesh_sent(struct pipe *p, struct stackmodule_i *module);

/**
 * \brief      This function is called when a packet needs to be forwarded.
 * \param p    A pointer to a pipe struct.
 * \param module Pointer to an abstract module struct.
 *
 *             This function handles forwarding packets for the c_mesh routing protocol.
 */
rimeaddr_t *c_mesh_forward(struct pipe *p, struct stackmodule_i *module);

/**
 * \brief      This function handles a message send timeout for the mesh routing protocol.
 * \param p    A pointer to a pipe struct.
 * \param module Pointer to an abstract module struct.
 *
 *             This function handles a timeout for an attempt to
 *             send a packet using c_mesh.
 */
void c_mesh_timedout(struct pipe *p, struct stackmodule_i *module);

#endif /* __C_MESH_H__ */
/** @} */
/** @} */

//turtle crime:c_mesh rdf:type cpan:Module .
//turtle crime:c_mesh rdfs:comment  The mesh module sends packets using multi-hop routing to a specified receiver somewhere in the network. The mesh module uses 3 channel; one for the multi-hop forwarding and two for the route disovery. .

//turtle crime:c_mesh_open rdf:type crime:c_open .
//turtle crime:c_mesh_close rdf:type crime:c_close .
//turtle crime:c_mesh_send rdf:type crime:c_send .
//turtle crime:c_mesh_sent rdf:type crime:c_sent .
//turtle crime:c_mesh_recv rdf:type crime:c_recv .
//turtle crime:c_mesh_forward rdf:type crime:c_forward .
//turtle crime:c_mesh_timedout rdf:type crime:c_timed_out .

//turtle crime:c_mesh_open crime:implements crime:top_interface .
//turtle crime:c_mesh_close crime:implements crime:top_interface .
//turtle crime:c_mesh_send crime:implements crime:top_interface .
//turtle crime:c_mesh_sent crime:implements crime:top_interface .
//turtle crime:c_mesh_recv crime:implements crime:top_interface .
//turtle crime:c_mesh_forward crime:implements crime:top_interface .

//turtle crime:c_mesh_open crime:implements crime:bottom_interface .
//turtle crime:c_mesh_close crime:implements crime:bottom_interface .
//turtle crime:c_mesh_send crime:implements crime:bottom_interface .
//turtle crime:c_mesh_recv crime:implements crime:bottom_interface .
//turtle crime:c_mesh_timedout crime:implements crime:bottom_interface .

//turtle crime:c_mesh crime:hasScope crime:multihop .

//turtle crime:c_mesh crime:defines crime:c_mesh_open .
//turtle crime:c_mesh crime:defines crime:c_mesh_close .
//turtle crime:c_mesh crime:defines crime:c_mesh_send .
//turtle crime:c_mesh crime:defines crime:c_mesh_sent .
//turtle crime:c_mesh crime:defines crime:c_mesh_recv .
//turtle crime:c_mesh crime:defines crime:c_mesh_forward .
//turtle crime:c_mesh crime:defines crime:c_mesh_timedout .

//turtle crime:channel_no rdf:type owls:Parameter .
//turtle crime:c_mesh crime:hasParameter crime:channel_no .
//turtle crime:attrlist rdf:type owls:Parameter .
//turtle crime:c_mesh crime:hasParameter crime:attrlist .

//turtle crime:hop_no rdf:type owls:Parameter .
//turtle crime:c_mesh crime:hasParameter hop_no .
//turtle crime:seq_no rdf:type owls:Parameter .
//turtle crime:c_mesh crime:hasParameter seq_no .
//turtle crime:originator rdf:type owls:Parameter .
//turtle crime:c_mesh crime:hasParameter originator .

//turtle crime:time_trigger_flg rdf:type owls:Parameter .
//turtle crime:c_mesh crime:hasParameter crime:time_trigger_flg .
//turtle crime:time_trigger_flg crime:isUserSetByOptional crime:c_mesh .
//turtle crime:trigger_interval rdf:type owls:Parameter .
//turtle crime:c_mesh crime:hasParameter crime:trigger_interval .
//turtle crime:trigger_interval crime:isUserSetByOptional crime:c_mesh .
//turtle crime:trigger_no rdf:type owls:Parameter .
//turtle crime:c_mesh crime:hasParameter crime:trigger_no .
//turtle crime:trigger_no crime:isUserSetByOptional crime:c_mesh .
//turtle crime:trigger_th rdf:type owls:Parameter .
//turtle crime:c_mesh crime:hasParameter crime:trigger_th .
//turtle crime:trigger_th crime:isUserSetByOptional crime:c_mesh .
