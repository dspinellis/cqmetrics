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

#ifndef BOILERPLATETEST_H
#define BOILERPLATETEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "Boilerplate.h"

class BoilerplateTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(BoilerplateTest);
	CPPUNIT_TEST(testCtor);
	CPPUNIT_TEST(testNonBoilerplate);
	CPPUNIT_TEST(testBoilerplateWarranties);
	CPPUNIT_TEST(testBoilerplateCopyright);
	CPPUNIT_TEST(testBoilerplateBegin);
	CPPUNIT_TEST_SUITE_END();
private:
	/** Process the characters of the specified string */
	void process_string(Boilerplate &b, const std::string &s) {
		for (std::string::const_iterator i = s.begin(); i != s.end(); i++)
			b.process_char(*i);
	}

public:
	void testCtor() {
		Boilerplate o;
		CPPUNIT_ASSERT(o.get_boilerplate_chars() == 0);
	}

	void testNonBoilerplate() {
		Boilerplate o;

		process_string(o, "/** This is a test */");
		CPPUNIT_ASSERT(o.get_boilerplate_chars() == 0);
		process_string(o, "not copyrighted code");
		CPPUNIT_ASSERT(o.get_boilerplate_chars() == 0);
	}

	void testBoilerplateWarranties() {
		Boilerplate o;

		process_string(o, " * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND\n");
		process_string(o, " * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE\n");
		CPPUNIT_ASSERT(o.get_boilerplate_chars() > 100);
		CPPUNIT_ASSERT(o.get_boilerplate_chars() < 200);
	}

	void testBoilerplateCopyright() {
		Boilerplate o;

		process_string(o, "Copyright (c) 1999 Lennart");
		CPPUNIT_ASSERT(o.get_boilerplate_chars() > 20);
		CPPUNIT_ASSERT(o.get_boilerplate_chars() < 80);
	}

	void testBoilerplateBegin() {
		Boilerplate o;

		process_string(o, "Copyright (c) 1999 Lennart");
		o.begin_comment();
		CPPUNIT_ASSERT(o.get_boilerplate_chars() == 0);
	}
};
#endif /*  BOILERPLATETEST_H */
