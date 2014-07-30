#include <cppunit/ui/text/TestRunner.h>

#include "BolStateTest.h"
#include "CharSourceTest.h"


int
main(int argc, char *argv[])
{
	CppUnit::TextUi::TestRunner runner;

	runner.addTest(BolStateTest::suite());
	runner.addTest(CharSourceTest::suite());
	runner.run();
	return 0;
}
