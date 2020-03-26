/*
*	Name: probabilityContactInQValueRange.h
*	Author: Thomas Dahlstrom
*	Date: Mar. 25, 2020
*	Updated: Mar. 26, 2020
*/

#ifndef PROBABILITY_CONTACT_IN_Q_VALUE_RANGE
#define PROBABILITY_CONTACT_IN_Q_VALUE_RANGE

#include "../xtcReader/xtcReader.h"
#include "../contactReader/contactReader.h"

struct ContactInformation {
	int focusResidue;
	int contactResidue;
	int totalOccurrences;
	float probability;
};

struct QRange {
	int low;
	int high;
};

struct TSRange {
	int low;
	int high;
};

struct ContactInformation* calculateContactProbability(int xtcFrames, int contacts, struct XtcCoordinates **xtcResidueCoordinates, char* contactFile, struct QRange qRange, struct TSRange timeRange, int* qValues);
struct ContactInformation* createResidueContactInformation(int contacts, struct Contact *residueContacts);
struct ContactInformation* allocateContactInformationMemory(int contacts);

#endif
