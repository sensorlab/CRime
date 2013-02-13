/*
 * c_echo_app.c
 *
 *  Created on: Feb 13, 2013
 *      Author: carolina
 */
#include "contiki.h"
#include "net/rime.h"
//#include "net/rime/route.h"
#include "net/rime/crime/c_echo_app.h"

#include <stddef.h> /* For offsetof */

#define DEBUG 0
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

/*---------------------------------------------------------------------------*/
rimeaddr_t *
c_echo_app_forward(struct pipe *p, struct stackmodule_i *module)
{
	PRINTF("~c_echo_app_forward \n");
    return NULL;
}
/*---------------------------------------------------------------------------*/
void
c_echo_app_timedout(struct pipe *p, struct stackmodule_i *module)
{
	PRINTF("~c_echo_app_timedout\n");
}
/*---------------------------------------------------------------------------*/

void
c_echo_app_open(struct pipe *p, struct stackmodule_i *module)
{
  PRINTF("~c_echo_app_open\n");
}
/*---------------------------------------------------------------------------*/
void
c_echo_app_close(struct pipe *p, struct stackmodule_i *module)
{
}
/*---------------------------------------------------------------------------*/
int
c_echo_app_send(struct pipe *p, struct stackmodule_i *module)
{
	PRINTF("~c_echo_app_send\n");
	printf("----------- sending %s \n", (char *) packetbuf_dataptr());
  return 1;
}
/*---------------------------------------------------------------------------*/
void
c_echo_app_recv(struct pipe *p, struct stackmodule_i *module)
{
	PRINTF("c_echo_app_recv\n");

	printf("----------- received %s \n", (char *) packetbuf_dataptr());
}

void c_echo_app_sent(struct pipe *p, struct stackmodule_i *module) {
	PRINTF("~c_echo_app_sent\n");
}


