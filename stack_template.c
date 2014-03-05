/*
 * stack.c
 *
 *  Created on: Jun 10, 2012
 *      Author: carolina
 */
#include <stdlib.h>

#include "stack.h"
#include "net/rime/crime/amodule.h"
#include "net/rime.h"
#include "net/rime/crime/c_channel.h"
#include "net/rime/crime/c_abc.h"
#include "net/rime/crime/c_broadcast.h"
#include "net/rime/crime/c_unicast.h"
#include "net/rime/crime/c_polite.h"
#include "net/rime/crime/c_multihop.h"
#include "net/rime/crime/c_trickle.h"
#include "net/rime/crime/c_netflood.h"
#include "net/rime/crime/c_mesh.h"
#include "net/rime/crime/c_route_discovery.h"
#include "net/rime/crime/c_echo_app.h"

#define DEBUG 0
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

void * calloc (size_t nm, size_t es){ return malloc(nm*es);}

/*---------------------------------------------------------------------------*/
void
printaddr(int stack_id)
{
  rimeaddr_t *sender = get_node_addr(stack_id, OUT, 0);

  rimeaddr_t *esender = get_node_addr(stack_id, OUT, 1);

  rimeaddr_t *receiver = get_node_addr(stack_id, OUT, 2);

  rimeaddr_t *ereceiver = get_node_addr(stack_id, OUT, 3);

  PRINTF("out: s%d.%d es%d.%d r%d.%d er%d.%d\n",
         sender->u8[0], sender->u8[1],
         esender->u8[0], esender->u8[1],
         receiver->u8[0], receiver->u8[1],
         ereceiver->u8[0], ereceiver->u8[1]);

  sender = get_node_addr(stack_id, IN, 0);
  esender = get_node_addr(stack_id, IN, 1);
  receiver = get_node_addr(stack_id, IN, 2);
  ereceiver = get_node_addr(stack_id, IN, 3);
  PRINTF("in: s%d.%d es%d.%d r%d.%d er%d.%d\n",
         sender->u8[0], sender->u8[1],
         esender->u8[0], esender->u8[1],
         receiver->u8[0], receiver->u8[1],
         ereceiver->u8[0], ereceiver->u8[1]);
}

/*---------------------------------------------------------------------------*/
void
stack_init()
{
  PRINTF("stack init\n");
  //init the stacks structure (columns of the matrix, branches of the tree)
  stack = (struct stack_i *)calloc(STACKNO, sizeof(struct stack_i));
  rimeaddr_t addr;

  //@defStack
}

/*---------------------------------------------------------------------------*/
void
stack_open(struct stack_i *stack)
{
  PRINTF("stack_open \n");
  int p;

  for(p = 0; p < STACKNO; p++) {
    int modno = 0;

    PRINTF("%d \n", stack[p].amodule[0].stack_id);
    if(stack[p].amodule[modno].c_open != NULL) {
      c_open(stack[p].pip, stack[p].amodule, modno);
      PRINTF("~%d \n", stack[p].amodule[0].stack_id);
    }
  }
  PRINTF("~stack_open \n");
}

/*---------------------------------------------------------------------------*/
void
stack_close(struct stack_i *stack)
{
  PRINTF("stack_close \n");
  int p;

  for(p = 0; p < STACKNO; p++) {
    int modno = stack[p].modno - 1;

    if(stack[p].amodule[modno].c_close != NULL) {
      c_close(stack[p].pip, stack[p].amodule, modno);
    }
  }
  PRINTF("~stack_close \n");
}

/*---------------------------------------------------------------------------*/
int
stack_send(struct stack_i *stack, uint8_t module_id)
{
  PRINTF("stack_send %d\n", module_id);
  int p;

  int sent_flag = 0;

  for(p = 0; p < STACKNO; p++) {
    int modno;

    if(module_id == -1) {
      modno = stack[p].modno - 1;
    } else {
      modno = module_id;
    }
    if(stack[p].amodule[modno].c_send != NULL) {
      sent_flag = c_send(stack[p].pip, stack[p].amodule, modno);
    }
    break;
  }
  PRINTF("~stack_send \n");
  return sent_flag;
}

/*---------------------------------------------------------------------------*/
void
stack_dropped(struct stack_i *stack)
{
  PRINTF("stack_dropped \n");
  int p;

  for(p = 0; p < STACKNO; p++) {
    int modno = stack[p].modno - 1;

    if(stack[p].amodule[modno].c_dropped != NULL) {
      c_dropped(stack[p].pip, stack[p].amodule, modno);
    }
  }
  PRINTF("~stack_dropped \n");
}

/*---------------------------------------------------------------------------*/
void
stack_timedout(struct stackmodule_i *module)
{
  PRINTF("stack_timedout \n");
  uint8_t stack_id = module->stack_id;

  uint8_t mod_id = module->module_id;

  int modno = stack[stack_id].modno - 1;

  if(mod_id <= modno) {
    if(stack[stack_id].amodule[modno].c_timed_out != NULL) {
      c_timed_out(stack[stack_id].pip, stack[stack_id].amodule, modno);
    }
  }
  if(stack[stack_id].amodule[mod_id].parent != NULL) {
    stack_timedout(stack[stack_id].amodule[mod_id].parent);
  }
  PRINTF("~stack_timedout \n");
}

/*---------------------------------------------------------------------------*/
/*
void stack_discover(struct stack_i *stack){
	PRINTF("stack_discover \n");
		int p;
		for (p = 0; p < STACKNO; p++) {
			int modno = stack[p].modno - 1;
			if (stack[p].amodule[modno].c_discover != NULL){
				c_discover(stack[p].pip, stack[p].amodule, modno - 1);
			}
		}
		PRINTF("~stack_dropped \n");
}
*/
