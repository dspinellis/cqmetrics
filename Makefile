all: test

HFILES=$(wildcard *.h)

UnitTests: UnitTests.cpp $(HFILES)
	g++ UnitTests.cpp -lcppunit -o $@

test: UnitTests
	./UnitTests
