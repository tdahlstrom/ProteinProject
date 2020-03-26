averageContactProbabilityInQValueRangeTest:
	gcc -o test averageContactProbabilityInQValueRangeTest.c ../software/headers/xtcReader/xtcReader.c ../software/headers/xtcReader/xdrfile.c ../software/headers/xtcReader/xdrfile_xtc.c ../software/headers/contactReader/contactReader.c ../software/headers/calcQFromContacts/calcQFromContacts.c ../software/headers/probabilityContactInQValueRange/probabilityContactInQValueRange.c ../software/headers/averageContactProbabilityInQValueRange/averageContactProbabilityInQValueRange.c -lcriterion -lm

calcQFromContactsTest:
	gcc -o test calcQFromContactsTest.c ../software/headers/xtcReader/xtcReader.c ../software/headers/xtcReader/xdrfile.c ../software/headers/xtcReader/xdrfile_xtc.c ../software/headers/contactReader/contactReader.c ../software/headers/calcQFromContacts/calcQFromContacts.c -lcriterion -lm

contactReaderTest:
	gcc -o test contactReaderTest.c ../software/headers/contactReader/contactReader.c -lcriterion

probabilityContactInQValueRangeTest:
	gcc -o test probabilityContactInQValueRangeTest.c ../software/headers/xtcReader/xtcReader.c ../software/headers/xtcReader/xdrfile.c ../software/headers/xtcReader/xdrfile_xtc.c ../software/headers/contactReader/contactReader.c ../software/headers/calcQFromContacts/calcQFromContacts.c ../software/headers/probabilityContactInQValueRange/probabilityContactInQValueRange.c -lcriterion -lm

xtcReaderTest:
	gcc -o test xtcReaderTest.c ../software/headers/xtcReader/xtcReader.c ../software/headers/xtcReader/xdrfile.c ../software/headers/xtcReader/xdrfile_xtc.c -lcriterion
	
clean:
	rm test