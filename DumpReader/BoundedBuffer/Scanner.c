/*
 * Implementation of a scanner
 */

#include <stdlib.h>
#include <stdio.h>
#include "../DumpReader.h"
#include "Scanner.h"
#include "../../StateMachine/TableStateMachine.h"

const unsigned int MASKS[] = { 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF };

void printIP(unsigned int ip) {
	printf("%u.%u.%u.%u", ((ip & MASKS[0]) >> 24), ((ip & MASKS[1]) >> 16), ((ip & MASKS[2]) >> 8), ip & MASKS[3]);
}

void *start_scanner_thread(void *param) {
	int status;
	ScannerData *scanner;
	Packet *packet;
#ifdef PRINT_MATCHING_TIME
	Timer matchTimer;
#endif
#ifdef COUNT_THROUGHPUT_ONLY_FOR_MARKED_PACKETS
	Timer matchMarkedTimer;
	unsigned long markedSize;
#endif

#ifdef PRINT_MATCHING_TIME
	matchTimer.micros = 0;
#endif

#ifdef COUNT_THROUGHPUT_ONLY_FOR_MARKED_PACKETS
	matchMarkedTimer.micros = 0;
	markedSize = 0;
#endif

#ifdef COUNT_BY_DEPTH
	// Histogram of accesses by depth
	// Histogram of accesses by state

	long numAccesses, idx;

	long accessesByDepth[200];
	long *accessesByState;
#endif

	scanner = (ScannerData*)param;

#ifdef COUNT_BY_DEPTH
	accessesByState = (long*)malloc(sizeof(long) * ((TableStateMachine*)(scanner->machine))->numStates);

	memset(accessesByState, 0, sizeof(long) * ((TableStateMachine*)(scanner->machine))->numStates);
	memset(accessesByDepth, 0, sizeof(long) * 200);

	numAccesses = 0;

#endif

	scanner->totalBytes = 0;
	scanner->totalSkipped = 0;
	scanner->totalDictGetTries = 0;
	scanner->totalMemcmpFails = 0;
	scanner->totalMemcmpTries = 0;
	scanner->totalLeftBorderChars = 0;
	scanner->totalLeftBorders = 0;

	while (!scanner->done) {

		packet = (Packet*)list_dequeue(scanner->packet_queue, &status);
		if (status == LIST_STATUS_DEQUEUE_SUCCESS && packet) {
			if (scanner->verbose) {
				printf("Scanning packet (size=%u) from ", packet->size);
				printIP(packet->source);
				printf(" to ");
				printIP(packet->dest);
				printf(" from time %u:\n", packet->time);
				printf("------------------------------------\n");
			}
			if (!(scanner->isTableMachine)) {
				match(scanner->machine, packet->data, packet->size, scanner->verbose, &(scanner->stats));
			} else {
#ifndef COUNT_BY_DEPTH
#ifdef PRINT_MATCHING_TIME
				startTiming(&matchTimer);
#endif
#ifdef USE_DICTIONARY_ONLY_FOR_MARKED_PACKETS
				if (packet->source != 0 && scanner->dict) {
#ifdef COUNT_THROUGHPUT_ONLY_FOR_MARKED_PACKETS
					startTiming(&matchMarkedTimer);
#endif
#else
				if (scanner->dict) {
#endif
					matchDictionaryTableMachine((TableStateMachine*)(scanner->machine),
							packet->data, packet->size, scanner->verbose,
							scanner->dict, scanner->dict_word_size, scanner->hasher,
							&(scanner->totalSkipped),
							&(scanner->totalLeftBorderChars),
							&(scanner->totalLeftBorders),
							&(scanner->totalDictGetTries),
							&(scanner->totalMemcmpTries),
							&(scanner->totalMemcmpFails));
#ifdef COUNT_THROUGHPUT_ONLY_FOR_MARKED_PACKETS
					endTiming(&matchMarkedTimer);
					markedSize += packet->size;
#endif
				} else {
					matchTableMachine((TableStateMachine*)(scanner->machine),
							packet->data,
							packet->size,
							scanner->verbose,
							NULL, NULL, NULL, NULL);
				}
#ifdef PRINT_MATCHING_TIME
				endTiming(&matchTimer);
#endif
#else
				matchTableMachine((TableStateMachine*)(scanner->machine),
						packet->data,
						packet->size,
						scanner->verbose,
						&numAccesses, accessesByDepth, accessesByState, NULL);
#endif
			}
#ifdef COUNT_DICTIONARY_SKIPPED_BYTES
			scanner->totalBytes += packet->size;
#endif
			list_enqueue(&(scanner->free_queue), (void*)packet);

		} else {
			break;
		}

	}

#ifdef COUNT_BY_DEPTH

#define DEPTH_THRESHOLD_FOR_DAVID_RATIO 1

	long deep_accesses = 0;
	long total_accesses = 0;

	for (idx = 0; idx < 200; idx++) {
		//printf("%d\t%ld", idx, accessesByDepth[idx]);
		if (idx > DEPTH_THRESHOLD_FOR_DAVID_RATIO) {
			deep_accesses += accessesByDepth[idx];
		}
		total_accesses += accessesByDepth[idx];
	}

	printf("Deep accesses: %ld\n", deep_accesses);
	printf("Total accesses: %ld\n", total_accesses);
	printf("David's ratio: %f\n", ((double)deep_accesses) / total_accesses);

	free(accessesByState);
#endif

#ifdef COUNT_CALLS
	printCounters();
#endif

#ifdef X_RATIO
	printXRatio();
#endif

#ifdef PRINT_MATCHING_TIME
	printf("Total time on matching: %ld micros\n", matchTimer.micros);
#endif
#ifdef COUNT_THROUGHPUT_ONLY_FOR_MARKED_PACKETS
	printf("Total throughput on matching marked packets: %5.3fMbps\n", GET_TRANSFER_RATE(markedSize, matchMarkedTimer));
#endif

	return NULL;	
}

void scanner_init(ScannerData *scanner, StateMachine *machine, int isTableMachine, LinkedList *packet_queue, Dictionary *dict, RollingHashTool *hasher, int dict_word_size, int verbose) {
	scanner->done = 0;
	scanner->machine = machine;
	scanner->isTableMachine = isTableMachine;
	scanner->packet_queue = packet_queue;
	scanner->verbose = verbose;
	scanner->stats.totalFailures = 0;
	scanner->stats.totalGotos = 0;
	scanner->dict = dict;
	scanner->hasher = hasher;
	scanner->dict_word_size = dict_word_size;
	list_init(&(scanner->free_queue));
}

int scanner_start(ScannerData *scanner) {
	return pthread_create(&(scanner->thread_id), NULL, start_scanner_thread, (void*)scanner);
}

void scanner_join(ScannerData *scanner) {
	pthread_join(scanner->thread_id, NULL);
}

void scanner_free_packets(ScannerData *scanner) {
	Packet *p;
	int status;
	list_set_done(&(scanner->free_queue));

	do {
		p = (Packet*)list_dequeue(&(scanner->free_queue), &status);
		if (status == LIST_STATUS_DEQUEUE_SUCCESS && p) {
			free(p->data);
			free(p);
		}
	} while (p && status == LIST_STATUS_DEQUEUE_SUCCESS);
	list_destroy(&(scanner->free_queue), 0);
}
