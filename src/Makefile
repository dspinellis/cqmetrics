# All warnings, treat warnings as errors, generate dependencies in .d files
CXXFLAGS=-Wall -Werror -MD

ifdef DEBUG
LDFLAGS=-g
CXXFLAGS+=-g -O0
else
CXXFLAGS+=-O2
endif

INSTALL_PREFIX?=/usr/local
SHARE != [ -d ${PREFIX}/share/man ] && echo /share || true
MAN_PREFIX ?= ${INSTALL_PREFIX}${SHARE}/man

all: qmcalc ../metrics.md header.tab header.txt qmcalc.1

HFILES=$(wildcard *.h)
OBJS=CMetricsCalculator.o QualityMetrics.o qmcalc.o

UnitTests: UnitTests.o $(OBJS)
	$(CXX) $(LDFLAGS) UnitTests.o $(filter-out qmcalc.o,$(OBJS)) -lcppunit -o $@

test: UnitTests
	./UnitTests

qmcalc: $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $@

QualityMetrics.o: QualityMetricNames.h

QualityMetricNames.h: QualityMetrics.h metric-names.sed
	sed -n -f metric-names.sed QualityMetrics.h >$@

../metrics.md: make-metrics-list.sh QualityMetrics.h QualityMetrics.cpp
	sh make-metrics-list.sh >$@

# Tab-separated, ending with a newline
header.tab: make-header.sh QualityMetrics.h QualityMetrics.cpp
	sh make-header.sh | tr ' \n' '\t\t' | sed 's/\t$$/\n/' >$@

# Numbered lines
header.txt: make-header.sh QualityMetrics.h QualityMetrics.cpp
	sh make-header.sh | tr ' \t' '\n\n' | cat -n >$@

qmcalc.1: make-metrics-list-man.sh QualityMetrics.h QualityMetrics.cpp
		sh make-metrics-list-man.sh >$@

install-bin: qmcalc
	install qmcalc "$(INSTALL_PREFIX)/bin/qmcalc"
	install showstyle.sh "$(INSTALL_PREFIX)/bin/qmcalc-showstyle"
	install sumstyle.sh "$(INSTALL_PREFIX)/bin/qmcalc-sumstyle"

install-man: qmcalc.1
	install qmcalc.1 "$(MAN_PREFIX)/qmcalc.1"
	ln -s "$(MAN_PREFIX)/qmcalc.1" qmcalc-showstyle.1
	ln -s "$(MAN_PREFIX)/qmcalc.1" qmcalc-sumstyle.1

install: install-bin install-man

clean:
	rm -f *.o *.d *.exe qmcalc UnitTests QualityMetricNames.h \
		header.tab header.txt

# Pull-in dependencies generated with -MD
-include $(OBJS:.o=.d)
