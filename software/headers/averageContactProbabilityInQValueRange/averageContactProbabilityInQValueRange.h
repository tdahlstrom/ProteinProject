/*
*	Name: averageContactProbabilityInQValueRange.h
*	Author: Thomas Dahlstrom
*	Date: Mar. 26, 2020
*	Updated: Mar. 26, 2020
*/

#ifndef AVERAGE_CONTACT_PROBABILITY_IN_Q_VALUE_RANGE
#define AVERAGE_CONTACT_PROBABILITY_IN_Q_VALUE_RANGE

struct ContactAverages {
	int focusResidue;
	float averageProbability;
};

struct ContactAverages* calculateAverageContactProbability(int residues, int contacts, struct ContactInformation* residueContactInformation);
struct ContactInformation* sortContacts(int contacts, struct ContactInformation* contactsForSort);
struct ContactInformation* createContactInfoForSort(int contacts, struct ContactInformation *contactInfo);
struct ContactAverages* allocateContactAveragesMemory(int residues);

#endif
