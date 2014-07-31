LDFLAGS=-g

all: test

HFILES=$(wildcard *.h)

UnitTests: UnitTests.cpp $(HFILES)
	$(CXX) $(LDFLAGS) UnitTests.cpp -lcppunit -o $@

test: UnitTests
	./UnitTests
