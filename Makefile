LDFLAGS=-g
CXXFLAGS=-g

all: test

HFILES=$(wildcard *.h)
OFILES=CMetricsCalculator.o

UnitTests: UnitTests.cpp $(HFILES) $(OFILES)
	$(CXX) $(LDFLAGS) UnitTests.cpp $(OFILES) -lcppunit -o $@

test: UnitTests
	./UnitTests
