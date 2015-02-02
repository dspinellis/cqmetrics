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

#ifndef QUALITYMETRICSTEST_H
#define QUALITYMETRICSTEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "QualityMetrics.h"

class QualityMetricsTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(QualityMetricsTest);
	CPPUNIT_TEST(testCtor);
	CPPUNIT_TEST(testNline);
	CPPUNIT_TEST(testInconsistency);
	CPPUNIT_TEST_SUITE_END();
public:
	void testCtor() {
		QualityMetrics q;
		CPPUNIT_ASSERT(q.get_nchar() == 0);
		CPPUNIT_ASSERT(q.get_ntypedef() == 0);
	}

	void testNline() {
		QualityMetrics q;

		q.add_line(10);
		q.add_line(12);
		CPPUNIT_ASSERT(q.get_line_length().get_count() == 2);
	}

	// Uncovered specification error
	void testInconsistency() {
		CPPUNIT_ASSERT(QualityMetrics::inconsistency(5, 0) == 0);
		CPPUNIT_ASSERT(QualityMetrics::inconsistency(0, 5) == 0);
		CPPUNIT_ASSERT(QualityMetrics::inconsistency(5, 5) == 5);
		CPPUNIT_ASSERT(QualityMetrics::inconsistency(2, 4) == 2);
		CPPUNIT_ASSERT(QualityMetrics::inconsistency(6, 2) == 2);
		CPPUNIT_ASSERT(QualityMetrics::inconsistency(1, 100) == 1);
	}
};
#endif /*  QUALITYMETRICSTEST_H */
