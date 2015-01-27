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

QualityMetrics.o: QualityMetrics.h QualityMetrics.cpp QualityMetricNames.h
CMetricsCalculator.o: CMetricsCalculator.h CMetricsCalculator.cpp BolState.h \
	CharSource.h QualityMetrics.h NestingLevel.h

QualityMetricNames.h: QualityMetrics.h metric-names.sed
	sed -f metric-names.sed QualityMetrics.h >$@

README.md: make-readme.sh QualityMetrics.h QualityMetrics.cpp
	sh make-readme.sh >$@

header.tab: make-header.sh QualityMetrics.cpp
	sh make-header.sh >$@

clean:
	rm -f *.o *.exe qmcalc UnitTests
