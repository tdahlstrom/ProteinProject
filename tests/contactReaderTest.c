#include <criterion/criterion.h>
#include <stdio.h>

#include "../software/contactReader/contactReader.h"

Test(contactReader, Test_getAmountOfContacts_pass) {
	int contacts = getAmountOfContacts("./files/contactFile");

	cr_expect_eq(contacts, 426);
}

Test(contactReader, Test_getContactFileContacts_pass) {
	int contacts = getAmountOfContacts("./files/contactFile");

	struct Contact* residueContacts = getContactFileContacts("./files/contactFile");

	cr_expect_eq(residueContacts[0].focusResidue, 1);
	cr_expect_eq(residueContacts[350].focusResidue, 84);

	cr_expect_eq(residueContacts[0].contactResidue, 29);
	cr_expect_eq(residueContacts[350].contactResidue, 88);
}
