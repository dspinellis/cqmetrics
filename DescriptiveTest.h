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

#ifndef DESCRIPTIVETEST_H
#define DESCRIPTIVETEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "Descriptive.h"

class DescriptiveTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(DescriptiveTest);
	CPPUNIT_TEST(testCtor);
	CPPUNIT_TEST(testAdd);
	CPPUNIT_TEST(testMean);
	CPPUNIT_TEST(testMeanDouble);
	CPPUNIT_TEST(testMinMax);
	CPPUNIT_TEST(testMinMaxDouble);
	CPPUNIT_TEST_SUITE_END();
public:
	void testCtor() {
		Descriptive<int> a;
		CPPUNIT_ASSERT(a.get_count() == 0);
		CPPUNIT_ASSERT(a.get_sum() == 0);
	}

	void testAdd() {
		Descriptive<int> a;
		a.add(12);
		CPPUNIT_ASSERT(a.get_sum() == 12);
		CPPUNIT_ASSERT(a.get_count() == 1);
		a.add(2);
		CPPUNIT_ASSERT(a.get_sum() == 14);
		CPPUNIT_ASSERT(a.get_count() == 2);
	}

	void testMean() {
		Descriptive<int> a;
		a.add(3);
		a.add(12);
		CPPUNIT_ASSERT(a.get_mean() == 7.5);
	}

	void testMeanDouble() {
		Descriptive<double> a;
		a.add(3);
		a.add(1.5);
		a.add(1.5);
		CPPUNIT_ASSERT(a.get_mean() == 2);
	}

	void testMinMax() {
		Descriptive<int> a;
		a.add(5);
		a.add(2);
		a.add(12);
		CPPUNIT_ASSERT(a.get_min() == 2);
		CPPUNIT_ASSERT(a.get_max() == 12);
	}
	void testMinMaxDouble() {
		Descriptive<double> a;
		a.add(5.1);
		a.add(2.5);
		a.add(12.5);
		CPPUNIT_ASSERT(a.get_min() == 2.5);
		CPPUNIT_ASSERT(a.get_max() == 12.5);
	}
};
#endif /*  DESCRIPTIVETEST_H */
