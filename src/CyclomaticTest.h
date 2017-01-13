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

#ifndef CYCLOMATICTEST_H
#define CYCLOMATICTEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "Cyclomatic.h"

class CyclomaticTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(CyclomaticTest);
	CPPUNIT_TEST(testCtor);
	CPPUNIT_TEST(testAdd);
	CPPUNIT_TEST_SUITE_END();
public:
	void testCtor() {
		Cyclomatic c;
		CPPUNIT_ASSERT(c.complexity() == 1);
		CPPUNIT_ASSERT(c.extended_complexity() == 1);
	}

	void testAdd() {
		Cyclomatic c;
		c.add_path();
		c.add_path();
		c.add_boolean_branch();
		CPPUNIT_ASSERT(c.complexity() == 3);
		CPPUNIT_ASSERT(c.extended_complexity() == 4);
	}
};
#endif /*  CYCLOMATICTEST_H */
