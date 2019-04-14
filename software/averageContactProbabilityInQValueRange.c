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

#include "/home/thomas/Research/Software/xtcReader/xtcReader.h"
#include "/home/thomas/Research/Software/contactReader/contactReader.h"

struct ContactInformation {
	int focusResidue;
	int contactResidue;
	int totalOccurrences;
	float probability;
};

struct ContactAverages {
	int focusResidue;
	float averageProbability;
};

struct QRange {
	int low;
	int high;
};

struct TSRange {
	int low;
	int high;
};

struct ContactAverages* calculateAverageContactProbability(int residues, int contacts, struct ContactInformation* residueContactInformation);
struct ContactInformation* sortContacts(int contacts, struct ContactInformation* contactsForSort);
struct ContactInformation* createContactInfoForSort(int contacts, struct ContactInformation *contactInfo);
struct ContactInformation* calculateContactProbability(int xtcFrames, int contacts, struct XtcCoordinates **xtcResidueCoordinates, char* contactFile, struct QRange qRange, struct TSRange timeRange, int* qValues);
int* calculateQValues(int xtcFrames, int contacts, struct XtcCoordinates **xtcResidueCoordinates, struct Contact *residueContacts);
float calculateDistance(struct XtcCoordinates coords1, struct XtcCoordinates coords2);
int* allocateQValuesMemory(int xtcFrames);
struct ContactAverages* allocateContactAveragesMemory(int residues);
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
	qValues = calculateQValues(xtcFrames, contacts, xtcResidueCoordinates, residueContacts);

	// Records total occurrences of each contact and calculates the probability of the contact occurring
	residueContactsInformation = calculateContactProbability(xtcFrames, contacts, xtcResidueCoordinates, contactFile, qRange, timeRange, qValues);

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
			if((qValues[i] >= qRange.low) && (qValues[i] <= qRange.high)) {

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
*	Returns: -float - distance between the two residues.
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
