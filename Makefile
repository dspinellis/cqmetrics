ifdef DEBUG
LDFLAGS=-g
CXXFLAGS+=-g -O0
else
CXXFLAGS+=-O2
endif

all: qmcalc README.md header.tab header.txt

HFILES=$(wildcard *.h)
OFILES=CMetricsCalculator.o QualityMetrics.o

UnitTests: UnitTests.cpp $(HFILES) $(OFILES)
	$(CXX) $(LDFLAGS) UnitTests.cpp $(OFILES) -lcppunit -o $@

test: UnitTest
	./UnitTests

qmcalc: $(HFILES) $(OFILES) qmcalc.cpp
	$(CXX) $(LDFLAGS) qmcalc.cpp $(OFILES) -o $@

QualityMetrics.o: QualityMetrics.h QualityMetrics.cpp QualityMetricNames.h
CMetricsCalculator.o: CMetricsCalculator.h CMetricsCalculator.cpp BolState.h \
	CharSource.h QualityMetrics.h NestingLevel.h

QualityMetricNames.h: QualityMetrics.h metric-names.sed
	sed -n -f metric-names.sed QualityMetrics.h >$@

README.md: make-readme.sh QualityMetrics.h QualityMetrics.cpp
	sh make-readme.sh >$@

# Tab-separated, ending with a newline
header.tab: make-header.sh QualityMetrics.cpp
	sh make-header.sh | tr ' \n' '\t\t' | sed 's/\t$$/\n/' >$@

# Numbered lines
header.txt: make-header.sh QualityMetrics.cpp
	sh make-header.sh | tr ' \t' '\n\n' | cat -n >$@

clean:
	rm -f *.o *.exe qmcalc UnitTests
