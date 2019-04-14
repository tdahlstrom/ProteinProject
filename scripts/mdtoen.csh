#! /bin/csh -f
# author: Thomas Dahlstrom

if ($#argv != 2) then
	echo "Usage Example: mdtoen md.log 150K.en"
	echo "Creates .en file from a Gromacs md.log file"
	exit 0
endif

awk '{print $4}' $1 | grep "-" | grep "e" | awk '{print NR, $1}' > $2

