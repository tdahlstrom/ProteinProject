/*
*	Name: contactReaderTest.c
*	Author: Thomas Dahlstrom
*	Date: Apr. 23, 2019
*	Updated: Mar. 26, 2020
*/

#include <criterion/criterion.h>
#include <stdio.h>

#include "../software/headers/xtcReader/xtcReader.h"

Test(xtcReader, Test_getFrames) {
	int frames = getFrames("./files/xtcFile", 163);

	cr_assert_eq(frames, 101);
}

Test(xtcReader, Test_getXtcFileCoordinates) {
	int frames = getFrames("./files/xtcFile", 163);
	struct XtcCoordinates** xtcCoords = getXtcFileCoordinates("./files/xtcFile", 163, frames);

	cr_assert_float_eq(-2.802f, xtcCoords[0][0].x, 0.0);
	cr_assert_float_eq(2.996f, xtcCoords[0][0].z, 0.0);
}
