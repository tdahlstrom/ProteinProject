/*
*	Name: averageContactProbabilityInQValueRangeTest.c
*	Author: Thomas Dahlstrom
*	Date: Mar. 26, 2020
*	Updated: Mar. 26, 2020
*/

#include <criterion/criterion.h>
#include <stdio.h>
#include <math.h>

#include "../software/headers/xtcReader/xtcReader.h"
#include "../software/headers/contactReader/contactReader.h"
#include "../software/headers/calcQFromContacts/calcQFromContacts.h"
#include "../software/headers/probabilityContactInQValueRange/probabilityContactInQValueRange.h"
#include "../software/headers/averageContactProbabilityInQValueRange/averageContactProbabilityInQValueRange.h"

Test(averageContactProbabilityInQValueRange, Test_contactAverages) {
	int frames = getFrames("./files/xtcFile", 163);
	struct XtcCoordinates** xtcCoords = getXtcFileCoordinates("./files/xtcFile", 163, frames);

	int contacts = getAmountOfContacts("./files/contactFile");
	struct Contact* residueContacts = getContactFileContacts("./files/contactFile");

	float cutOff = 1.0f;

	int *qValues = calculateQValues(frames, contacts, cutOff, xtcCoords, residueContacts);

	struct QRange qRange;
	qRange.low = 300;
	qRange.high = 400;

	struct TSRange timeRange;
	timeRange.low = 0;
	timeRange.high = 20;

	struct ContactInformation *residueContactsInformation =
			calculateContactProbability(frames, contacts, xtcCoords, "./files/contactFile",
					qRange, timeRange, qValues, cutOff);

	struct ContactInformation *sortedContacts =
			sortContacts(contacts, createContactInfoForSort(contacts, residueContactsInformation));

	struct ContactAverages* actualContactAverages =
			calculateAverageContactProbability(163, contacts, sortedContacts);

	struct ContactAverages* expectedContactAverages =
			(struct ContactAverages*) malloc(sizeof(struct ContactAverages) * 163);

	FILE *file;
	file = fopen("./files/contactAverages", "r");

	for(int i = 0; i < 163; i++) {
		fscanf(file, "%i", &expectedContactAverages[i].focusResidue);
		fscanf(file, "%f", &expectedContactAverages[i].averageProbability);
	}

	fclose(file);

	for(int i = 0; i < 163; i++) {
		if(expectedContactAverages[i].focusResidue != actualContactAverages[i].focusResidue)  {
			cr_assert_fail("Focused residue incorrect at line: %i\n", i + 1);
		}

		// Using print commands to round so actual matches expected,
		// this is because expected was created using print commands
		char expectedProbability[10], actualProbability[10];
		sprintf(actualProbability, "%.6f", actualContactAverages[i].averageProbability);
		sscanf(actualProbability, "%f", &actualContactAverages[i].averageProbability);

		if(expectedContactAverages[i].averageProbability != actualContactAverages[i].averageProbability) {
			cr_assert_fail("Average Probability incorrect at line: %i\n", i + 1);
		}
	}

	cr_assert(true);
}
