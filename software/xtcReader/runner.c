/*
*	Name: runner.c
*	Author: Thomas Dahlstrom
*	Date: Mar. 24, 2017
*	Updated: Apr. 22, 2017
*
*	Summary of expected functionality:
*		Provides an example of how use xtcReader.h.
*	Compile:
*		gcc -o runner runner.c xdrfile.c xdrfile_xtc.c
*/

#include <stdio.h>
#include <stdlib.h>

#include "xtcReader.h"

int main(int argc, char *argv[]) {
	char *fileLocation = "/home/tdahlstrom/Desktop/coordsReader/data/traj.xtc";

	int frames = getFrames(fileLocation, 163);

	struct XtcCoordinates** frameArray = getXtcFileCoordinates(fileLocation, 163, frames);

	for(int i = 0; i < 1; i++) {
		printf("frame = %i\n", i);

		for(int j = 0; j < 163; j++) {
			printf("x = %f\ty = %f\tz = %f\n", frameArray[i][j].x, frameArray[i][j].y, frameArray[i][j].z);
		}
	}

	exit(0);
}
