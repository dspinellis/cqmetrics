all: test

HFILES=BolState.h BolStateTest.h CharSource.h CharSourceTest.h

UnitTests: UnitTests.cpp $(HFILES)
	g++ UnitTests.cpp -lcppunit -o $@

test: UnitTests
	./UnitTests
