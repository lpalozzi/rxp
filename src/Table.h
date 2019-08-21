/*
 * Table.h
 *
 *  Created on: 2018-04-01
 *      Author: leo
 */

#ifndef SRC_TABLE_H_
#define SRC_TABLE_H_

#include <bullet/LinearMath/btMotionState.h>

#include "Actor.h"
#include "Flipper.h"

class Table: public Actor, public btMotionState {
public:
	Table();
	virtual ~Table();

protected:
	Flipper *left_flipper_;
	Flipper *right_flipper_;
};

#endif /* SRC_TABLE_H_ */
