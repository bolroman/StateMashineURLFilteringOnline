#ifndef STATEMACHINEGENERATOR_H_
#define STATEMACHINEGENERATOR_H_

#include "StateMachine.h"

StateMachine *createStateMachine(Anchor* anchorsArr, int numOfAnchors, int maxGotosLE, int maxGotosBM, int verbose);
StateMachine *createSimpleStateMachine(const char *path, int maxGotosLE, int maxGotosBM, int verbose);
void destroyStateMachine(StateMachine *machine);

#endif /* STATEMACHINEGENERATOR_H_ */
