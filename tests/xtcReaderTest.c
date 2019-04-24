#include <criterion/criterion.h>
#include <stdio.h>

#include "../software/xtcReader/xtcReader.h"

Test(xtcReader, Test_getFrames) {
	int frames = getFrames("./files/xtcFile", 163);

	cr_assert_eq(frames, 101);
}

Test(xtcReader, Test_getXtcFileCoordinates) {
	int frames = getFrames("./files/xtcFile", 163);
	struct XtcCoordinates** xtcCoords = getXtcFileCoordinates("./files/xtcFile", 163, frames);

	cr_assert_float_eq(xtcCoords[0][0].x, -2.802f, 0.0);
	cr_assert_float_eq(xtcCoords[0][0].z, 2.996f, 0.0);
}
