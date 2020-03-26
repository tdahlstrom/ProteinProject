/*
*	Name: probabilityContactInQValueRange.c
*	Author: Thomas Dahlstrom
*	Date: Mar. 25, 2020
*	Updated: Mar. 26, 2020
*/

#include <stdio.h>
#include <stdlib.h>

#include "../xtcReader/xtcReader.h"
#include "../contactReader/contactReader.h"
#include "../calcQFromContacts/calcQFromContacts.h"
#include "probabilityContactInQValueRange.h"

/*
*	Name: struct ContactInformation* calculateContactProbability()
*	Description: Given an array of Q values, and a range of Q values, the function
*		     will determine the probability of a contact occurring when the Q value
*		     of a frame is within the specified range.
*
*	Args: -int xtcFrames - the amount of frame in the traj.xtc file
*	      -int contacts - the amount of contacts in the contactFile
*	      -struct XtcCoordinates **xtcResidueCoordinates - the residue coordinates from the
*				traj.xtc file
*	      -char* contactFile - location of the contactFile
*	      -struct QRange qRange - low and high range of Q values
*	      -struct TSRange timeRange - low and high range of time slices
*	      -int* qValues - an array of Q values; every frame of the traj.xtc has one Q value
*
*	Returns: -struct ContactInformation* residueContactsInformation - an array of structures
*				containing values for every variable
*/

struct ContactInformation* calculateContactProbability(int xtcFrames, int contacts, struct XtcCoordinates **xtcResidueCoordinates, char* contactFile, struct QRange qRange, struct TSRange timeRange, int* qValues) {

	// Populates an array of struct ContactInformation; size being the amount of contacts
	struct ContactInformation* residueContactsInformation = createResidueContactInformation(contacts, getContactFileContacts(contactFile));
	int qValuesInRange = 0;

	// For every frame in the traj.xtc file; also the total amount of Q values
	for(int i = 0; i < xtcFrames; i++) {

		// If the time step is within the defined range
		if((i >= (timeRange.low-1)) && (i <= (timeRange.high-1))) {

			// If the Q value is within the defined range
			if(qValues[i] >= qRange.low && qValues[i] <= qRange.high) {

				// For every contact pair in the contact file
				for(int j = 0; j < contacts; j++) {

					// If the distance between the residue pair is within the cutoff value
					if(calculateDistance(xtcResidueCoordinates[i][residueContactsInformation[j].focusResidue-1],
					   xtcResidueCoordinates[i][residueContactsInformation[j].contactResidue-1]) <= 1.2) {

						// Increase the total occurrences of the contact by 1
						residueContactsInformation[j].totalOccurrences++;
					}
				}

				qValuesInRange++;
			}
		}
	}

	// For every contact pair
	for(int i = 0; i < contacts; i++) {
		if(qValuesInRange == 0) {
			residueContactsInformation[i].probability = 0;
		} else {
			// Calculate the probability of the contact occurring
			residueContactsInformation[i].probability = (float)residueContactsInformation[i].totalOccurrences / (float)qValuesInRange;
		}
	}

	return residueContactsInformation;
}

/*
*	Name: struct ContactInformation* createResidueContactInformation()
*	Description: Inputs the contacts from the contact file into an array of struct
*		     ContactInformation.
*	Args: -int contacts - total amount of contacts from the contactFile
*	      -struct Contact *residueContacts - contact pairs from the contactFile
*
*	Returns: -struct ContactInformation *residueContactsInformation - an array of struct
*		 		ContactInformation, containing contacts from the contactFile
*/

struct ContactInformation* createResidueContactInformation(int contacts, struct Contact *residueContacts) {
	struct ContactInformation *residueContactsInformation = allocateContactInformationMemory(contacts);

	for(int i = 0; i < contacts; i++) {
		residueContactsInformation[i].focusResidue = residueContacts[i].focusResidue;
		residueContactsInformation[i].contactResidue = residueContacts[i].contactResidue;
	}

	return residueContactsInformation;
}

struct ContactInformation* allocateContactInformationMemory(int contacts) {
	struct ContactInformation *residueContactsInformation;

	residueContactsInformation = (struct ContactInformation*) malloc(sizeof(struct ContactInformation) * contacts);
	if(!residueContactsInformation) {
		perror("residueContactsInformation memory not allocated");
		abort();
	}

	for(int i = 0; i < contacts; i++) {
		residueContactsInformation[i].totalOccurrences = 0;
	}

	return residueContactsInformation;
}
