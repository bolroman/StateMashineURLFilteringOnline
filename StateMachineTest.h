#ifndef STATEMACHINETEST_H_
#define STATEMACHINETEST_H_

#include "CommonFunctions.h"
#include "StateMachine/StateMachine.h"

#include "AhoCorasick/ACBuilder.h"
#include "StateMachine/TableStateMachine.h"
#include "StateMachine/TableStateMachineGenerator.h"
#include "AhoCorasick/ACTypes.h"
#include "Common/BitArray/BitArray.h"

#include "Common/FastLookup/FastLookup.h"
#include "StateMachine/StateMachineGenerator.h"
#include "States/LinearEncodedState.h"
#include "States/BitmapEncodedState.h"
#include "States/LookupTableState.h"
#include "States/PathCompressedState.h"


int CompressURLsStateMachine (char *path,StateMachine *machine, int *num_of_urls, Symbol symbolArr[NUM_OF_SYMBOLS], unsigned char results[10000][100]);




#endif /* STATEMACHINETEST_H_ */
