/*
*	Name: calcQFromContacts.c
*	Author: Thomas Dahlstrom
*	Date: Nov. 01, 2017
*	Updated: Nov. 02, 2017
*
*	Summary of expected functionality:
*		Reads contacts from a provided contacts file.  Then, reads the traj.xtc file.
*		For every frame, the residue position data will be referenced when calculating
*		if a contact is present.  A contact is found by referencing the contact file.
*		The contact file contains a list of the contacts that are checked.  So, a frame
*		will only check the residue position data of a contact present in the contact file.
*		The position data of a contact will be used to calculate if the residue pair is
*		within a cutoff distance.  If the residue pair is within the cutoff value, it will
*		be considered a contact, and increase the Q value of the frame by 1.  The Q value
*		of a frame is the amount of contacts present in the frame.
*		
*	Compile example:
*		gcc -o calcQFromContacts calcQFromContacts.c xdrfile.c xdrfile_xtc.c -lm
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "/home/tdahlstrom/Research/Software/xtcReader/xtcReader.h"
#include "/home/tdahlstrom/Research/Software/contactReader/contactReader.h"

void writeQFile(int *qValues, int xtcFrames, char *qFile);
int* calculateQValues(int xtcFrames, int contacts, float cutoff, struct XtcCoordinates **xtcResidueCoordinates, struct Contact *residueContacts);
float calculateDistance(struct XtcCoordinates coords1, struct XtcCoordinates coords2);
int* allocateQValuesMemory(int xtcFrames);

int main(int argc, char *argv[]) {
	char *xtcfile = argv[3];
	char *contactFile = argv[4];
	char *qFile = argv[5];

	float cutoff = atof(argv[2]);
	int residues = atoi(argv[1]);
	int *qValues, xtcFrames, contacts;

	struct XtcCoordinates **xtcResidueCoordinates;
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
	qValues = calculateQValues(xtcFrames, contacts, cutoff, xtcResidueCoordinates, residueContacts);
	
	// Creates qFile of the Q values
	writeQFile(qValues, xtcFrames, qFile);

	return 0;
}

/*
*	Name: void writeQFile()
*	Description: Creates a file with a Q value on every line.
*
*	Args: -int *qValues - a Q value for each frame of the traj.xtc.
*	      -int xtcFrames - the amount of frames in the traj.xtc file.
*	      -char *qFile - the file name where Q values will be written.
*/

void writeQFile(int *qValues, int xtcFrames, char *qFile) {
	FILE *fp;	

	// Creates the Q file
	if ((fp = fopen(qFile, "w")) == NULL) {
		perror("could not open qFile for output.");
		exit(1);
	}

	// Writes the Q values to the file
	for(int i = 0; i < xtcFrames; i++) {
		fprintf(fp, "%i\n", qValues[i]);
	}

	// Closes the Q file
	fclose(fp);
}

/*
*	Name: int* calculateQValues()
*	Description: Calculates the Q values of every frames from the traj.xtc.  Uses the contacts
*		     from the contact file and measures if contact is present via a cutoff value.
*
*	Args: -int xtcFrames - the amount of frames in the traj.xtc file.
*	      -int contacts - the amount of contacts in the contacts file.
*	      -float cutoff - the user specified contact cutoff value for a residue pair.
*	      -struct XtcCoordinates **xtcResidueCoordinates - frame data and residue coordinates from
*							       the traj.xtc file.
*	      -struct Contact *residueContacts - all of the residue pairs from the contacts file.
*
*	returns: -int *qValues - a Q value for each frame of the traj.xtc.
*/

int* calculateQValues(int xtcFrames, int contacts, float cutoff, struct XtcCoordinates **xtcResidueCoordinates, struct Contact *residueContacts) {
	int *qValues = allocateQValuesMemory(xtcFrames);
	
	// For every frame in the traj.xtc file	
	for(int i = 0; i < xtcFrames; i++) {
	
		// For every contact pair in the contact file
		for(int j = 0; j < contacts; j++) {

			// If the distance between the residue pair is within the cutoff value
			if(calculateDistance(xtcResidueCoordinates[i][residueContacts[j].focusResidue-1],
			   xtcResidueCoordinates[i][residueContacts[j].contactResidue-1]) <= cutoff) {
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
