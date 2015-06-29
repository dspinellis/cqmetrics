# All warnings, treat warnings as errors, generate dependencies in .d files
CXXFLAGS=-Wall -Werror -MD

ifdef DEBUG
LDFLAGS=-g
CXXFLAGS+=-g -O0
else
CXXFLAGS+=-O2
endif

all: qmcalc README.md header.tab header.txt

HFILES=$(wildcard *.h)
OBJS=CMetricsCalculator.o QualityMetrics.o

UnitTests: UnitTests.o $(OBJS)
	$(CXX) $(LDFLAGS) UnitTests.o $(OBJS) -lcppunit -o $@

test: UnitTests
	./UnitTests

qmcalc: $(OBJS) qmcalc.o
	$(CXX) $(LDFLAGS) qmcalc.o $(OBJS) -o $@

QualityMetrics.o: QualityMetricNames.h

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
	rm -f *.o *.d *.exe qmcalc UnitTests QualityMetricNames.h \
		header.tab header.txt

# Pull-in dependencies generated with -MD
-include $(OBJS:.o=.d)
