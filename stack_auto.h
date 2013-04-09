/*
 * stack.h
 *
 *  Created on: Jun 10, 2012
 *      Author: carolina
 */

#ifndef STACK_H_
#define STACK_H_

#include "net/rime/amodule.h"

//@prefix rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .
//@prefix cpan: <http://downlode.org/rdf/cpan/0.1/cpan.rdf#> .
//@prefix owls: <http://www.daml.org/services/owl-s/1.1B/Process.owl#> .
//@prefix rdfs: <http://www.w3.org/2000/01/rdf-schema#> .
//@prefix daml: <http://www.daml.org/2002/03/agents/agent-ont#> .
//@prefix damlproc: <http://www.daml.org/services/owl-s/1.1/Process.owl#> .
//@prefix xsd:     <http://www.w3.org/2001/XMLSchema#> .

//@prefix crime: <http://sensors.ijs.si/crime#> .
//hasPrimitive, crime:OpenPrimitive, crime:ClosePrimitive

//turtle crime:c_open rdf:type crime:Command .
//turtle crime:c_close rdf:type crime:Command .
//turtle crime:c_send rdf:type crime:Command .
// crime:c_input rdf:type crime:Notification .
//turtle crime:c_sent rdf:type crime:Notification .
//turtle crime:c_recv rdf:type crime:Notification .
//turtle crime:c_dropped rdf:type crime:Notification .
//turtle crime:c_timed_out rdf:type crime:Notification .
//turtle crime:c_forward rdf:type crime:Command .
//turtle crime:c_discover rdf:type crime:Command .

//turtle crime:c_app rdf:type cpan:Module .
//turtle crime:c_app rdfs:comment The c_app module is mandatory and it assumes there's an application. .
//turtle crime:c_app crime:hasScope crime:input .

//turtle crime:c_app_recv rdf:type crime:c_recv .
//turtle crime:c_app crime:hasPrimitive crime:c_app_recv .

#define OUT 0
#define IN 1
#define SENDER 0
#define ESENDER 1
#define RECEIVER 2
#define ERECEIVER 3

static uint8_t STACKNO = 3;

//@defFor all
static uint8_t MAIN_STACK_ID = 0;

//@defFor c_route_discovery
static uint8_t RREQ_STACK_ID = 1;

//@defFor c_route_discovery
static uint8_t RREP_STACK_ID = 2;

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

#endif /* STACK_H_ */
