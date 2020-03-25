/*
*	Name: xtcReader.h
*	Author: Thomas Dahlstrom
*	Date: Mar. 24, 2017
*	Updated: Nov. 02, 2017
*
*	Dependencies: libxdrfile v2.1
*
*	Summary of expected functionality:
*		Provides functions to read a traj.xtc (trajectory) file from Gromacs 4.6.7.
*/

#include "xdrfile.h"
#include "xdrfile_xtc.h"

struct XtcCoordinates {
	float x;
	float y;
	float z;
};

struct XtcCoordinates** getXtcFileCoordinates(char *, int, int);
struct XtcCoordinates** allocateFrameArrayMemory(int, int);
int getFrames(char*, int);

/*
*	Name: struct XtcCoordinates** getXtcFileCoordinates()
*	Description:	Opens a trajectory from Gromacs 4.6.7.  Then values form the
*			trajectory file are input into a data structure.
*
*	Args: -char *xtcFile - location of the trajectory file to be read. 
*	      -int residues - total number of residues in the protein.
*	      -int xtcFrames - total number of frames in the xtcFile.
*
*	Returns: -struct XtcCoordinates** frameArray - a double pointer for a 2D-array
*			of structures which contain the x, y, and z velocities of
*			each residue of the protein at each frame of the
*			trajectory file.
*/

struct XtcCoordinates** getXtcFileCoordinates(char *xtcFile, int residues, int xtcFrames) {
	
	//The following variables are required for the xtc reader library	
	XDRFILE *xd;
	int result, natoms, step;
	float time, prec;
	matrix box;
	rvec *x;

	struct XtcCoordinates** frameArray = allocateFrameArrayMemory(residues, xtcFrames);

	result = read_xtc_natoms(xtcFile, &natoms);
	if (exdrOK != result) {
		printf("\nread_xtc_natoms: incorrect result\n");
		exit(1);
	}

	x = calloc(natoms,sizeof(*x));
	if (NULL == x) {
		printf("\nMemory for x not allocated.\n");
		exit(1);
	}

	xd = xdrfile_open(xtcFile, "r");
	if (NULL == xd) {
		printf("\nError opening XTC file\n");
		exit(1);	}

	int currentFrame = 0;

	

	// Iterates over, reads and stores the velocities contained in the trajectory file.
	do {
		// Reads a frame from the trajectory file.
		result = read_xtc(xd, natoms, &step, &time, box, x, &prec);

		if (exdrENDOFFILE != result) {
			if (exdrOK != result) {
				printf("\nread_xtc result: %d\n",result);
				exit(1);
			}

			// Stores the velocities contained in the trajectory file.
			for(int i = 0; i < residues; i++) {
				frameArray[currentFrame][i].x = x[i][0];
				frameArray[currentFrame][i].y = x[i][1];
				frameArray[currentFrame][i].z = x[i][2];
			}
	
			currentFrame++;
		}
	} while (exdrOK == result);

	// Frees up memory allocated for variable x.
	free(x);

	result = xdrfile_close(xd);
	if (result != 0) {
		printf("\nclose_xtc result: %d\n", result);
		exit(1);
	}

	return frameArray;
}

struct XtcCoordinates** allocateFrameArrayMemory(int residues, int xtcFrames) {
	struct XtcCoordinates **frameArray;
	
	frameArray = (struct XtcCoordinates**) malloc(sizeof(struct XtcCoordinates*) * xtcFrames);
	for(int i = 0; i < xtcFrames; i++) {
		frameArray[i] = (struct XtcCoordinates*) malloc(sizeof(struct XtcCoordinates) * residues);
	}

	return frameArray;
}

/*
*	Name: int getFrames()
*	Description:	Counts the frame in a trajectory from Gromacs 4.5.7.
*
*	Args: -char *xtcFile - location of the trajectory file to be read. 
*	      -int residues - total number of residues in the protein.	      
*
*	Returns: currentFrame - number of frames contained in the trajectory file.
*
*	Note: Inline comments ommitted due to most of the code being very similar
*	      to readXtcFile().
*/

int getFrames(char *xtcFile, int residues) {
	XDRFILE *xd;
	int result, natoms, step;
	float time, prec;
	matrix box;
	rvec *x;

	result = read_xtc_natoms(xtcFile, &natoms);
	if (exdrOK != result) {
		printf("\nread_xtc_natoms: incorrect result\n");
		exit(1);
	}
	
	if (residues != natoms) {
		printf("\nPlease change directive value residues.  residues is defined in");
		printf("\nxtcReader.h as the number of residues cointained in the protein used");
		printf("\nto calculate the values of the trajectory file.\n");
		printf("residues = %i\n", residues);
		printf("natoms = %i\n", natoms);
		exit(1);
	}

	x = calloc(natoms,sizeof(*x));
	if (NULL == x) {
		printf("\nMemory for x not allocated.\n");
		exit(1);
	}

	xd = xdrfile_open(xtcFile, "r");
	if (NULL == xd) {
		printf("\nError opening XTC file\n");
		exit(1);
	}

	int currentFrame = 0;

	do {
		result = read_xtc(xd, natoms, &step, &time, box, x, &prec);

		if (exdrENDOFFILE != result) {
			if (exdrOK != result) {
				printf("\nread_xtc result: %d\n", result);
				exit(1);
			}

			currentFrame++;
		}
	} while (exdrOK == result);

	free(x);

	result = xdrfile_close(xd);
	if (result != 0) {
		printf("\nclose_xtc result: %d\n", result);
		exit(1);
	}

	return currentFrame;
}
