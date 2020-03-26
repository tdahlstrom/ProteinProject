#include <stdio.h>
#include <stdlib.h>

#include "../probabilityContactInQValueRange/probabilityContactInQValueRange.h"
#include "averageContactProbabilityInQValueRange.h"

/*
*	Name: struct ContactAverages* calculateAverageContactProbability()
*	Description: Given a struct of ContactInformation; the probability that the residues of a protein
*		     are part of a contact is calculated.  Every contact has a probability, these probabilities
*		     are listed in a struct of ContactInformation.  Each contact where the residue (not the
*		     residue contact/contact residue) is the same, the probabilities of these contacts will be
*		     averaged together.  After averaging, each residue of the protein will have a probability.
*		     This probability will indicate how often the residue is in contact with another residue
*		     within the protein.
*
*	Args: -int residues - the amount of residues in the protein
*	      -int contacts - the amount of contacts in the contactFile
*	      -struct ContactInformation* residueContactInformation - information gathered and processed; from
*				the contact file
*
*	Returns: -struct ContactAverages* contactAverages - the probability that a specific residue is part of
*				a contact
*/

struct ContactAverages* calculateAverageContactProbability(int residues, int contacts, struct ContactInformation* residueContactInformation) {
	struct ContactAverages* contactAverages = allocateContactAveragesMemory(residues);

	float tempAvgTotal = 0.0;
	float occurances = 1;

	for(int i = 0; i < contacts*2; i++) {
		tempAvgTotal = residueContactInformation[i].probability;
		occurances = 1;

		while((i+1 < contacts*2) && (residueContactInformation[i].focusResidue == residueContactInformation[i+1].focusResidue)) {
			tempAvgTotal += residueContactInformation[++i].probability;
			occurances++;
		}

		contactAverages[residueContactInformation[i].focusResidue-1].averageProbability = tempAvgTotal / occurances;
	}

	return contactAverages;
}

/*
*	Name: struct ContactInformation* sortContacts()
*	Description: Sorts the contacts and their reverse, ascending, by focusResidue.  Uses the
*			 insertion sort algorithm.  Sorting by focusResidue is required for
*			 calculateAverageContactProbability().
*
*	Args: -int contacts - the amount of contacts in the contactFile
*		  -struct ContactInformation *contactsForSort - the contact pairs and their reverse
*
*   Returns: struct ContactInformation* contactsForSort - contact pairs and their reverse,
*   			  sorted by focusResidue
*
*/

struct ContactInformation* sortContacts(int contacts, struct ContactInformation* contactsForSort) {
	int n = contacts*2;

	for(int i = 1,j; i < n; i++) {
		struct ContactInformation tmp;
		tmp.focusResidue = contactsForSort[i].focusResidue;
		tmp.contactResidue = contactsForSort[i].contactResidue;
		tmp.totalOccurrences = contactsForSort[i].totalOccurrences;
		tmp.probability = contactsForSort[i].probability;

		for(j = i; (j > 0) && (tmp.focusResidue < contactsForSort[j-1].focusResidue); j--) {
			contactsForSort[j].focusResidue = contactsForSort[j-1].focusResidue;
			contactsForSort[j].contactResidue = contactsForSort[j-1].contactResidue;
			contactsForSort[j].totalOccurrences = contactsForSort[j-1].totalOccurrences;
			contactsForSort[j].probability = contactsForSort[j-1].probability;
		}

		contactsForSort[j].focusResidue = tmp.focusResidue;
		contactsForSort[j].contactResidue = tmp.contactResidue;
		contactsForSort[j].totalOccurrences = tmp.totalOccurrences;
		contactsForSort[j].probability = tmp.probability;
	}

	return contactsForSort;
}

/*
*	Name: struct ContactInformation* createContactInfoForSort()
*	Description: Given a struct of ContactInformation, the reverse of the contacts are
*			 added to the end of the contacts.  This increases the size to twice that
*			 of the total contacts.
*
*	Args: -int contacts - the amount of contacts in the contactFile
*		  -struct ContactInformation *contactInfo - the contact pairs, their occurrences and
*		        probabilities
*
*	Returns: -struct ContactInformation *contactsForSort - the contact pairs and their reverse
*/

struct ContactInformation* createContactInfoForSort(int contacts, struct ContactInformation *contactInfo) {
	struct ContactInformation *contactsForSort = allocateContactInformationMemory(contacts*2);

	int i;
	for(i = 0; i < contacts; i++) {
		contactsForSort[i].focusResidue = contactInfo[i].focusResidue;
		contactsForSort[i].contactResidue = contactInfo[i].contactResidue;
		contactsForSort[i].totalOccurrences = contactInfo[i].totalOccurrences;
		contactsForSort[i].probability = contactInfo[i].probability;
	}

	for(int j = 0; i < contacts*2; i++,j++) {
		contactsForSort[i].focusResidue = contactInfo[j].contactResidue;
		contactsForSort[i].contactResidue = contactInfo[j].focusResidue;
		contactsForSort[i].totalOccurrences = contactInfo[j].totalOccurrences;
		contactsForSort[i].probability = contactInfo[j].probability;
	}

	return contactsForSort;
}

struct ContactAverages* allocateContactAveragesMemory(int residues) {
	struct ContactAverages* contactAverages;

	contactAverages = (struct ContactAverages*) malloc(sizeof(struct ContactAverages) * residues);
	if(!contactAverages) {
		perror("contactAverages memory not allocated");
		abort();
	}

	for(int i = 0; i < residues; i++) {
		contactAverages[i].focusResidue = i;
		contactAverages[i].averageProbability = -1.0;
	}

	return contactAverages;
}
