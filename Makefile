all: test

UnitTests: UnitTests.cpp
	g++ UnitTests.cpp -lcppunit -o $@

test: UnitTests
	./UnitTests
