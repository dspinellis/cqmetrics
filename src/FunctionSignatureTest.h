/*-
 * Copyright 2017 Diomidis Spinellis, Thodoris Sotiropoulos
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

#ifndef FUNCTIONSIGNATURETEST_H
#define FUNCTIONSIGNATURETEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "FunctionSignature.h"

class FunctionSignatureTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(FunctionSignatureTest);
	CPPUNIT_TEST(testCtor);
	CPPUNIT_TEST(testAddParam);
	CPPUNIT_TEST_SUITE_END();
public:
	void testCtor() {
		FunctionSignature func_sign;
		CPPUNIT_ASSERT_EQUAL(func_sign.get_nparams(), 0);
	}

	void testAddParam() {
		FunctionSignature func_sign;
		func_sign.add_param();
		CPPUNIT_ASSERT_EQUAL(func_sign.get_nparams(), 1);
		func_sign.add_param();
		CPPUNIT_ASSERT_EQUAL(func_sign.get_nparams(), 2);
	}
};
#endif /* FUNCTIONSIGNATURETEST_H */
