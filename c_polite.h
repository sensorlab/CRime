/*
 * c-polite.h
 *
 *  Created on: Nov 22, 2011
 *      Author: carolina
 */

#ifndef C_POLITE_H_
#define C_POLITE_H_

#include "net/queuebuf.h"
#include "sys/ctimer.h"


/**
 * \brief      Open a polite connection
 * \param c    A pointer to a struct polite_conn.
 * \param channel The channel number to be used for this connection
 * \param cb   A pointer to the callbacks used for this connection
 *
 *             This function opens a polite connection on the
 *             specified channel. The callbacks are called when a
 *             packet is received, or when another event occurs on the
 *             connection (see \ref "struct polite_callbacks").
 */
void c_polite_open(struct pipe* p, struct stackmodule_i *module);

/**
 * \brief      Close a polite connection
 * \param c    A pointer to a struct polite_conn that has previously been opened with polite_open().
 *
 *             This function closes a polite connection that has
 *             previously been opened with polite_open().
 */
void c_polite_close(struct pipe* p, struct stackmodule_i *module);


/**
 * \brief      Send a packet on a polite connection.
 * \param c    A pointer to a struct polite_conn that has previously been opened with polite_open().
 * \param interval The timer interval in which the packet should be sent.
 * \param hdrsize The size of the header that should be unique within the time interval.
 *
 *             This function sends a packet from the packetbuf on the
 *             polite connection. The packet is sent some time during
 *             the time interval, but only if no other packet is
 *             received with the same header.
 *
 */
int  c_polite_send(struct pipe* p, struct stackmodule_i *module);

void c_polite_recv(struct pipe* p, struct stackmodule_i *module);
/**
 * \brief      Cancel a pending packet
 * \param c    A pointer to a struct polite_conn that has previously been opened with polite_open().
 *
 *             This function cancels a pending transmission that has
 *             previously been started with polite_send().
 */
void  c_polite_sent(struct pipe* p, struct stackmodule_i *module);
void c_polite_cancel(struct pipe* p, struct stackmodule_i *module);
void c_polite_dropped(struct pipe* p, struct stackmodule_i *module);

#endif /* C_POLITE_H_ */

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
