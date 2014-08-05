ifdef DEBUG
LDFLAGS=-g
CXXFLAGS+=-g
else
CXXFLAGS+=-O2
endif

all: qmcalc README.md header.tab

HFILES=$(wildcard *.h)
OFILES=CMetricsCalculator.o QualityMetrics.o

UnitTests: UnitTests.cpp $(HFILES) $(OFILES)
	$(CXX) $(LDFLAGS) UnitTests.cpp $(OFILES) -lcppunit -o $@

test: UnitTests
	./UnitTests

qmcalc: $(HFILES) $(OFILES) qmcalc.cpp
	$(CXX) $(LDFLAGS) qmcalc.cpp $(OFILES) -o $@

README.md: make-readme.sh QualityMetrics.h QualityMetrics.cpp
	./make-readme.sh >$@

header.tab: make-header.sh
	./make-header.sh >$@
