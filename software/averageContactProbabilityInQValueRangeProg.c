/*
*	Name: averageContactProbabilityInQValueRange.c
*	Author: Thomas Dahlstrom
*	Date: Jan. 21, 2018
*	Updated: Dec. 28, 2018
*
*	Summary of expected functionality:
*		A trajectory will be analysed against a contact file to determine the probability of a
*		contact (a residue and the residue it is in contact with) being present.  These probabilities
*		will then be averaged together to determine the probability that a residue is part of any
*		contact.  A range of Q values will be given.  A range of time slice values will be given.
*		
*
*	Compile example:
*		gcc -o averageContactProbabilityInQValueRange averageContactProbabilityInQValueRange.c xdrfile.c xdrfile_xtc.c -lm
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "headers/xtcReader/xtcReader.h"
#include "headers/contactReader/contactReader.h"
#include "headers/calcQFromContacts/calcQFromContacts.h"
#include "headers/probabilityContactInQValueRange/probabilityContactInQValueRange.h"
#include "headers/averageContactProbabilityInQValueRange/averageContactProbabilityInQValueRange.h"

int main(int argc, char *argv[]) {
	char *xtcfile = argv[5];
	char *contactFile = argv[6];

	struct QRange qRange;
	struct TSRange timeRange;

	qRange.low = atoi(argv[1]), qRange.high = atoi(argv[2]);
	timeRange.low = atoi(argv[3]), timeRange.high = atoi(argv[4]);

	int residues = atoi(argv[7]);
	float cutOff = atof(argv[8]);

	int *qValues, xtcFrames, contacts;

	struct XtcCoordinates **xtcResidueCoordinates;
	struct ContactInformation *residueContactsInformation, *sortedContacts;
	struct ContactAverages* contactAverages;
	struct Contact *residueContacts;

	// Counts amount of frames from the traj.xtc file
	xtcFrames = getFrames(xtcfile, residues);

	// Copies all the residue coordinates for each frame of the traj.xtc file into memory
	xtcResidueCoordinates = getXtcFileCoordinates(xtcfile, residues, xtcFrames);

	// Reads the amount of contacts from the contact file
	contacts = getAmountOfContacts(contactFile);

	// Copies all the residue contacts from the contact file
	residueContacts = getContactFileContacts(contactFile);

	// Creates Q values for every frame in the traj.xtc file
	qValues = calculateQValues(xtcFrames, contacts, cutOff, xtcResidueCoordinates, residueContacts);

	// Records total occurrences of each contact and calculates the probability of the contact occurring
	residueContactsInformation = calculateContactProbability(xtcFrames, contacts, xtcResidueCoordinates, contactFile, qRange, timeRange, qValues, cutOff);

	// Copies, reverses, and appended the copy to the end of the contacts, then sorts the
	// contacts by the first of two contacts in the pair
	sortedContacts = sortContacts(contacts, createContactInfoForSort(contacts, residueContactsInformation));

	// Calculates the average of all probabilities per residue
	contactAverages = calculateAverageContactProbability(residues, contacts, sortedContacts);

	for(int i = 0; i < residues; i++) {
		if(contactAverages[i].averageProbability != -1.0) {
			//printf("Residue: %d Probability: %f\n", contactAverages[i].focusResidue+1, contactAverages[i].averageProbability);
			printf("%d %f\n", contactAverages[i].focusResidue+1, contactAverages[i].averageProbability);
		} else {
			//printf("Residue: %d Probability: N/A\n", contactAverages[i].focusResidue+1);
			printf("%d N/A\n", contactAverages[i].focusResidue+1);
		}
	}

	return 0;
}
