averageContactProbabilityInQValueRangeProg:
	gcc -o averageContactProbabilityInQValueRangeProg averageContactProbabilityInQValueRangeProg.c headers/xtcReader/xtcReader.c headers/xtcReader/xdrfile.c headers/xtcReader/xdrfile_xtc.c headers/contactReader/contactReader.c headers/calcQFromContacts/calcQFromContacts.c headers/probabilityContactInQValueRange/probabilityContactInQValueRange.c headers/averageContactProbabilityInQValueRange/averageContactProbabilityInQValueRange.c -lm

calcQFromContactsProg:
	gcc -o calcQFromContactsProg calcQFromContactsProg.c headers/xtcReader/xtcReader.c headers/xtcReader/xdrfile.c headers/xtcReader/xdrfile_xtc.c headers/contactReader/contactReader.c headers/calcQFromContacts/calcQFromContacts.c -lm

probabilityContactInQValueRangeProg:
	gcc -o probabilityContactInQValueRangeProg probabilityContactInQValueRangeProg.c headers/xtcReader/xtcReader.c headers/xtcReader/xdrfile.c headers/xtcReader/xdrfile_xtc.c headers/contactReader/contactReader.c headers/calcQFromContacts/calcQFromContacts.c headers/probabilityContactInQValueRange/probabilityContactInQValueRange.c -lm

clean:
	rm *Prog