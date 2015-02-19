#ifndef TABLESTATEMACHINETEST_H_
#define TABLESTATEMACHINETEST_H_

#include "CommonFunctions.h"
#include "StateMachine/StateMachine.h"
#include "StateMachine/StateMachineGenerator.h"
#include "StateMachine/TableStateMachine.h"
#include "StateMachine/TableStateMachineGenerator.h"
#include "AhoCorasick/ACBuilder.h"
#include "AhoCorasick/ACTypes.h"
#include "Common/BitArray/BitArray.h"

#include "Common/FastLookup/FastLookup.h"
#include "States/LinearEncodedState.h"
#include "States/BitmapEncodedState.h"
#include "States/LookupTableState.h"
#include "States/PathCompressedState.h"

int CompressURLsMachine (char *path,TableStateMachine *machine, int *num_of_urls, Symbol symbolArr[NUM_OF_SYMBOLS], unsigned char results[10000][100]);



#endif /* TABLESTATEMACHINETEST_H_ */
