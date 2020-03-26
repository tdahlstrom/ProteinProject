#ifndef CALC_Q_FROM_CONTACTS
#define CALC_Q_FROM_CONTACTS

#include "../xtcReader/xtcReader.h"
#include "../contactReader/contactReader.h"

void writeQFile(int *qValues, int xtcFrames, char *qFile);
int* calculateQValues(int xtcFrames, int contacts, float cutoff, struct XtcCoordinates **xtcResidueCoordinates, struct Contact *residueContacts);
float calculateDistance(struct XtcCoordinates coords1, struct XtcCoordinates coords2);
int* allocateQValuesMemory(int xtcFrames);

#endif
