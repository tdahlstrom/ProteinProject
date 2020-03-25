void writeQFile(int *qValues, int xtcFrames, char *qFile);
int* calculateQValues(int xtcFrames, int contacts, float cutoff, struct XtcCoordinates **xtcResidueCoordinates, struct Contact *residueContacts);
float calculateDistance(struct XtcCoordinates coords1, struct XtcCoordinates coords2);
int* allocateQValuesMemory(int xtcFrames);

/*
*	Name: void writeQFile()
*	Description: Creates a file with a Q value on every line.
*
*	Args: -int *qValues - a Q value for each frame of the traj.xtc.
*	      -int xtcFrames - the amount of frames in the traj.xtc file.
*	      -char *qFile - the file name where Q values will be written.
*/

void writeQFile(int *qValues, int xtcFrames, char *qFile) {
	FILE *fp;

	// Creates the Q file
	if ((fp = fopen(qFile, "w")) == NULL) {
		perror("could not open qFile for output.");
		exit(1);
	}

	// Writes the Q values to the file
	for(int i = 0; i < xtcFrames; i++) {
		fprintf(fp, "%i\n", qValues[i]);
	}

	// Closes the Q file
	fclose(fp);
}

/*
*	Name: int* calculateQValues()
*	Description: Calculates the Q values of every frames from the traj.xtc.  Uses the contacts
*		     from the contact file and measures if contact is present via a cutoff value.
*
*	Args: -int xtcFrames - the amount of frames in the traj.xtc file.
*	      -int contacts - the amount of contacts in the contacts file.
*	      -float cutoff - the user specified contact cutoff value for a residue pair.
*	      -struct XtcCoordinates **xtcResidueCoordinates - frame data and residue coordinates from
*							       the traj.xtc file.
*	      -struct Contact *residueContacts - all of the residue pairs from the contacts file.
*
*	returns: -int *qValues - a Q value for each frame of the traj.xtc.
*/

int* calculateQValues(int xtcFrames, int contacts, float cutoff, struct XtcCoordinates **xtcResidueCoordinates, struct Contact *residueContacts) {
	int *qValues = allocateQValuesMemory(xtcFrames);

	// For every frame in the traj.xtc file
	for(int i = 0; i < xtcFrames; i++) {

		// For every contact pair in the contact file
		for(int j = 0; j < contacts; j++) {

			// If the distance between the residue pair is within the cutoff value
			if(calculateDistance(xtcResidueCoordinates[i][residueContacts[j].focusResidue-1],
			   xtcResidueCoordinates[i][residueContacts[j].contactResidue-1]) <= cutoff) {
				qValues[i]++;
			}
		}
	}

	return qValues;
}

/*
*	Name: float calculateDistance()
*	Description: Calculates the distance between the residue pair.
*
*	Args: -struct XtcCoordinates coords1 - coordinates of the focused residue.
*	      -struct XtcCoordinates coords2 - coordinates of the residue potentially in contact
*					       with the focused residue.
*
*	returns: -float - distance between the two residues.
*/

float calculateDistance(struct XtcCoordinates coords1, struct XtcCoordinates coords2) {
	return (float) sqrt(pow(coords2.x - coords1.x, 2) + pow(coords2.y - coords1.y, 2) + pow(coords2.z - coords1.z, 2));
}

int* allocateQValuesMemory(int xtcFrames) {
	int *qValues;

	qValues = (int*) malloc(sizeof(int) * xtcFrames);
	if(!qValues) {
		perror("qValues memory not allocated");
		abort();
	}

	for(int i = 0; i < xtcFrames; i++) {
		qValues[i] = 0;
	}

	return qValues;
}
