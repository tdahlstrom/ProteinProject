/*
*	Name: probabilityContactInQValueRange.c
*	Author: Thomas Dahlstrom
*	Date: Dec. 02, 2017
*	Updated: Dec. 28, 2018
*
*	Summary of expected functionality:
*		Given a trajectory file, contact file, and a range of Q values, the program will
*		list all of the contacts in the contact file alongside the probability of the
*		contact occurring when the Q value of the frame where the contact occurs is within
*		the specified range and the amount of times the contact occurs.  At the top of the
*		previous list, the program will display the amount of frames that had a Q value
*		within the specified range.  Also a range of time slices is provided to narrow the
*		search.
*
*	Compile example:
*		gcc -o probabilityContactInQValueRange probabilityContactInQValueRange.c xdrfile.c xdrfile_xtc.c -lm
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "headers/xtcReader/xtcReader.h"
#include "headers/contactReader/contactReader.h"
#include "headers/calcQFromContacts/calcQFromContacts.h"
#include "headers/probabilityContactInQValueRange/probabilityContactInQValueRange.h"

int main(int argc, char *argv[]) {
	char *xtcfile = argv[5];
	char *contactFile = argv[6];

	struct QRange qRange;
	struct TSRange timeRange;

	qRange.low = atoi(argv[1]), qRange.high = atoi(argv[2]);
	timeRange.low = atoi(argv[3]), timeRange.high = atoi(argv[4]);

	float cutoff = atof(argv[7]);
	int residues = atoi(argv[8]);
	int *qValues, xtcFrames, contacts;

	struct XtcCoordinates **xtcResidueCoordinates;
	struct Contact *residueContacts;
	struct ContactInformation *residueContactsInformation;

	// Counts amount of frames from the traj.xtc file
	xtcFrames = getFrames(xtcfile, residues);

	// Copies all the residue coordinates for each frame of the traj.xtc file into memory
	xtcResidueCoordinates = getXtcFileCoordinates(xtcfile, residues, xtcFrames);

	// Reads the amount of contacts from the contact file
	contacts = getAmountOfContacts(contactFile);

	// Copies all the residue contacts from the contact file
	residueContacts = getContactFileContacts(contactFile);

	// Creates Q values for every frame in the traj.xtc file
	qValues = calculateQValues(xtcFrames, contacts, cutoff, xtcResidueCoordinates, residueContacts);

	// Records total occurrences of each contact and calculates the probability of the contact occuring
	residueContactsInformation = calculateContactProbability(xtcFrames, contacts, xtcResidueCoordinates, contactFile, qRange, timeRange, qValues);

	// Output for all information on that contacts
	for(int i = 0; i < contacts; i++) {
		//printf("%d - focusedResidue: %d\tcontactResidue: %d\tprobability: %f\toccurrences: %d\n", i+1, residueContactsInformation[i].focusResidue, residueContactsInformation[i].contactResidue, residueContactsInformation[i].probability, residueContactsInformation[i].totalOccurrences);
		printf("%d %d %d %f %d\n", i+1, residueContactsInformation[i].focusResidue, residueContactsInformation[i].contactResidue, residueContactsInformation[i].probability, residueContactsInformation[i].totalOccurrences);
	}

	return 0;
}


