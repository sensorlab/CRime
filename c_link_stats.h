/*
 * c_link_stats.h
 *
 *  Created on: Feb 3, 2014
 *      Author: carolina
 */

#ifndef C_LINK_STATS_H_
#define C_LINK_STATS_H_

/**
 * \brief      Set up a snooping function - works best over a boadcast connection
 * \param p    A pointer to a pipe struct.
 * \param module Pointer to an abstract module struct.
 *
 *             This function sets up snooping functionality on the
 *             specified channel.
 */
void c_link_stats_open(struct pipe *p, struct stackmodule_i *module);

/**
 * \brief      Close the snooping functionality over the existing connection
 * \param p    A pointer to a pipe struct.
 * \param module Pointer to an abstract module struct.
 *
 *             This function is called when a snooping functionality is not needed anymore.
 */
void c_link_stats_close(struct pipe *p, struct stackmodule_i *module);

/**
 * \brief      Empty function, snooping does not send packets.
 * \param p    A pointer to a pipe struct.
 * \param module Pointer to an abstract module struct.
 *
 *
 */
int c_link_stats_send(struct pipe *p, struct stackmodule_i *module);

/**
 * \brief      A packet has been snooped, see if the neighbor list needs updating.
 * \param p    A pointer to a pipe struct.
 * \param module Pointer to an abstract module struct.
 *
 *             This function is called when a packet has to be sent by c_broadcast.
 */
void c_link_stats_recv(struct pipe *p, struct stackmodule_i *module);

/**
 * \brief      Empty function, snooping does not notify after sending
 * \param p    A pointer to a pipe struct.
 * \param module Pointer to an abstract module struct.
 *
 *             This function is called when a packet has been successfully sent by c_broadcast.
 */
void c_link_stats_sent(struct pipe *p, struct stackmodule_i *module);

#endif /* C_LINK_STATS_H_ */

//turtle crime:c_link_stats rdf:type cpan:Module .
//turtle crime:c_link_stats rdfs:comment The link_stats module performs statistics for hardware link estimators such as RSSI and LQI .

//turtle crime:c_link_stats_open rdf:type crime:c_open .
//turtle crime:c_link_stats_close rdf:type crime:c_close .
//turtle crime:c_link_stats_send rdf:type crime:c_send .
//turtle crime:c_link_stats_sent rdf:type crime:c_sent .
//turtle crime:c_link_stats_recv rdf:type crime:c_recv .

//turtle crime:c_link_stats_open crime:implements crime:top_interface .
//turtle crime:c_link_stats_close crime:implements crime:top_interface .
//turtle crime:c_link_stats_send crime:implements crime:top_interface .
//turtle crime:c_link_stats_sent crime:implements crime:top_interface .
//turtle crime:c_link_stats_recv crime:implements crime:top_interface .

//turtle crime:c_link_stats_open crime:implements crime:bottom_interface .
//turtle crime:c_link_stats_close crime:implements crime:bottom_interface .
//turtle crime:c_link_stats_send crime:implements crime:bottom_interface .
//turtle crime:c_link_stats_recv crime:implements crime:bottom_interface .
//turtle crime:c_link_stats_sent crime:implements crime:bottom_interface .

//turtle crime:c_link_stats crime:hasScope crime:singlehop .

//turtle crime:c_link_stats crime:defines crime:c_link_stats_open .
//turtle crime:c_link_stats crime:defines crime:c_link_stats_close .
//turtle crime:c_link_stats crime:defines crime:c_link_stats_send .
//turtle crime:c_link_stats crime:defines crime:c_link_stats_sent .
//turtle crime:c_link_stats crime:defines crime:c_link_stats_recv .

//turtle crime:channel_no rdf:type owls:Parameter .
//turtle crime:c_link_stats crime:hasParameter crime:maxrssi_pts .
//turtle crime:attrlist rdf:type owls:Parameter .
//turtle crime:c_link_stats crime:hasParameter crime:maxlqi_pts .

