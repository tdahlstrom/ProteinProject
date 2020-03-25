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

/*
*	Name: int getAmountOfContacts()
*	Description:	Reads the amount of contacts, from the gro file.
*
*	Args: -char *contactfile - location of the contacts file.
*
*	Returns: -int contacts - the amount of contacts.
*/

int getAmountOfContacts(char *contactfile) {
	int contacts, iTemp;
	FILE *fp;

	// Opens file
	if ((fp = fopen(contactfile, "r")) == NULL) {
		perror("could not open contactFile for getAmountOfContacts().");
		exit(1);
	}

	// Reads number of contacts and omits other value
	fscanf(fp, "%i", &contacts);
	fscanf(fp, "%i", &iTemp);

	// Closes file
	fclose(fp);

	return contacts;
}

/*
*	Name: struct Contact* getContactfileContacts()
*	Description:	Copies all of the contact pairs from the gro file.
*
*	Args: -char *contactfile - location of the contacts file.
*
*	Returns: -struct Contact *residueContacts - contains contact pairs.
*/

struct Contact* getContactFileContacts(char *contactfile) {
	struct Contact *residueContacts;
	int contacts, iTemp;
	FILE *fp;

	// Opens file
	if ((fp = fopen(contactfile, "r")) == NULL) {
		perror("could not open contactFile for getContactFileContacts().");
		exit(1);
	}

	// Reads number of contacts and omits other value
	fscanf(fp, "%i", &contacts);
	fscanf(fp, "%i", &iTemp);

	// Allocate memory for the coordinates from the contactfile
	residueContacts = (struct Contact*) malloc(sizeof(struct Contact) * contacts);
	if(!residueContacts) {
		perror("contactFile memory not allocated");
		abort();
	}

	// Reads the focused residue and contact residue while omitting other values
	for(int i = 0; i < contacts; i++) {
		fscanf(fp, "%i", &iTemp);
		fscanf(fp, "%i", &residueContacts[i].focusResidue);
		fscanf(fp, "%i", &iTemp);
		fscanf(fp, "%i", &residueContacts[i].contactResidue);
	}	

	// Closes file
	fclose(fp);

	return residueContacts;
}
