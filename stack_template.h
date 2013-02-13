/*
 * stack.h
 *
 *  Created on: Jun 10, 2012
 *      Author: carolina
 */

#ifndef STACK_H_
#define STACK_H_

#include "net/rime/crime/amodule.h"

#define OUT 0
#define IN 1
#define SENDER 0
#define ESENDER 1
#define RECEIVER 2
#define ERECEIVER 3

//defStackNo

#define MAIN_STACK_ID 0
#define RREQ_STACK_ID 1
#define RREP_STACK_ID 2

struct stack_i {
	struct pipe *pip;
	struct stackmodule_i *amodule;
	uint8_t modno;
};

struct stack_i *stack;

void printaddr(int stack_id);
void stack_init();
void stack_open(struct stack_i *stack);
void stack_close(struct stack_i *stack);
int stack_send(struct stack_i *stack, uint8_t moduleid);
void stack_recv(struct stack_i *stack);
void stack_dropped(struct stack_i *stack);
void stack_timedout(struct stackmodule_i *module);

#endif /* STACK_H_ */

//@prefix rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .
//@prefix cpan: <http://downlode.org/rdf/cpan/0.1/cpan.rdf#> .
//@prefix owls: <http://www.daml.org/services/owl-s/1.1B/Process.owl#> .
//@prefix rdfs: <http://www.w3.org/2000/01/rdf-schema#> .
//@prefix daml: <http://www.daml.org/2002/03/agents/agent-ont#> .
//@prefix damlproc: <http://www.daml.org/services/owl-s/1.1/Process.owl#> .
//@prefix xsd:     <http://www.w3.org/2001/XMLSchema#> .
//@prefix crime: <http://sensorlab.ijs.si/2012/v0/crime.owl#> .


//turtle crime:c_app rdf:type cpan:Module .
//turtle crime:c_app rdfs:comment The c_app module is mandatory and it assumes there's an application. .
//turtle crime:c_app crime:hasScope crime:input .

//turtle crime:c_app_open rdf:type crime:c_open .
//turtle crime:c_app_close rdf:type crime:c_close .
//turtle crime:c_app_recv rdf:type crime:c_recv .
//turtle crime:c_app_send rdf:type crime:c_send .

//turtle crime:c_app_open crime:implements crime:bottom_interface .
//turtle crime:c_app_close crime:implements crime:bottom_interface .
//turtle crime:c_app_recv crime:implements crime:bottom_interface .
//turtle crime:c_app_send crime:implements crime:bottom_interface .

//turtle crime:c_app crime:defines crime:c_app_open .
//turtle crime:c_app crime:defines crime:c_app_close .
//turtle crime:c_app crime:defines crime:c_app_recv .
//turtle crime:c_app crime:defines crime:c_app_send .
