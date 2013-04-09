/*
 * c_echo_app.h
 *
 *  Created on: Feb 13, 2013
 *      Author: carolina
 */

#ifndef C_ECHO_APP_H_
#define C_ECHO_APP_H_

void c_echo_app_open(struct pipe *p, struct stackmodule_i *module);

void c_echo_app_close(struct pipe *p, struct stackmodule_i *module);

int c_echo_app_send(struct pipe *p, struct stackmodule_i *module);

void c_echo_app_recv(struct pipe *p, struct stackmodule_i *module);

void c_echo_app_sent(struct pipe *p, struct stackmodule_i *module);

rimeaddr_t *c_echo_app_forward(struct pipe *p, struct stackmodule_i *module);

void c_echo_app_timedout(struct pipe *p, struct stackmodule_i *module);

#endif /* C_ECHO_APP_H_ */

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
