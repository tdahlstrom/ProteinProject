#ifndef CONTACT_READER
#define CONTACT_READER

/*
*	Name: contactReader.h
*	Author: Thomas Dahlstrom
*	Date: Nov. 02, 2017
*	Updated: Nov. 02, 2017
*
*	Summary of expected functionality:
*		Provides functions to read a contact file created from http://smog-server.org/.
*	Notes:
*		The residue pairs in the contact file start at a value of 1.  So, residue 1 in
*		the contact file is the first residue in the polypeptide.
*/

struct Contact {
	int focusResidue;
	int contactResidue;
};


int getAmountOfContacts(char *contactfile);
struct Contact* getContactFileContacts(char *contactfile);

#endif
