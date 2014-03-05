
/*
 * Copyright (c) 2011, Jozef Stefan Institute.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file may be part of the Contiki operating system.
 *
 * $Id: amodule.h,v 0.1 2011/10/11 19:19:17 cfortuna Exp $
 */

/**
 * \file
 *         Header file for amodule (abstract module) for C_Rime
 * \author
 *         Carolina Fortuna <carolina.fortuna@ijs.si>
 */

#ifndef __AMODULE_H__
#define __AMODULE_H__

#include "sys/ctimer.h"
#include "net/queuebuf.h"
#include "net/packetbuf.h"
#include "net/rime/channel.h"
#include "lib/list.h"
#include "lib/memb.h"


#define C_ABC_ATTRIBUTES
#define C_BROADCAST_ATTRIBUTES  { PACKETBUF_ADDR_SENDER, PACKETBUF_ADDRSIZE }, \
		C_ABC_ATTRIBUTES
#define C_POLITE_ATTRIBUTES C_ABC_ATTRIBUTES
#define C_UNICAST_ATTRIBUTES   { PACKETBUF_ADDR_RECEIVER, PACKETBUF_ADDRSIZE }, \
				{PACKETBUF_ADDR_SENDER, PACKETBUF_ADDRSIZE}, C_ABC_ATTRIBUTES
#define C_MULTIHOP_ATTRIBUTES   { PACKETBUF_ADDR_ESENDER, PACKETBUF_ADDRSIZE }, \
                              { PACKETBUF_ADDR_ERECEIVER, PACKETBUF_ADDRSIZE }, \
                              { PACKETBUF_ATTR_HOPS, PACKETBUF_ATTR_BIT * 5 }, \
                                C_UNICAST_ATTRIBUTES
#define C_MESH_ATTRIBUTES C_MULTIHOP_ATTRIBUTES
#define C_IPOLITE_ATTRIBUTES IBC_ATTRIBUTES
#define C_LQE_EWMA_ATTRIBUTES { PACKETBUF_ADDR_SENDER, PACKETBUF_ADDRSIZE }, \
							  {PACKETBUF_ATTR_PACKET_ID, PACKETBUF_ATTR_BIT * 8}, \
							  C_ABC_ATTRIBUTES

/* faulty, does not compile! In Contiki, the structure is not even used, although is defined
#define C_NETFLOOD_ATTRIBUTES   { PACKETBUF_ADDR_ESENDER, PACKETBUF_ADDRSIZE }, \
                              { PACKETBUF_ATTR_HOPS, PACKETBUF_ATTR_BIT * 5 }, \
                              { PACKETBUF_ATTR_EPACKET_ID, PACKETBUF_ATTR_BIT * 4 }, \
                              C_POLITE_ATTRIBUTES
*/

#define C_NETFLOOD_ATTRIBUTES   C_BROADCAST_ATTRIBUTES
#define C_ROUTE_DISCOVERY_ATTRIBUTES C_NETFLOOD_ATTRIBUTES

#ifdef NEIGHBOR_CONF_ENTRIES
#define NUM_NEIGHBOR_ENTRIES NEIGHBOR_CONF_ENTRIES
#else /* NEIGHBOR_CONF_ENTRIES */
#define NUM_NEIGHBOR_ENTRIES 8
#endif /* NEIGHBOR_CONF_ENTRIES */

#ifdef ROUTE_CONF_ENTRIES
#define NUM_RT_ENTRIES ROUTE_CONF_ENTRIES
#else /* ROUTE_CONF_ENTRIES */
#define NUM_RT_ENTRIES 8
#endif /* ROUTE_CONF_ENTRIES */

struct netflood_hdr {
	uint16_t originator_seq_no;
	rimeaddr_t originator;
	uint16_t hop_no;
};

//@definedFor c_polite
struct polite_p {
  struct ctimer timer;
  struct queuebuf *q;
  uint8_t hdrsize;
  uint8_t maxdups;
  uint8_t duplicate_no;
};

struct netflood_p {
  clock_time_t queue_time;
  rimeaddr_t last_originator;
  uint8_t last_originator_seq_no;
  uint8_t rebroadcast_flag;
  uint8_t doFlood;
  struct netflood_hdr hdr;
  uint8_t hops;
  struct queuebuf *queuebuf;

};

struct trickle_p {
  struct ctimer timer, interval_timer, first_transmission_timer;
  struct pt pt;
  struct queuebuf *q;
  clock_time_t interval;
  uint8_t seq_no;
  uint8_t interval_scaling;
  uint8_t duplicate_no;
};

struct route_discovery_p {
  struct ctimer timer;
  clock_time_t timeout;
  rimeaddr_t last_rreq_originator;
  uint16_t last_rreq_id;
  uint16_t rreq_id;
  uint8_t trigger_flg;
};

//@definedFor c_multihop
struct multihop_p {
  uint8_t hop_no;
};

struct mesh_p {
  struct queuebuf *queued_data;
  rimeaddr_t queued_data_dest;
  uint8_t packet_timeout;
};

struct link_stats_p {
	uint8_t maxpr_pts;
	uint8_t maxrssi_pts;
	uint8_t maxlqi_pts;
};

struct lqe_ewma_p {
	double alpha;
	uint16_t packet_loss;
};

struct lqe_linreg_p {
};

struct c_route_entry {
  struct c_route_entry *next;
  rimeaddr_t dest;
  rimeaddr_t nexthop;
  uint8_t seqno;
  uint8_t hopno;
  uint8_t cost;
  uint8_t time;
  uint8_t decay;
  uint8_t time_last_decay;
};

struct c_neighbor {
  struct c_neighbor *next;
  rimeaddr_t addr;

  uint8_t count_lock;
  uint8_t cost_lock;

  //software estimators
  uint16_t last_seq_no;
  clock_time_t first_time_stamp;
  clock_time_t last_time_stamp;
  uint16_t count;
  double rate;
  double cost;
  uint16_t m; //missed_packets;
  uint16_t k; //guessed_missed_packets;
  int16_t recv_packets;
  double prr;
  double xy;
  double x_2;

  // hardware estimators
  int16_t rssi_count;
  int16_t lqi_count;
  int16_t rssi;
  int16_t lqi;
};

/* Pipe is a structure containing a channel and the associated vlayer.*/
struct pipe {
  struct channel *channel;
  //vlayer
  uint16_t channel_no;
  //@definedFor c_abc, c_broadcast, c_unicast, c_trickle, c_polite, c_unicast
  struct queuebuf *buf;
  struct packetbuf_attrlist *attrlist;
  //@definedFor c_broadcast, c_unicast
  rimeaddr_t in_sender, out_sender;
  //@definedFor c_unicast
  rimeaddr_t in_receiver, out_receiver;
  //@definedFor c_multihop
  rimeaddr_t in_esender, out_esender;
  //@definedFor c_multihop
  rimeaddr_t in_ereceiver, out_ereceiver;

  int status;
  int num_tx;
  uint8_t seq_no;
  uint8_t hop_no;

  //@defineParams
  struct polite_p polite_param;
  struct netflood_p netflood_param;
  struct trickle_p trickle_param;
  struct route_discovery_p route_discovery_param;
  struct multihop_p multihop_param;
  struct mesh_p mesh_param;
  struct link_stats_p link_stats_param;
  struct lqe_ewma_p lqe_ewma_param;

  list_t route_table;
  struct memb route_mem;
  list_t neighbor_list;
  struct memb neighbor_mem;
};

struct stackmodule_i {
  uint8_t stack_id;
  uint8_t module_id;
  /* This flag is set to 1 if the module is called when a timer expires. */
  uint8_t time_trigger_flg;
  /* This flag is set to 1 if the time trigger associated with the module
   * has been initialized. */
  uint8_t trigger_init_flg;

  /* Holds the maximum number of allowed triggers for the module. */
  uint8_t trigger_no;
  uint8_t trigger_th;

  /* Timer for triggering the module. */
  //@definedFor c_abc, c_broadcast, c_unicast, c_trickle, c_polite, c_unicast
  struct ctimer timer;
  clock_time_t trigger_interval;

  struct stackmodule_i *parent;

  void (*c_open) (struct pipe * p, struct stackmodule_i * module);
  void (*c_close) (struct pipe * p, struct stackmodule_i * module);
  int (*c_send) (struct pipe * p, struct stackmodule_i * module);
  void (*c_recv) (struct pipe * p, struct stackmodule_i * module);
  void (*c_sent) (struct pipe * p, struct stackmodule_i * module);
  void (*c_dropped) (struct pipe * p, struct stackmodule_i * module);
  void (*c_timed_out) (struct pipe * p, struct stackmodule_i * module);
  //void (* c_cancelled)(struct pipe *p, struct stackmodule_i *module);
  //@definedFor c_rnd_routing
  //@prerequisiteFor c_multihop
  rimeaddr_t *(*c_forward) (struct pipe * p, struct stackmodule_i * module);
  int (*c_discover) (struct pipe * p, struct stackmodule_i * module);
  void (*c_read_chunk) (struct pipe * p, struct stackmodule_i * module);
  void (*c_write_chunk) (struct pipe * p, struct stackmodule_i * module);
  void (*c_new_route) (struct pipe * p, struct stackmodule_i * module);
};

void c_close(struct pipe *p, struct stackmodule_i *module, uint8_t len);

void c_open(struct pipe *p, struct stackmodule_i *module, uint8_t len);

void c_recv(struct pipe *p, struct stackmodule_i *module, uint8_t len);

int c_send(struct pipe *p, struct stackmodule_i *module, uint8_t len);

void c_sent(struct pipe *p, struct stackmodule_i *module, uint8_t len);

void c_dropped(struct pipe *p, struct stackmodule_i *module, uint8_t len);

void c_discover(struct pipe *p, struct stackmodule_i *module, uint8_t len);

void c_timed_out(struct pipe *p, struct stackmodule_i *module, uint8_t len);

struct trigger_hdr {
	uint16_t originator_seq_no;
	rimeaddr_t originator;
	uint16_t hop_no;
	rimeaddr_t dest;
  	uint8_t rreq_id;
};

/*struct trigger_route_msg {
  rimeaddr_t dest;
  uint8_t rreq_id;
  uint8_t pad;
};*/

struct trigger_param {
  //struct pipe *pip;
  //struct stackmodule_i *amodule;
  char *buf;
  struct trigger_hdr hdr;
  //struct queuebuf *queuebuf;
  //struct trigger_route_msg *msg;
  uint8_t stackidx;
  uint8_t modidx;
  uint8_t triggerno;
} param;

void c_triggered_send(struct trigger_param *param);

void set_amodule_trigger(int stackIdx);

void set_node_addr(uint8_t stackid, uint8_t type, uint8_t addrid,
                   rimeaddr_t * addr);
rimeaddr_t *get_node_addr(uint8_t stackid, uint8_t type, uint8_t addrid);

#endif /* __AMODULE_H__ */
