/*-
 * Copyright 2015 Diomidis Spinellis
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

#include <cppunit/ui/text/TestRunner.h>

#include "BoilerplateTest.h"
#include "BolStateTest.h"
#include "CharSourceTest.h"
#include "CKeywordTest.h"
#include "CMetricsCalculatorTest.h"
#include "CyclomaticTest.h"
#include "DescriptiveTest.h"
#include "HalsteadTest.h"
#include "NestingLevelTest.h"
#include "QualityMetricsTest.h"

int
main(int argc, char *argv[])
{
	CppUnit::TextUi::TestRunner runner;

	runner.addTest(BoilerplateTest::suite());
	runner.addTest(BolStateTest::suite());
	runner.addTest(CharSourceTest::suite());
	runner.addTest(CKeywordTest::suite());
	runner.addTest(CMetricsCalculatorTest::suite());
	runner.addTest(CyclomaticTest::suite());
	runner.addTest(DescriptiveTest::suite());
	runner.addTest(HalsteadTest::suite());
	runner.addTest(NestingLevelTest::suite());
	runner.addTest(QualityMetricsTest::suite());

	runner.run();
	return 0;
}
