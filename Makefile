LDFLAGS = -L.
LDLIBS = -lGoncharovsAlgorithm -lstdc++
# Specify extensions of files to delete when cleaning
CLEANEXTS   = o a 

# Specify the target file and the install directory
OUTPUTFILE = libGoncharovsAlgorithm.a 
TESTFILE = test

# Default target
.PHONY: all
all: $(OUTPUTFILE) $(TESTFILE)

# Build libjohnpaul.a from john.o, paul.o, and johnpaul.o
$(OUTPUTFILE): csGoncharovsAlgorithm.o
	ar ru $@ $^
	ranlib $@

$(TESTFILE): test.o

# No rule to build john.o, paul.o, and johnpaul.o from .cpp 
# files is required; this is handled by make's database of
# implicit rules

.PHONY: install
install:
	cp -p $(OUTPUTFILE) .

.PHONY: clean 
clean:
	for file in $(CLEANEXTS); do rm -f *.$$file; done

# Indicate dependencies of .ccp files on .hpp files
csGoncharovsAlgorithm.o: csGoncharovsAlgorithm.h csPop.h
