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

#include "headers/xtcReader/xtcReader.h"
#include "headers/contactReader/contactReader.h"
#include "headers/calcQFromContacts.h"

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


