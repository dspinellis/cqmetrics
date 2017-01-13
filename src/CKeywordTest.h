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

#ifndef CKEYWORDTEST_H
#define CKEYWORDTEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "CKeyword.h"

class CKeywordTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(CKeywordTest);
	CPPUNIT_TEST(testBranch);
	CPPUNIT_TEST(testGoto);
	CPPUNIT_TEST(testRegister);
	CPPUNIT_TEST(testTypedef);
	CPPUNIT_TEST(testOther);
	CPPUNIT_TEST(testId);
	CPPUNIT_TEST(testInclude);
	CPPUNIT_TEST_SUITE_END();
	CKeyword ck;
public:
	void testBranch() {
		CPPUNIT_ASSERT(ck.identifier_type("if") == CKeyword::IF);
		CPPUNIT_ASSERT(ck.identifier_type("elif") == CKeyword::ELIF);
		CPPUNIT_ASSERT(ck.identifier_type("ifdef") == CKeyword::IFDEF);
		CPPUNIT_ASSERT(ck.identifier_type("for") == CKeyword::FOR);
		CPPUNIT_ASSERT(ck.identifier_type("while") == CKeyword::WHILE);
		CPPUNIT_ASSERT(ck.identifier_type("case") == CKeyword::CASE);
		CPPUNIT_ASSERT(ck.identifier_type("default") == CKeyword::DEFAULT);
	}

	void testGoto() {
		CPPUNIT_ASSERT(ck.identifier_type("goto") == CKeyword::GOTO);
	}

	void testRegister() {
		CPPUNIT_ASSERT(ck.identifier_type("register") == CKeyword::REGISTER);
	}

	void testInclude() {
		CPPUNIT_ASSERT(ck.identifier_type("include") == CKeyword::INCLUDE);
	}

	void testTypedef() {
		CPPUNIT_ASSERT(ck.identifier_type("typedef") == CKeyword::TYPEDEF);
	}

	void testOther() {
		CPPUNIT_ASSERT(ck.identifier_type("sizeof") == CKeyword::OTHER);
		CPPUNIT_ASSERT(ck.identifier_type("int") == CKeyword::OTHER);
		CPPUNIT_ASSERT(ck.identifier_type("static") == CKeyword::STATIC);
	}

	void testId() {
		CPPUNIT_ASSERT(ck.identifier_type("returning") == CKeyword::IDENTIFIER);
		CPPUNIT_ASSERT(ck.identifier_type("id") == CKeyword::IDENTIFIER);
		CPPUNIT_ASSERT(ck.identifier_type("xyzzy") == CKeyword::IDENTIFIER);
	}
};
#endif /*  CKEYWORDTEST_H */
