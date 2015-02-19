/*
 * DictionaryReader.h
 *
 *  Created on: Jan 24, 2013
 *      Author: yotam
 */

#ifndef DICTIONARYREADER_H_
#define DICTIONARYREADER_H_

#include "Dictionary.h"
#include "../StateMachine/TableStateMachine.h"

int dictionaryreader_read(const char *path, Dictionary *dict, RollingHashTool *hasher, TableStateMachine *machine);

#endif /* DICTIONARYREADER_H_ */
