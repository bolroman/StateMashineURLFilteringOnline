/*
 * FastLookup.h
 *
 *  Created on: Jan 11, 2011
 *      Author: yotamhc
 */

#ifndef FASTLOOKUP_H_
#define FASTLOOKUP_H_

#include "../Types.h"
#include "../StateTable.h"
#include "../../StateMachine/StateMachine.h"

#define GET_SECOND_LEVEL_HASH_KEY(previous, last) (((((uchar)(previous)) & 0x0FF) << 8) | (((uchar)(last)) & 0xFF))
#define LOOKUP_VALUE_NOT_FOUND (unsigned short)(0xFFFFFFFF)

#define GET_FIRST_LEVEL_STATE(machinePtr, lastChar) \
	((machinePtr)->firstLevelLookupTable[(int)((uchar)(lastChar))])

#define GET_SECOND_LEVEL_STATE(machinePtr, prevChar, lastChar) \
	(*((STATE_PTR_TYPE*)(hashmap_get((machinePtr)->secondLevelLookupHash, GET_SECOND_LEVEL_HASH_KEY((uchar)(prevChar), (uchar)(lastChar))))))

#define GET_STATE_POINTER_FROM_ID(machinePtr, stateID) \
	((machinePtr)->states->table[(int)(stateID)])


#endif /* FASTLOOKUP_H_ */
