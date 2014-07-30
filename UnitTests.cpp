#include <cppunit/ui/text/TestRunner.h>
#include "BolStateTest.h"


int main(int argc, char *argv[])
{
	CppUnit::TextUi::TestRunner runner; runner.addTest(BolStateTest::suite());
	runner.run();
	return 0;
}

