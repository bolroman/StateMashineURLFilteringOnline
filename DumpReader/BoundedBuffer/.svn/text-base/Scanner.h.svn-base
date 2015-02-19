/*
 * Scanner
 * Functions to initialize, start, stop and destroy a scanner
 */

#include <pthread.h>
#include "LinkedList.h"
#include "../../StateMachine/StateMachine.h"
#include "../../Dedup/Dictionary.h"

typedef struct scanner_data {
	StateMachine *machine;
	int isTableMachine;
	int verbose;
	LinkedList *packet_queue;
	LinkedList free_queue;
	MachineStats stats;
	Dictionary *dict;
	RollingHashTool *hasher;
	int dict_word_size;
	pthread_t thread_id;
	int done;
	long totalBytes;
	long totalSkipped;
	long totalLeftBorderChars;
	long totalLeftBorders;
	long totalDictGetTries;
	long totalMemcmpTries;
	long totalMemcmpFails;
} ScannerData;

void scanner_init(ScannerData *scanner, StateMachine *machine, int isTableMachine, LinkedList *packet_queue, Dictionary *dict, RollingHashTool *hasher, int dict_word_size, int verbose);

int scanner_start(ScannerData *scanner);

void scanner_join(ScannerData *scanner);

void scanner_free_packets(ScannerData *scanner);
