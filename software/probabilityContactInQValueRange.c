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

#include "/home/thomas/Research/Software/xtcReader/xtcReader.h"
#include "/home/thomas/Research/Software/contactReader/contactReader.h"

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

int* calculateQValues(int xtcFrames, int contacts, struct XtcCoordinates **xtcResidueCoordinates, struct Contact *residueContacts);
float calculateDistance(struct XtcCoordinates coords1, struct XtcCoordinates coords2);
int* allocateQValuesMemory(int xtcFrames);
struct ContactInformation* calculateContactProbability(int xtcFrames, int contacts, struct XtcCoordinates **xtcResidueCoordinates, char* contactFile, struct QRange qRange, struct TSRange timeRange, int* qValues);
struct ContactInformation* createResidueContactInformation(int contacts, struct Contact *residueContacts);
struct ContactInformation* allocateContactInformationMemory(int contacts);

int main(int argc, char *argv[]) {
	char *xtcfile = argv[5];
	char *contactFile = argv[6];

	struct QRange qRange;
	struct TSRange timeRange;

	qRange.low = atoi(argv[1]), qRange.high = atoi(argv[2]);
	timeRange.low = atoi(argv[3]), timeRange.high = atoi(argv[4]);
	int residues = 147;
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
	qValues = calculateQValues(xtcFrames, contacts, xtcResidueCoordinates, residueContacts);

	// Records total occurrences of each contact and calculates the probability of the contact occuring
	residueContactsInformation = calculateContactProbability(xtcFrames, contacts, xtcResidueCoordinates, contactFile, qRange, timeRange, qValues);

	// Output for all information on that contacts
	for(int i = 0; i < contacts; i++) {
		//printf("%d - focusedResidue: %d\tcontactResidue: %d\tprobability: %f\toccurrences: %d\n", i+1, residueContactsInformation[i].focusResidue, residueContactsInformation[i].contactResidue, residueContactsInformation[i].probability, residueContactsInformation[i].totalOccurrences);
		printf("%d %d %d %f %d\n", i+1, residueContactsInformation[i].focusResidue, residueContactsInformation[i].contactResidue, residueContactsInformation[i].probability, residueContactsInformation[i].totalOccurrences);
	}

	return 0;
}

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

	// This is here for simplicity of logic
	printf("Q Values In Range: %d\n", qValuesInRange);

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
*	Name: int* calculateQValues()
*	Description: Calculates the Q value of every frame from the traj.xtc.  Uses the contacts
*		     from the contact file and measures if contact is present via a cutoff value.
*
*	Args: -int xtcFrames - the amount of frames in the traj.xtc file.
*	      -int contacts - the amount of contacts in the contacts file.
*	      -struct XtcCoordinates **xtcResidueCoordinates - frame data and residue coordinates from
*							       the traj.xtc file.
*	      -struct Contact *residueContacts - all of the residue pairs from the contacts file.
*
*	returns: -int *qValues - a Q value for each frame of the traj.xtc.
*/

int* calculateQValues(int xtcFrames, int contacts, struct XtcCoordinates **xtcResidueCoordinates, struct Contact *residueContacts) {
	int *qValues = allocateQValuesMemory(xtcFrames);
	
	// For every frame in the traj.xtc file	
	for(int i = 0; i < xtcFrames; i++) {
	
		// For every contact pair in the contact file
		for(int j = 0; j < contacts; j++) {

			// If the distance between the residue pair is within the cutoff value
			if(calculateDistance(xtcResidueCoordinates[i][residueContacts[j].focusResidue-1],
			   xtcResidueCoordinates[i][residueContacts[j].contactResidue-1]) <= 1.2) {
				qValues[i]++;
			}
		}
	}

	return qValues;
}

/*
*	Name: float calculateDistance()
*	Description: Calculates the distance between the residue pair.
*
*	Args: -struct XtcCoordinates coords1 - coordinates of the focused residue.
*	      -struct XtcCoordinates coords2 - coordinates of the residue potentially in contact
*					       with the focused residue.
*
*	returns: -float - distance between the two residues.
*/

float calculateDistance(struct XtcCoordinates coords1, struct XtcCoordinates coords2) {
	return (float) sqrt(pow(coords2.x - coords1.x, 2) + pow(coords2.y - coords1.y, 2) + pow(coords2.z - coords1.z, 2));
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

int* allocateQValuesMemory(int xtcFrames) {
	int *qValues;

	qValues = (int*) malloc(sizeof(int) * xtcFrames);
	if(!qValues) {
		perror("qValues memory not allocated");
		abort();
	}

	for(int i = 0; i < xtcFrames; i++) {
		qValues[i] = 0;
	}

	return qValues;
}
