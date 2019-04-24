#include <criterion/criterion.h>
#include <stdio.h>

#include "../software/contactReader/contactReader.h"

Test(contactReader, Test_getAmountOfContacts) {
	int contacts = getAmountOfContacts("./files/contactFile");

	cr_assert_eq(contacts, 426);
}

Test(contactReader, Test_getContactFileContacts) {
	int contacts = getAmountOfContacts("./files/contactFile");

	struct Contact* residueContacts = getContactFileContacts("./files/contactFile");

	cr_assert_eq(residueContacts[0].focusResidue, 1);
	cr_assert_eq(residueContacts[350].focusResidue, 84);

	cr_assert_eq(residueContacts[0].contactResidue, 29);
	cr_assert_eq(residueContacts[350].contactResidue, 88);
}
