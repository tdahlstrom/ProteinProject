/*
*	Name: xtcReader.h
*	Author: Thomas Dahlstrom
*	Date: Mar. 24, 2017
*	Updated: Mar. 26, 2020
*/

#ifndef XTC_READER
#define XTC_READER

struct XtcCoordinates {
	float x;
	float y;
	float z;
};

struct XtcCoordinates** getXtcFileCoordinates(char *, int, int);
struct XtcCoordinates** allocateFrameArrayMemory(int, int);
int getFrames(char*, int);


#endif
