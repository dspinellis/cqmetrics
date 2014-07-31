/*-
 * Copyright 2014 Diomidis Spinellis
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

#ifndef CMETRICSCALCULATORTEST_H
#define CMETRICSCALCULATORTEST_H

#include <sstream>

#include <cppunit/extensions/HelperMacros.h>

#include "CMetricsCalculator.h"

class CMetricsCalculatorTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(CMetricsCalculatorTest);
	CPPUNIT_TEST(testCtor);
	CPPUNIT_TEST(testLine);
	CPPUNIT_TEST_SUITE_END();
public:
	void testCtor() {
		CMetricsCalculator calc;

		const QualityMetrics& qm(calc.get_metrics());
		CPPUNIT_ASSERT(qm.get_nchar() == 0);
		CPPUNIT_ASSERT(qm.get_nline() == 0);
	}

	void testLine() {
		std::stringstream str("hi\n");
		CMetricsCalculator calc(str);
		calc.calculate_metrics();
		const QualityMetrics& qm(calc.get_metrics());
		CPPUNIT_ASSERT(qm.get_nchar() == 3);
		CPPUNIT_ASSERT(qm.get_nline() == 1);
	}
};
#endif /*  CMETRICSCALCULATORTEST_H */
