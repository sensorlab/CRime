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

void
printaddr(int stack_id)
{
  rimeaddr_t *sender = get_node_addr(stack_id, 0, 0);

  rimeaddr_t *esender = get_node_addr(stack_id, 0, 1);

  rimeaddr_t *receiver = get_node_addr(stack_id, 0, 2);

  rimeaddr_t *ereceiver = get_node_addr(stack_id, 0, 3);

  PRINTF("out: s%d.%d es%d.%d r%d.%d er%d.%d\n",
         sender->u8[0], sender->u8[1],
         esender->u8[0], esender->u8[1],
         receiver->u8[0], receiver->u8[1],
         ereceiver->u8[0], ereceiver->u8[1]);

  sender = get_node_addr(stack_id, 1, 0);
  esender = get_node_addr(stack_id, 1, 1);
  receiver = get_node_addr(stack_id, 1, 2);
  ereceiver = get_node_addr(stack_id, 1, 3);
  PRINTF("in: s%d.%d es%d.%d r%d.%d er%d.%d\n",
         sender->u8[0], sender->u8[1],
         esender->u8[0], esender->u8[1],
         receiver->u8[0], receiver->u8[1],
         ereceiver->u8[0], ereceiver->u8[1]);
}

void
stack_init()
{
  PRINTF("stack init\n");
  //init the stacks structure (columns of the matrix, branches of the tree)
  stack = (struct stack_i *)calloc(STACKNO, sizeof(struct stack_i));
  rimeaddr_t *addr;

  //@defStack
  struct pipe *pi0;

  pi0 = (struct pipe *)calloc(1, sizeof(struct pipe));
  struct channel *ch0;

  ch0 = (struct channel *)calloc(1, sizeof(struct channel));
  stack[0].pip = pi0;
  stack[0].pip->channel = ch0;
  stack[0].modno = 5;
  struct stackmodule_i *amodule0;

  amodule0 =
    (struct stackmodule_i *)calloc(stack[0].modno,
                                   sizeof(struct stackmodule_i));

  static struct packetbuf_attrlist c_attributes0[] = {
    //@defStack attributes
    C_MULTIHOP_ATTRIBUTES PACKETBUF_ATTR_LAST
  };

  stack[0].pip->channelno = 132;
  stack[0].pip->attrlist = c_attributes0;
  stack[0].pip->channel->channelno = stack[0].pip->channelno;
  stack[0].pip->channel->attrlist = stack[0].pip->attrlist;
  stack[0].amodule = amodule0;

  amodule0[0].stack_id = 0;
  amodule0[0].module_id = 0;
  amodule0[0].c_close = c_channel_close;
  amodule0[0].c_open = c_channel_open;
  amodule0[0].c_recv = c_abc_input;
  amodule0[0].c_send = c_rime_output;

  addr->u8[0] = 0;
  addr->u8[1] = 3;
  set_node_addr(0, 0, 0, addr);
  amodule0[1].stack_id = 0;
  amodule0[1].module_id = 1;
  amodule0[1].c_close = c_abc_close;
  amodule0[1].c_open = c_abc_open;
  amodule0[1].c_recv = c_abc_recv;
  amodule0[1].c_send = c_abc_send;
  amodule0[1].c_sent = c_abc_sent;

  addr->u8[0] = 0;
  addr->u8[1] = 3;
  set_node_addr(0, 0, 0, addr);
  addr->u8[0] = 1;
  addr->u8[1] = 0;
  set_node_addr(0, 0, 3, addr);
  amodule0[2].stack_id = 0;
  amodule0[2].module_id = 2;
  amodule0[2].c_close = c_unicast_close;
  amodule0[2].c_open = c_unicast_open;
  amodule0[2].c_recv = c_unicast_recv;
  amodule0[2].c_send = c_unicast_send;
  amodule0[2].c_sent = c_unicast_sent;

  addr->u8[0] = 0;
  addr->u8[1] = 3;
  set_node_addr(0, 0, 1, addr);
  addr->u8[0] = 1;
  addr->u8[1] = 0;
  set_node_addr(0, 0, 4, addr);
  amodule0[3].stack_id = 0;
  amodule0[3].module_id = 3;
  amodule0[3].c_close = c_multihop_close;
  amodule0[3].c_open = c_multihop_open;
  amodule0[3].c_recv = c_multihop_recv;
  amodule0[3].c_send = c_multihop_send;
  amodule0[3].c_sent = c_multihop_sent;
  amodule0[3].c_forward = c_multihop_forward;

  amodule0[4].stack_id = 0;
  amodule0[4].module_id = 4;
  amodule0[4].c_close = c_mesh_close;
  amodule0[4].c_open = c_mesh_open;
  amodule0[4].c_recv = c_mesh_recv;
  amodule0[4].c_send = c_mesh_send;
  amodule0[4].c_sent = c_mesh_sent;
  amodule0[4].c_timed_out = c_mesh_timed_out;
  amodule0[4].c_forward = c_mesh_forward;


  PRINTF("stack0 completed %d %s %d\n", stack[0].modno,
         (char *)queuebuf_dataptr(stack[0].pip->buf),
         stack[0].pip->channel->channelno);

  static struct pipe pi1;

  static struct channel ch1;

  stack[1].pip = &pi1;
  stack[1].pip->channel = &ch1;
  stack[1].modno = 5;

  struct stackmodule_i *amodule1;

  amodule1 =
    (struct stackmodule_i *)calloc(stack[1].modno,
                                   sizeof(struct stackmodule_i));

  static struct packetbuf_attrlist c_attributes1[] = {
    //@defStack attributes
    C_NETFLOOD_ATTRIBUTES PACKETBUF_ATTR_LAST
  };
  stack[1].pip->channel->channelno = 133;
  stack[1].pip->channelno = 133;
  stack[1].pip->attrlist = c_attributes1;
  amodule1[0].stack_id = 1;
  amodule1[0].module_id = 0;
  amodule1[0].c_close = c_channel_close;
  amodule1[0].c_open = c_channel_open;
  amodule1[0].c_recv = c_abc_input;
  amodule1[0].c_send = c_rime_output;

  amodule1[1].stack_id = 1;
  amodule1[1].module_id = 1;
  amodule1[1].c_close = c_abc_close;
  amodule1[1].c_open = c_abc_open;
  amodule1[1].c_recv = c_abc_recv;
  amodule1[1].c_send = c_abc_send;
  amodule1[1].c_sent = c_abc_sent;

  stack[1].pip->polite_param.identified_flg = 1;
  stack[1].pip->polite_param.hdrsize = 4;
  stack[1].pip->polite_param.duplicate_no = 0;
  addr->u8[0] = 0;
  addr->u8[1] = 3;
  set_node_addr(0, 0, 0, addr);
  amodule1[2].stack_id = 1;
  amodule1[2].module_id = 2;
  amodule1[2].c_close = c_polite_close;
  amodule1[2].c_open = c_polite_open;
  amodule1[2].c_recv = c_polite_recv;
  amodule1[2].c_send = c_polite_send;
  amodule1[2].c_sent = c_polite_sent;
  amodule1[2].c_dropped = c_polite_dropped;

  amodule1[3].stack_id = 1;
  amodule1[3].module_id = 3;
  amodule1[3].c_close = c_netflood_close;
  amodule1[3].c_open = c_netflood_open;
  amodule1[3].c_recv = c_netflood_recv;
  amodule1[3].c_send = c_netflood_send;
  amodule1[3].c_sent = c_netflood_sent;

  stack[1].pip->route_discovery_param.timeout = 10 * CLOCK_SECOND;
  amodule1[4].stack_id = 1;
  amodule1[4].module_id = 4;
  //amodule1[4].packet_timeout = 10;
  //amodule1[4].seq_no = 0;
  amodule1[4].c_close = c_route_discovery_close;
  amodule1[4].c_open = c_route_discovery_open;
  amodule1[4].c_send = c_route_discovery_discover;
  amodule1[4].c_recv = c_route_discovery_recv;

  stack[1].amodule = amodule1;

  stack[1].pip->channel->channelno = stack[1].pip->channelno;
  stack[1].pip->channel->attrlist = stack[1].pip->attrlist;

  PRINTF("stack1 completed %d %s %d\n", stack[1].modno,
         (char *)queuebuf_dataptr(stack[1].pip->buf),
         stack[1].pip->channel->channelno);

  static struct pipe pi2;

  static struct channel ch2;

  stack[2].pip = &pi2;
  stack[2].pip->channel = &ch2;
  stack[2].modno = 4;

  struct stackmodule_i *amodule2;

  amodule2 =
    (struct stackmodule_i *)calloc(stack[2].modno,
                                   sizeof(struct stackmodule_i));

  static struct packetbuf_attrlist c_attributes2[] = {
    //@defStack attributes
    C_UNICAST_ATTRIBUTES PACKETBUF_ATTR_LAST
  };
  stack[2].pip->channel->channelno = 134;
  stack[2].pip->channelno = 134;
  stack[2].pip->attrlist = c_attributes2;
  amodule2[0].stack_id = 2;
  amodule2[0].module_id = 0;
  amodule2[0].c_close = c_channel_close;
  amodule2[0].c_open = c_channel_open;
  amodule2[0].c_recv = c_abc_input;
  amodule2[0].c_send = c_rime_output;

  amodule2[1].stack_id = 2;
  amodule2[1].module_id = 1;
  amodule2[1].c_close = c_abc_close;
  amodule2[1].c_open = c_abc_open;
  amodule2[1].c_recv = c_abc_recv;
  amodule2[1].c_send = c_abc_send;
  amodule2[1].c_sent = c_abc_sent;

  addr->u8[0] = 0;
  addr->u8[1] = 3;
  set_node_addr(0, 0, 0, addr);
  addr->u8[0] = 1;
  addr->u8[1] = 0;
  set_node_addr(0, 0, 3, addr);
  amodule2[2].stack_id = 2;
  amodule2[2].module_id = 2;
  amodule2[2].c_close = c_unicast_close;
  amodule2[2].c_open = c_unicast_open;
  amodule2[2].c_recv = c_unicast_recv;
  amodule2[2].c_send = c_unicast_send;
  amodule2[2].c_sent = c_unicast_sent;

  amodule2[3].stack_id = 2;
  amodule2[3].module_id = 3;
  amodule2[3].c_recv = c_route_discovery_recv;

  stack[2].amodule = amodule2;

  stack[2].pip->channel->channelno = stack[2].pip->channelno;
  stack[2].pip->channel->attrlist = stack[2].pip->attrlist;

  PRINTF("stack2 completed %d %s %d\n", stack[2].modno,
         (char *)queuebuf_dataptr(stack[2].pip->buf),
         stack[2].pip->channel->channelno);


  printaddr(0);
  printaddr(1);
  printaddr(2);
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

void
stack_dropped(struct stack_i *stack)
{
  PRINTF("stack_dropped \n");
  int p;

  for(p = 0; p < STACKNO; p++) {
    int modno = stack[p].modno - 1;

    if(stack[p].amodule[modno].c_dropped != NULL) {
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
