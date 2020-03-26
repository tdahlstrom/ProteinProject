/*
*	Name: contactReaderTest.c
*	Author: Thomas Dahlstrom
*	Date: Apr. 15, 2019
*	Updated: Mar. 26, 2020
*/

#include <criterion/criterion.h>
#include <stdio.h>

#include "../software/headers/contactReader/contactReader.h"

Test(contactReader, Test_getAmountOfContacts) {
	int contacts = getAmountOfContacts("./files/contactFile");

	cr_assert_eq(440, contacts);
}

Test(contactReader, Test_getContactFileContacts) {
	int contacts = getAmountOfContacts("./files/contactFile");

	struct Contact* residueContacts = getContactFileContacts("./files/contactFile");

	cr_assert_eq(3, residueContacts[0].focusResidue);
	cr_assert_eq(105, residueContacts[0].contactResidue);

	cr_assert_eq(94, residueContacts[350].focusResidue);
	cr_assert_eq(98, residueContacts[350].contactResidue);
}
