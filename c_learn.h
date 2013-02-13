/*
 * c_learn.h
 *
 *  Created on: Aug 10, 2012
 *      Author: carolina
 */

#ifndef C_LEARN_H_
#define C_LEARN_H_

struct route_model {
  struct route_model *next;
  rimeaddr_t dest;
  rimeaddr_t nexthop;
  uint8_t cost;
};

void learn_init(void);

#endif /* C_LEARN_H_ */
