/*
 * DumpReader.h
 *
 *  Created on: Jan 17, 2011
 *      Author: yotamhc
 */

#ifndef DUMPREADER_H_
#define DUMPREADER_H_

#include "../StateMachine/StateMachine.h"

typedef struct {
	unsigned int size;
	unsigned int source;
	unsigned int dest;
	unsigned int time;
	char *data;
} Packet;

#define BYTES_TO_MBITS 8.0/(1024 * 1024)
#define GET_TRANSFER_RATE(bytes, timer) (((bytes) * BYTES_TO_MBITS) / ((timer.micros) / 1000000.0))

void inspectDumpFile(const char *path, const char *dict_path, int dict_width, StateMachine *machine, int isTableMachine, int verbose, int timing, int threads);
void runTest(StateMachine *machine, int isTableMachine);

void startTiming(Timer *timer);
void endTiming(Timer *timer);

#endif /* DUMPREADER_H_ */
