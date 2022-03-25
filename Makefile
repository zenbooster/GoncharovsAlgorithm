CPPFLAGS = -O3
LDFLAGS = -L.
LDLIBS = -lstdc++
# Specify extensions of files to delete when cleaning
CLEANEXTS   = o 

# Specify the target file and the install directory
OUTPUTFILE = test

# Default target
.PHONY: all
all: $(OUTPUTFILE)

$(OUTPUTFILE): test.o

# No rule to build john.o, paul.o, and johnpaul.o from .cpp 
# files is required; this is handled by make's database of
# implicit rules

#.PHONY: install
#install:
#	cp -p $(OUTPUTFILE) .

.PHONY: clean 
clean:
	for file in $(CLEANEXTS); do rm -f *.$$file; done

# Indicate dependencies of .cpp files on .h files
test.o: csGoncharovsAlgorithmTest.h csGoncharovsAlgorithm.h csPop.h
