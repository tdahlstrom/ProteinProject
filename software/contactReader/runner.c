/*
*	Name: runner.c
*	Author: Thomas Dahlstrom
*	Date: Nov. 02, 2017
*	Updated: Nov. 02, 2017
*
*	Summary of expected functionality:
*		Provides an example of how use contactReader.h.
*/

#include <stdio.h>
#include <stdlib.h>

#include "contactReader.h"

int main(int argc, char *argv[]) {
	char *groFile = "/home/tdahlstrom/Research/4FAK.19182.pdb.sb/4FAK.19182.pdb.contacts";
	struct Contact *residueContacts;

	int numberOfContacts = getAmountOfContacts(groFile);

	residueContacts = getContactFileContacts(groFile);

	printf("number of contacts: %d\n", numberOfContacts);

	for(int i = 0; i < numberOfContacts; i++) {
		printf("focusedResidue: %d\tcontactResidue: %d\n", residueContacts[i].focusResidue, residueContacts[i].contactResidue);
	}

	return 0;
}
