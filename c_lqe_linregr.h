/*
 * lqe_linreg.h
 *
 *  Created on: Feb 3, 2014
 *      Author: carolina
 */

#ifndef LQE_LINREG_H_
#define LQE_LINREG_H_

/**
 * \brief      Set up a snooping function - works best over a boadcast connection
 * \param p    A pointer to a pipe struct.
 * \param module Pointer to an abstract module struct.
 *
 *             This function sets up snooping functionality on the
 *             specified channel.
 */
void c_lqe_linregr_open(struct pipe *p, struct stackmodule_i *module);

/**
 * \brief      Close the snooping functionality over the existing connection
 * \param p    A pointer to a pipe struct.
 * \param module Pointer to an abstract module struct.
 *
 *             This function is called when a snooping functionality is not needed anymore.
 */
void c_lqe_linregr_close(struct pipe *p, struct stackmodule_i *module);

/**
 * \brief      Empty function, snooping does not send packets.
 * \param p    A pointer to a pipe struct.
 * \param module Pointer to an abstract module struct.
 *
 *
 */
int c_lqe_linregr_send(struct pipe *p, struct stackmodule_i *module);

/**
 * \brief      A packet has been snooped, see if the neighbor list needs updating.
 * \param p    A pointer to a pipe struct.
 * \param module Pointer to an abstract module struct.
 *
 *             This function is called when a packet has to be sent by c_broadcast.
 */
void c_lqe_linregr_recv(struct pipe *p, struct stackmodule_i *module);

/**
 * \brief      Empty function, snooping does not notify after sending
 * \param p    A pointer to a pipe struct.
 * \param module Pointer to an abstract module struct.
 *
 *             This function is called when a packet has been successfully sent by c_broadcast.
 */
void c_lqe_linregr_sent(struct pipe *p, struct stackmodule_i *module);



#endif /* LQE_LINREG_H_ */
