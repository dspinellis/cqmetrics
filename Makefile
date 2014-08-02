LDFLAGS=-g
CXXFLAGS=-g

all: test qmcalc

HFILES=$(wildcard *.h)
OFILES=CMetricsCalculator.o

UnitTests: UnitTests.cpp $(HFILES) $(OFILES)
	$(CXX) $(LDFLAGS) UnitTests.cpp $(OFILES) -lcppunit -o $@

test: UnitTests
	./UnitTests

qmcalc: $(HFILES) $(OFILES) qmcalc.cpp
	$(CXX) $(LDFLAGS) qmcalc.cpp $(OFILES) -o $@
