/*
 * amodule.c
 *
 *  Created on: 2011-10-17
 *      Author: carolina
 */

#include "net/rime.h"
#include "net/rime/crime/stack.h"
#include "net/rime/crime/c_channel.h"
#include "net/rime/crime/c_abc.h"
#include "net/rime/crime/c_broadcast.h"
#include "net/rime/crime/c_unicast.h"
#include "net/rime/crime/c_polite.h"
#include "net/rime/crime/c_multihop.h"
#include "net/rime/crime/c_trickle.h"
#include "net/rime/crime/c_netflood.h"

#include "vsntime.h"

#define DEBUG 0
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

#define EVAL 0

#if EVAL
int startTm2 = 0; int stopTm1 = 0;
#endif

int packets = 0;

void c_open(struct pipe *p, struct stackmodule_i *module, uint8_t len) {
	PRINTF("c_open %d %d\n", len, p->channel->channelno);
	if (module[len].c_open == NULL) return;
	uint8_t modno = stack[module[len].stack_id].modno;
	module[len].c_open(p, &module[len]);
	len++;
	if ((len >= 0) && (len < modno)) {
		c_open(p, module, len);
	}
	PRINTF("~c_open %d\n", len);
}

void c_close(struct pipe *p, struct stackmodule_i *module, uint8_t len) {
	PRINTF("c_close %d %d\n", len, p->channel->channelno);
	if (module[len].c_close == NULL) return;
	uint8_t modno = stack[module[len].stack_id].modno;
	if (module[len].time_trigger_flg == 1) {
		ctimer_stop(&module[len].timer);
		if (p->buf != NULL) {
			queuebuf_free(p->buf);
			p->buf = NULL;
		}
	}
	module[len].c_close(p, &module[len]);
	len++;
	if ((len >= 0) && (len < modno)) {
		c_close(p, module, len);
	}
	PRINTF("~c_close %d\n", len);
}

void c_recv(struct pipe *p, struct stackmodule_i *module, uint8_t len) {
	PRINTF("c_recv %d\n", len);

#if EVAL
	startTm2 = c_get_start_tm();
	stopTm1 = vsnTime_freeRunTimeDiff(startTm2);
	printf("%d, ", stopTm1);
#endif

	if (module[len].c_recv == NULL) return;
	uint8_t modno = stack[module[len].stack_id].modno;

#if EVAL
	startTm2 = vsnTime_freeRunTime();
#endif
	module[len].c_recv(p, &module[len]);
#if EVAL
	stopTm1 = vsnTime_freeRunTimeDiff(startTm2);
	printf("%d, ", stopTm1);

	startTm2 = vsnTime_freeRunTime();
#endif
	len++;
	if ((len >= 0) && (len < modno)) {
		c_recv(p, module, len);
	}
#if EVAL
	stopTm1 = vsnTime_freeRunTimeDiff(startTm2);
	printf("%d\n ", stopTm1);
#endif
	/*if (len == modno) {
		startTm2 = c_get_start_tm();
		stopTm1 = vsnTime_freeRunTimeDiff(startTm2);
		printf("%d %d %d\n ", len, startTm2, stopTm1);
	}*/

	//printf("%d\n ", packets++);
	PRINTF("~c_recv %d\n", len);
}

int c_send(struct pipe *p, struct stackmodule_i *module, uint8_t len) {
	PRINTF("c_send %d\n", len);
	int send_flg = module[len].c_send(p, &module[len]);
	len--;
	if ((len >= 0) && (len < 255) && (send_flg)) {
		c_send(p, module, len);
	}
	PRINTF("~c_send %d\n", len);
	return send_flg;
}

void c_triggered_send(struct trigger_param *param) {
	PRINTF("c_triggered_send\n");
	queuebuf_to_packetbuf(param->pip->buf);
	c_send(param->pip, param->amodule, param->modidx);

	int modno = param->modidx;//stack[param->amodule[param->modidx].stack_id].modno - 1;
	printf("!!!! %d %d %d %s\n", modno, param->amodule[modno].trigger_th,
			param->amodule[modno].trigger_no, (char*) param->pip->buf);
	if (param->amodule[modno].trigger_th &&
			(--param->amodule[modno].trigger_no > 0)) {
		param->pip->buf = queuebuf_new_from_packetbuf();
		ctimer_set(&param->amodule[modno].timer, param->rxmittime,
			c_triggered_send, param);
		PRINTF("c_triggered_send scheduled\n");
	}
	PRINTF("~c_triggered_send\n");
}

void c_sent(struct pipe *p, struct stackmodule_i *module, uint8_t len) {
	PRINTF("c_sent %d\n", len);
	//int startTm = vsnTime_freeRunTime();
	if (module[len].c_sent == NULL) return;
	uint8_t modno = stack[module[len].stack_id].modno;
	module[len].c_sent(p, &module[len]);
	len++;
	if ((len >= 0) && (len < modno)) {
		c_sent(p, module, len);
	}
	//int endTm = vsnTime_freeRunTimeDiff(startTm);
	PRINTF("~c_sent %d\n", len);
}

void c_dropped(struct pipe *p, struct stackmodule_i *module, uint8_t len) {
	PRINTF("c_dropped %d\n", len);
	if (module[len].c_sent == NULL) { return; }
	uint8_t modno = stack[module[len].stack_id].modno;
	module[len].c_dropped(p, &module[len]);
	len++;
	if ((len >= 0) && (len < modno)) {
		c_dropped(p, module, len);
	}
	PRINTF("~c_dropped %d\n", len);
}

void c_discover(struct pipe *p, struct stackmodule_i *module, uint8_t len) {
	PRINTF("c_discover %d\n", len);

	module[len].c_discover(p, &module[len]);
	len--;
	if ((len >= 0) && (len < 255)) {
		c_discover(p, module, len);
	}
	PRINTF("~c_discover %d\n", len);
}

rimeaddr_t* c_forward(struct pipe *p, struct stackmodule_i *module, uint8_t len) {
	PRINTF("c_forward \n");
	rimeaddr_t *tmpaddr;
	for (;;){
		if ((len >= 0) && (len < 255) && (module[len].c_forward != NULL)){
			tmpaddr = module[len].c_forward(p, &module[len]);
			PRINTF("%d%d %d %d %d\n", tmpaddr->u8[0], tmpaddr->u8[1], len, module[len].module_id, module[len].stack_id);
			return tmpaddr;
		}
		len--;
	}
	PRINTF("~c_forward %d\n", len);
	return NULL;
}

void c_timed_out(struct pipe *p, struct stackmodule_i *module, uint8_t len) {
	PRINTF("c_timed_out %d\n", len);
	if (module[len].c_timed_out == NULL) return;
	uint8_t modno = stack[module[len].stack_id].modno;
	module[len].c_timed_out(p, &module[len]);
	len++;
	if ((len >= 0) && (len < modno)) {
		c_timed_out(p, module, len);
	}
	PRINTF("~c_timed_out %d\n", len);
}

void set_node_addr(uint8_t stackid, uint8_t type,
		uint8_t addrid, rimeaddr_t* addr){
	if (type == IN) {
		if (addrid == SENDER){
			rimeaddr_copy(&stack[stackid].pip->in_sender, addr);
		} else if (addrid == ESENDER){
			rimeaddr_copy(&stack[stackid].pip->in_esender, addr);
		} else if (addrid == RECEIVER){
			rimeaddr_copy(&stack[stackid].pip->in_receiver, addr);
		} else if (addrid == ERECEIVER){
			rimeaddr_copy(&stack[stackid].pip->in_ereceiver, addr);
		}
	} else {
		if (addrid == SENDER){
			rimeaddr_copy(&stack[stackid].pip->out_sender, addr);
		} else if (addrid == ESENDER){
			rimeaddr_copy(&stack[stackid].pip->out_esender, addr);
		} else if (addrid == RECEIVER){
			rimeaddr_copy(&stack[stackid].pip->out_receiver, addr);
		} else if (addrid == ERECEIVER){
			rimeaddr_copy(&stack[stackid].pip->out_ereceiver, addr);
		}
	}
}

rimeaddr_t* get_node_addr(uint8_t stackid, uint8_t type, uint8_t addrid){
	if (type == IN) {
		if (addrid == SENDER){
			return &stack[stackid].pip->in_sender;
		} else if (addrid == ESENDER){
			return &stack[stackid].pip->in_esender;
		} else if (addrid == RECEIVER){
			return &stack[stackid].pip->in_receiver;
		} else if (addrid == ERECEIVER){
			return &stack[stackid].pip->in_ereceiver;
		}
	} else {
		if (addrid == SENDER){
			return &stack[stackid].pip->out_sender;
		} else if (addrid == ESENDER){
			return &stack[stackid].pip->out_esender;
		} else if (addrid == RECEIVER){
			return &stack[stackid].pip->out_receiver;
		} else if (addrid == ERECEIVER){
			return &stack[stackid].pip->out_ereceiver;
		}
	}
}

/*void amodule_set_node_addr(uint8_t stackid){
	rimeaddr_t *tmpaddr = get_node_addr(stackid, 0, 0);
	PRINTF("senderaddr %d.%d \n", tmpaddr->u8[0], tmpaddr->u8[1]);
	rimeaddr_set_node_addr(tmpaddr);
}*/
