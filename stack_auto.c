/*
 * stack.c
 *
 *  Created on: Jun 10, 2012
 *      Author: carolina
 */
#include <stdlib.h>

#include "stack.h"
#include "net/rime/amodule.h"
#include "net/rime.h"
#include "net/rime/c_channel.h"
#include "net/rime/c_abc.h"
#include "net/rime/c_broadcast.h"
#include "net/rime/c_unicast.h"
#include "net/rime/c_polite.h"
#include "net/rime/c_multihop.h"
#include "net/rime/c_trickle.h"
#include "net/rime/c_netflood.h"
#include "net/rime/c_mesh.h"
#include "net/rime/c_route_discovery.h"

#define DEBUG 1
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

void printaddr(int stack_id) {
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

void stack_init(){
	PRINTF("stack init\n");
	//init the stacks structure (columns of the matrix, branches of the tree)
	stack = (struct stack_i*) calloc(STACKNO, sizeof(struct stack_i));
	rimeaddr_t *addr;

	//@defStack
	struct pipe *pi0; 
	pi0 = (struct pipe*) calloc(1, sizeof(struct pipe)); 
	struct channel *ch0; 
	ch0 = (struct channel*) calloc(1, sizeof(struct channel)); 
	stack[0].pip = pi0; 
	stack[0].pip->channel = ch0; 
	stack[0].modno = 2; 
	struct stackmodule_i *amodule0; 
	amodule0 = (struct stackmodule_i*) calloc( 
	stack[0].modno, sizeof(struct stackmodule_i)); 

	static struct packetbuf_attrlist c_attributes0[] = 
	{ 
	C_ABC_ATTRIBUTES PACKETBUF_ATTR_LAST 
	}; 

	stack[0].pip->channelno = 0; 
	stack[0].pip->attrlist = c_attributes0; 
	stack[0].pip->channel->channelno = stack[0].pip->channelno; 
	stack[0].pip->channel->attrlist = stack[0].pip->attrlist; 
	stack[0].amodule = amodule0; 

	amodule0[0].stack_id = 0; 
	amodule0[0].module_id = 0; 
	stack[0].pip->channel_no = 111;
	amodule0[0].c_open = c_channel_open;
	amodule0[0].c_close = c_channel_close;
	amodule0[0].c_recv = c_abc_input;
	amodule0[0].c_send = c_rime_output;

	amodule0[1].stack_id = 0; 
	amodule0[1].module_id = 1; 
	addr->u8[0] = 1; addr->u8[1] = 1;set_node_addr(0, OUT, SENDER, addr);
	amodule0[1].c_open = c_abc_open;
	amodule0[1].c_close = c_abc_close;
	amodule0[1].c_send = c_abc_send;
	amodule0[1].c_recv = c_abc_input;
	amodule0[1].c_sent = c_abc_sent;



/*
		if (amodule[MODNO - 1].time_trigger_flg == 1) {
			//put application data in the queue, it will be sent on time trigger
			packetbuf_copyfrom("test", 5);
			pip->buf = queuebuf_new_from_packetbuf();
			struct trigger_param *param;
			param = (struct trigger_param*) malloc(sizeof(struct trigger_param));
			param->pip = pip;
			param->amodule = amodule;
			param->modidx = MODNO - 1;
			param->rxmittime = amodule[MODNO - 1].trigger_interval;
			amodule[MODNO - 1].trigger_init_flg = 1;
			ctimer_set(&amodule[MODNO - 1].timer, param->rxmittime,
					c_triggered_send, param);
		}
		PRINTF("rime init: completed %d %s\n", MODNO, (char*)queuebuf_dataptr(pip->buf));
		*/
}

void stack_open(struct stack_i *stack){
	PRINTF("stack_open \n");
	int p;
	for (p = 0; p < STACKNO; p++) {
		int modno = 0;
		PRINTF("%d \n", stack[p].amodule[0].stack_id);
		if (stack[p].amodule[modno].c_open != NULL){
			c_open(stack[p].pip, stack[p].amodule, modno);
			PRINTF("~%d \n", stack[p].amodule[0].stack_id);
		}
	}
	PRINTF("~stack_open \n");
}

void stack_close(struct stack_i *stack){
	PRINTF("stack_close \n");
	int p;
	for (p = 0; p < STACKNO; p++) {
		int modno = stack[p].modno - 1;
		if (stack[p].amodule[modno].c_close != NULL){
			c_close(stack[p].pip, stack[p].amodule, modno);
		}
	}
	PRINTF("~stack_close \n");
}

int stack_send(struct stack_i *stack, uint8_t module_id){
	PRINTF("stack_send %d\n", module_id);
	int p; int sent_flag = 0;
	for (p = 0; p < STACKNO; p++) {
		int modno;
		if (module_id == -1) {
			modno = stack[p].modno - 1;
		} else {
			modno = module_id;
		}
		if (stack[p].amodule[modno].c_send != NULL){
			sent_flag = c_send(stack[p].pip, stack[p].amodule, modno);
		}
		break;
	}
	PRINTF("~stack_send \n");
	return sent_flag;
}

void stack_dropped(struct stack_i *stack){
	PRINTF("stack_dropped \n");
	int p;
	for (p = 0; p < STACKNO; p++) {
		int modno = stack[p].modno - 1;
		if (stack[p].amodule[modno].c_dropped != NULL){
			c_dropped(stack[p].pip, stack[p].amodule, modno - 1);
		}
	}
	PRINTF("~stack_dropped \n");
}

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
