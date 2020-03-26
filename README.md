# ProteinProject
**Program:** averageContactProbabilityInQValueRangeProg.c  
**Description:** Calculates the probability of a residue being within a range of Q values from a trajectory.

**Program:** calcQFromContactsProg.c  
**Description:** Determines the Q value of each frame of a given trajectory.

**Program:** probabilityContactInQValueRangeProg.c  
**Description:** Calculates the probability of a contact being within a range of Q values from a trajectory.

**Header:** xtcReader.h  
**Description:** Provides functions to read a traj.xtc (trajectory) file from Gromacs 4.6.7.  
**Requires:** [libxdrfile v2.1](https://github.com/wesbarnett/libxdrfile/tree/2.1).

**Header:** contactReader.h  
**Description:** Provides functions to read a contact file created from [SMOG Server](http://smog-server.org).

**Script:** mdtoen.csh  
**Description:** Parses out the LJ-12 energy values into a file.

**Script:** run_simulation_set.csh  
**Description:** Automates the starting of Gromacs simulation jobs.  User can edit how many jobs are ran at once.  

# Running Tests
From the tests folder run:
```bash
make {name of test file}
```