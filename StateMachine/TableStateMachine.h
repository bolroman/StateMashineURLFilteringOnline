/*
 * TableStateMachine.h
 *
 *  Created on: Jan 23, 2011
 *      Author: yotamhc
 */

#ifndef TABLESTATEMACHINE_H_
#define TABLESTATEMACHINE_H_
#include "../Common/Types.h"
#include "../Dedup/Dictionary.h"
#include "../AhoCorasick/ACTypes.h"

#ifdef COUNT_BY_DEPTH
#ifndef USE_DEPTHMAP
#define USE_DEPTHMAP
#endif
#endif

typedef struct {
	STATE_PTR_TYPE_WIDE *table;
	unsigned char *matches;
	char **patterns;
	unsigned int numStates;
	Node **nodeTable;
#ifdef USE_DEPTHMAP
	int *depthMap;
#endif
} TableStateMachine;

#define GET_TABLE_IDX(state, c) \
	(((state) * 256) + (unsigned char)(c))

#define GET_NEXT_STATE(table, state, c) \
	((table)[GET_TABLE_IDX(state, c)])

TableStateMachine *createTableStateMachine(unsigned int numStates);
void destroyTableStateMachine(TableStateMachine *machine);

void setGoto(TableStateMachine *machine, STATE_PTR_TYPE_WIDE currentState, char c, STATE_PTR_TYPE_WIDE nextState);
void setMatch(TableStateMachine *machine, STATE_PTR_TYPE_WIDE state, char *pattern, int length);

int matchTableMachine(TableStateMachine *machine, char *input, int length, int verbose,
		long *numAccesses, long *accessesByDepth, long *accessesByState, unsigned int *lastState);

int match1TableMachine(TableStateMachine *machine, char *input, int length, int verbose,
		long *numAccesses, long *accessesByDepth, long *accessesByState, unsigned int *lastState, char foundedAnchorsArr[][100]);

int matchDictionaryTableMachine(TableStateMachine *machine, char *input, int length, int verbose,
			Dictionary *dict, int dict_word_size, RollingHashTool *hasher, long *totalSkipped, long *totalLeftBorderChars, long *totalLeftBorders,
			long *dictGetTries, long *memcmpTries, long *memcmpFails);

#endif /* TABLESTATEMACHINE_H_ */
