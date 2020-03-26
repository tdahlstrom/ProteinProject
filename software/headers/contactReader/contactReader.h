/*
*	Name: contactReader.h
*	Author: Thomas Dahlstrom
*	Date: Nov. 02, 2017
*	Updated: Mar. 26, 2020
*/

#ifndef CONTACT_READER
#define CONTACT_READER

struct Contact {
	int focusResidue;
	int contactResidue;
};


int getAmountOfContacts(char *contactfile);
struct Contact* getContactFileContacts(char *contactfile);

#endif
