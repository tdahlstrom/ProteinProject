/*
*	Name: calcQFromContactsTest.c
*	Author: Thomas Dahlstrom
*	Date: Mar. 24, 2020
*	Updated: Mar. 26, 2020
*/

#include <criterion/criterion.h>
#include <stdio.h>
#include <math.h>

#include "../software/headers/xtcReader/xtcReader.h"
#include "../software/headers/contactReader/contactReader.h"
#include "../software/headers/calcQFromContacts/calcQFromContacts.h"

int getLinesInFile(char* fileName);
int* getContentsOfFile(char* fileName, int size);

void cleanUp() {
	remove("qFile");
}

Test(calcQFromContacts, Test_calculateDistance) {
	struct XtcCoordinates coords1, coords2;
	coords1.x = 0.0;
	coords1.y = 0.0;
	coords1.z = 0.0;
	coords2.x = 3.0;
	coords2.y = 3.0;
	coords2.z = 3.0;

	float distance = calculateDistance(coords1, coords2);
	distance = floor(100000*distance)/100000;

	cr_assert_eq(5.19615F, distance);
}

Test(calcQFromContacts, Test_writeQFile, .fini = cleanUp) {
	int frames = getFrames("./files/xtcFile", 163);
	struct XtcCoordinates** xtcCoords = getXtcFileCoordinates("./files/xtcFile", 163, frames);

	int contacts = getAmountOfContacts("./files/contactFile");
	struct Contact* residueContacts = getContactFileContacts("./files/contactFile");

	int *qValues = calculateQValues(frames, contacts, 1.0f, xtcCoords, residueContacts);

	writeQFile(qValues, frames, "qFile");

	int linesActual = getLinesInFile("qFile");
	int* contactsActual = getContentsOfFile("qFile", linesActual);

	int linesExpected = getLinesInFile("./files/qFile");
	int* contactsExpected = getContentsOfFile("./files/qFile", linesExpected);

	if(sizeof(contactsExpected) != sizeof(contactsActual)) {
		cr_assert_fail("Files are not equal in length.\n");
	}

	cr_assert_eq(sizeof(contactsExpected), sizeof(contactsActual));
	for(int i = 0; i < linesActual; i++) {
		if(contactsExpected[i] != contactsActual[i]){
			cr_assert_fail("Values at line: %i is different.\n", i+1);
		}
	}

	cr_assert(true);
}

int getLinesInFile(char* fileName) {
	char line[20];
	FILE *file;
	file = fopen(fileName, "r");

	int numberOfLines = 0;

	while(fgets(line, 100, file) != NULL) {
		numberOfLines++;
	}
	fclose(file);

	return numberOfLines;
}

int* getContentsOfFile(char* fileName, int size) {
	char line[20];
	FILE *file;

	int *contentsOfFile = (int*) malloc(sizeof(int) * size);

	file = fopen(fileName, "r");
	for(int i = 0; i < size; i++) {
		fscanf(file, "%i", &contentsOfFile[i]);
	}
	fclose(file);

	return contentsOfFile;
}
