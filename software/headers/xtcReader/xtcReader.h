#ifndef XTC_READER
#define XTC_READER

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

struct XtcCoordinates {
	float x;
	float y;
	float z;
};

struct XtcCoordinates** getXtcFileCoordinates(char *, int, int);
struct XtcCoordinates** allocateFrameArrayMemory(int, int);
int getFrames(char*, int);


#endif
