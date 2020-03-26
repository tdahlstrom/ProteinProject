/*
*	Name: probabilityContactInQValueRangeTest.c
*	Author: Thomas Dahlstrom
*	Date: Mar. 25, 2020
*	Updated: Mar. 26, 2020
*/

#include <criterion/criterion.h>
#include <stdio.h>
#include <stdlib.h>

#include "../software/headers/xtcReader/xtcReader.h"
#include "../software/headers/contactReader/contactReader.h"
#include "../software/headers/calcQFromContacts/calcQFromContacts.h"
#include "../software/headers/probabilityContactInQValueRange/probabilityContactInQValueRange.h"

Test(probabilityContactInQValueRangeTest, Test_calculateContactProbability) {
	int frames = getFrames("./files/xtcFile", 163);
	struct XtcCoordinates** xtcCoords = getXtcFileCoordinates("./files/xtcFile", 163, frames);

	int contacts = getAmountOfContacts("./files/contactFile");
	struct Contact* residueContacts = getContactFileContacts("./files/contactFile");

	int *qValues = calculateQValues(frames, contacts, 1.0, xtcCoords, residueContacts);

	struct QRange qRange;
	qRange.low = 300;
	qRange.high = 400;

	struct TSRange timeRange;
	timeRange.low = 0;
	timeRange.high = 20;

	struct ContactInformation *actualResidueContactsInformation =
			calculateContactProbability(frames, contacts, xtcCoords, "./files/contactFile",
					qRange, timeRange, qValues);

	struct ContactInformation *expectedResidueContactsInformation = (struct ContactInformation*) malloc(sizeof(struct ContactInformation) * contacts);

	FILE *file;
	file = fopen("./files/contactInformation", "r");

	for(int i = 0; i < contacts; i++) {
		int tmp;
		fscanf(file, "%i", &tmp);
		fscanf(file, "%i", &expectedResidueContactsInformation[i].focusResidue);
		fscanf(file, "%i", &expectedResidueContactsInformation[i].contactResidue);
		fscanf(file, "%f", &expectedResidueContactsInformation[i].probability);
		fscanf(file, "%i", &expectedResidueContactsInformation[i].totalOccurrences);
	}

	for(int i = 0; i < contacts; i++) {
		if(expectedResidueContactsInformation[i].focusResidue != actualResidueContactsInformation[i].focusResidue) {
			cr_assert_fail("Focused residue incorrect at line: %i\n", contacts + 1);
		}
		if(expectedResidueContactsInformation[i].contactResidue != actualResidueContactsInformation[i].contactResidue) {
			cr_assert_fail("Contact residue incorrect at line: %i\n", contacts + 1);
		}
		if(expectedResidueContactsInformation[i].probability != actualResidueContactsInformation[i].probability) {
			cr_assert_fail("Probability incorrect at line: %i\n", contacts + 1);
		}
		if(expectedResidueContactsInformation[i].totalOccurrences != actualResidueContactsInformation[i].totalOccurrences) {
			cr_assert_fail("Total occurrences incorrect at line: %i\n", contacts + 1);
		}
	}

	cr_assert(true);
}
