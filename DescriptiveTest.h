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

#include <sstream>

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
	CPPUNIT_TEST(testOutput);
	CPPUNIT_TEST(testOutputEmpty);
	CPPUNIT_TEST(testSDZero);
	CPPUNIT_TEST(testSDTwo);
	CPPUNIT_TEST(testSDHalf);
	CPPUNIT_TEST(testMedianOne);
	CPPUNIT_TEST(testMedianTwo);
	CPPUNIT_TEST(testMedianEven);
	CPPUNIT_TEST(testMedianOdd);
	CPPUNIT_TEST(testMedianPartial);
	CPPUNIT_TEST_SUITE_END();
public:
	void testCtor() {
		Descriptive<int> a;
		CPPUNIT_ASSERT(a.get_count() == 0);
		CPPUNIT_ASSERT(a.get_sum() == 0);
		CPPUNIT_ASSERT(std::isnan(a.get_standard_deviation()));
		CPPUNIT_ASSERT(std::isnan(a.get_mean()));
		CPPUNIT_ASSERT(std::isnan(a.get_median()));
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

	void testOutput() {
		std::stringstream str;
		Descriptive<int> a;
		a.add(2);
		a.add(4);
		str << a;
		// Don't forget to update testOutputEmpty
		CPPUNIT_ASSERT(str.str() == "2\t2\t3\t3\t4\t1");
	}

	void testOutputEmpty() {
		std::stringstream str;
		Descriptive<int> a;
		str << a;
		CPPUNIT_ASSERT(str.str() == "0\t\t\t\t\t");
	}

	void testSDZero() {
		Descriptive<int> a;
		a.add(1);
		a.add(1);
		a.add(1);
		CPPUNIT_ASSERT(a.get_standard_deviation() == 0);
	}

	// From https://en.wikipedia.org/wiki/Standard_deviation#Basic_examples
	void testSDTwo() {
		Descriptive<int> a;
		a.add(2);
		a.add(4);
		a.add(4);
		a.add(4);
		a.add(5);
		a.add(5);
		a.add(7);
		a.add(9);
		CPPUNIT_ASSERT(a.get_standard_deviation() == 2);
	}

	// Verified in R with:
	// x <- c(1,2,rep(3,16)); sqrt(sum((x-mean(x))^2)/(length(x))
	void testSDHalf() {
		Descriptive<double> a;
		a.add(1);
		a.add(2);
		for (int i = 0; i < 16; i++)
			a.add(3);
		CPPUNIT_ASSERT(a.get_standard_deviation() == 0.5);
	}

	void testMedianOne() {
		Descriptive<int> a;
		a.add(42);
		CPPUNIT_ASSERT(a.get_median() == 42);
	}

	void testMedianTwo() {
		Descriptive<int> a;
		a.add(4);
		a.add(2);
		CPPUNIT_ASSERT(a.get_median() == 3);
	}

	void testMedianOdd() {
		Descriptive<int> a;
		a.add(2);
		a.add(0);
		a.add(1);
		CPPUNIT_ASSERT(a.get_median() == 1);
	}

	void testMedianEven() {
		Descriptive<int> a;
		a.add(2);
		a.add(0);
		a.add(1);
		a.add(5);
		CPPUNIT_ASSERT(a.get_median() == 1.5);
	}

	void testMedianPartial() {
		Descriptive<int> a;
		a.add(100);
		a.add(3);
		a.add(0);
		a.add(5);
		a.add(7);
		a.add(9);
		CPPUNIT_ASSERT(a.get_median() == 6);
	}

};
#endif /*  DESCRIPTIVETEST_H */
