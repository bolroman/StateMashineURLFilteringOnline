/*
 * StateMachine.h
 *
 *  Created on: Jan 12, 2011
 *      Author: yotamhc
 */

#ifndef STATEMACHINE_H_
#define STATEMACHINE_H_

#include "../Common/Types.h"
#include "../Common/HashMap/HashMap.h"
#include "../AhoCorasick/ACTypes.h"
#include "../Common/PatternTable.h"

typedef struct {
	int totalFailures;
	int totalGotos;
} MachineStats;


typedef struct {
	StateTable *states;
	PatternTable *patternTable;
	Node *nodeTable[100000];
	STATE_PTR_TYPE firstLevelLookupTable[256];
	HashMap *secondLevelLookupHash;
	int isLoadedFromDump;
	int isSimple;
} StateMachine;

//int getNextState(StateMachine *machine, State *current, char *str, int length, int *idx, NextStateResult *result);
int match(StateMachine *machine, char *input, int length, int verbose, MachineStats *stats);
void compressStateTable(StateMachine *machine);
int getStateID(State *state);

#endif /* STATEMACHINE_H_ */
