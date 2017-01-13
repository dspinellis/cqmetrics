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

#ifndef HALSTEADTEST_H
#define HALSTEADTEST_H

#include <cmath>

#include <cppunit/extensions/HelperMacros.h>

#include "Halstead.h"

class HalsteadTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(HalsteadTest);
	CPPUNIT_TEST(testCtor);
	CPPUNIT_TEST(testReset);
	CPPUNIT_TEST(testSimple);
	CPPUNIT_TEST(testComplex);
	CPPUNIT_TEST_SUITE_END();
public:
	void testCtor() {
		Halstead h;

		CPPUNIT_ASSERT(h.complexity() == 0);
	}

	void testReset() {
		Halstead h;

		h.add("a");
		h.add("b");
		h.reset();
		CPPUNIT_ASSERT(h.complexity() == 0);
	}

	void testSimple() {
		Halstead h;

		h.add("a");
		CPPUNIT_ASSERT(h.complexity() == 0);

		h.add("b");
		/* 2 * log2(2) == 2 * 1 == 2 */
		CPPUNIT_ASSERT(h.complexity() == 2);

		h.add("b");
		/* 3 * log2(2) == 3 * 1 == 3 */
		CPPUNIT_ASSERT(h.complexity() == 3);
	}

	/* From Code Quality: The Open Source Perspective, p. 328 */
	void testComplex() {
		Halstead h;

		h.add("old_bucket");
		h.add("=");
		h.add("(");
		h.add("hashp");
		h.add("->");
		h.add("MAX_BUCKET");
		h.add("&");
		h.add("hashp");
		h.add("->");
		h.add("LOW_MASK");
		h.add(")");

		/*
		 * The book performs the multiplication after rounding log2
		 * to 9 and gives 35.2 as the result.
		 */
		CPPUNIT_ASSERT(fabs(h.complexity() - 34.87) < 0.01);
	}
};
#endif /*  HALSTEADTEST_H */
