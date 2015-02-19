#ifndef ACBUILDER_H_
#define ACBUILDER_H_

#include "ACTypes.h"
#include "../Common/Types.h"

void acBuildTree(ACTree *tree, Anchor anchorsArr[], int avoidFailToLeaves, int mixIDs);
void acBuildTreeASCII(ACTree *tree, Anchor anchorsArr[], int numOfAnchors, int avoidFailToLeaves, int mixID);

void acDestroyTreeNodes(ACTree *tree);
Node *acGetNextNode(Node *node, char c);
void acPrintTree(ACTree *tree);
Node *acTransit(Node *node, char c, Node *root);

#endif /* ACBUILDER_H_ */
