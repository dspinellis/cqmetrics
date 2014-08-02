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
#include "Halstead.h"

class CMetricsCalculatorTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(CMetricsCalculatorTest);
	CPPUNIT_TEST(testCtor);
	CPPUNIT_TEST(testNFunction);
	CPPUNIT_TEST(testNStatement);
	CPPUNIT_TEST(testStatementNesting);
	CPPUNIT_TEST(testStatementNestingTwoFunction);
	CPPUNIT_TEST(testHalsteadOperator);
	CPPUNIT_TEST(testHalsteadOperand);
	CPPUNIT_TEST(testHalsteadOperandTwoFunctions);
	CPPUNIT_TEST(testNLineComment);
	CPPUNIT_TEST(testNBlockComment);
	CPPUNIT_TEST(testNBlockCommentChar);
	CPPUNIT_TEST(testNLineCommentChar);
	CPPUNIT_TEST(testNCommentChar);
	CPPUNIT_TEST(testCyclomaticBoolean);
	CPPUNIT_TEST(testCyclomaticLogical);
	CPPUNIT_TEST(testCyclomaticLogicalTwoFunctions);
	CPPUNIT_TEST(testCyclomaticCombined);
	CPPUNIT_TEST(testCppDirective);
	CPPUNIT_TEST(testCKeyword);
	CPPUNIT_TEST(testIdentifierLength);
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

	void testNFunction() {
		std::stringstream str("foo()\n{((??}\nstruct bar {}");
		CMetricsCalculator calc(str);
		calc.calculate_metrics();
		const QualityMetrics& qm(calc.get_metrics());
		CPPUNIT_ASSERT(qm.get_nfunction() == 1);
	}

	void testHalsteadOperator() {
		std::stringstream str("foo()\n{((??}\nstruct bar {}");
		CMetricsCalculator calc(str);
		calc.calculate_metrics();
		const QualityMetrics& qm(calc.get_metrics());
		CPPUNIT_ASSERT(qm.get_halstead().get_count() == 1);
		CPPUNIT_ASSERT(qm.get_halstead().get_mean() == 4);
	}

	void testHalsteadOperand() {
		std::stringstream str("foo()\n{a a b b}\nstruct bar {}");
		CMetricsCalculator calc(str);
		calc.calculate_metrics();
		const QualityMetrics& qm(calc.get_metrics());
		CPPUNIT_ASSERT(qm.get_halstead().get_count() == 1);
		CPPUNIT_ASSERT(qm.get_halstead().get_mean() == 4);
	}

	void testHalsteadOperandTwoFunctions() {
		std::stringstream str("foo()\n{a a b b}bar()\n{a a b b}");
		CMetricsCalculator calc(str);
		calc.calculate_metrics();
		const QualityMetrics& qm(calc.get_metrics());
		CPPUNIT_ASSERT(qm.get_halstead().get_count() == 2);
		CPPUNIT_ASSERT(qm.get_halstead().get_mean() == 4);
	}

	void testNStatement() {
		std::stringstream str("foo()\n{a;b;c;}\nint a;");
		CMetricsCalculator calc(str);
		calc.calculate_metrics();
		const QualityMetrics& qm(calc.get_metrics());
		CPPUNIT_ASSERT(qm.get_statement_nesting().get_count() == 3);
	}

	void testStatementNesting() {
		std::stringstream str("foo()\n{a;{b;{c;}}}");
		CMetricsCalculator calc(str);
		calc.calculate_metrics();
		const QualityMetrics& qm(calc.get_metrics());
		CPPUNIT_ASSERT(qm.get_statement_nesting().get_count() == 3);
		// (1 + 2 + 3) / 3 == 2
		CPPUNIT_ASSERT(qm.get_statement_nesting().get_mean() == 2);
	}

	void testStatementNestingTwoFunction() {
		std::stringstream str("foo()\n{a;{{c;}}}"
			"bar()\n{{a;{b;{{{c;}}} d; }}}");
		CMetricsCalculator calc(str);
		calc.calculate_metrics();
		const QualityMetrics& qm(calc.get_metrics());
		CPPUNIT_ASSERT(qm.get_statement_nesting().get_count() == 6);
		// (1 + 3 + 2 + 3 + 6 + 3)  == 18; 18 / 6 == 3
		CPPUNIT_ASSERT(qm.get_statement_nesting().get_mean() == 3);
	}

	void testNBlockComment() {
		std::stringstream str("/* hi\n * / */\n"
			"fun()\n{\n/* block */\n}\n");
		CMetricsCalculator calc(str);
		calc.calculate_metrics();
		const QualityMetrics& qm(calc.get_metrics());
		CPPUNIT_ASSERT(qm.get_ncomment() == 2);
		CPPUNIT_ASSERT(qm.get_nfun_comment() == 1);

		std::stringstream str2("/* hi\n * / */ /**//* */");
		CMetricsCalculator calc2(str2);
		calc2.calculate_metrics();
		const QualityMetrics& qm2(calc2.get_metrics());
		CPPUNIT_ASSERT(qm2.get_ncomment() == 3);
	}

	void testNLineComment() {
		std::stringstream str(" // hi\n / /\n"
			"fun()\n{\n f // line /* */\n}\n");
		CMetricsCalculator calc(str);
		calc.calculate_metrics();
		const QualityMetrics& qm(calc.get_metrics());
		CPPUNIT_ASSERT(qm.get_ncomment() == 2);
		CPPUNIT_ASSERT(qm.get_nfun_comment() == 1);

		std::stringstream str2(" // hi\n //\n");
		CMetricsCalculator calc2(str2);
		calc2.calculate_metrics();
		const QualityMetrics& qm2(calc2.get_metrics());
		CPPUNIT_ASSERT(qm2.get_ncomment() == 2);
	}

	void testNBlockCommentChar() {
		std::stringstream str("/* hi\n * / */ /  there !\n");
		//                       123 456789
		CMetricsCalculator calc(str);
		calc.calculate_metrics();
		const QualityMetrics& qm(calc.get_metrics());
		CPPUNIT_ASSERT(qm.get_ncomment() == 1);
		CPPUNIT_ASSERT(qm.get_ncomment_char() == 9);
	}

	void testNLineCommentChar() {
		std::stringstream str("// hi  * /  there */\n");
		//                       123456789012345678
		CMetricsCalculator calc(str);
		calc.calculate_metrics();
		const QualityMetrics& qm(calc.get_metrics());
		CPPUNIT_ASSERT(qm.get_ncomment() == 1);
		CPPUNIT_ASSERT(qm.get_ncomment_char() == 18);
	}

	void testNCommentChar() {
		std::stringstream str("/* hi\n * / */ // there /*\n");
		//                       123 456789     012345678
		CMetricsCalculator calc(str);
		calc.calculate_metrics();
		const QualityMetrics& qm(calc.get_metrics());
		CPPUNIT_ASSERT(qm.get_ncomment() == 2);
		CPPUNIT_ASSERT(qm.get_ncomment_char() == 18);
	}

	void testCyclomaticBoolean() {
		std::stringstream str("foo()\n{a && b && d || c (?}\nstruct bar { a = d && e}");
		CMetricsCalculator calc(str);
		calc.calculate_metrics();
		const QualityMetrics& qm(calc.get_metrics());
		CPPUNIT_ASSERT(qm.get_cyclomatic().get_count() == 1);
		// One path plus four additional ones
		CPPUNIT_ASSERT(qm.get_cyclomatic().get_mean() == 5);
	}

	void testCyclomaticLogical() {
		std::stringstream str("foo()\n{for while case default if do switch}");
		CMetricsCalculator calc(str);
		calc.calculate_metrics();
		const QualityMetrics& qm(calc.get_metrics());
		// One path plus five additional ones
		CPPUNIT_ASSERT(qm.get_cyclomatic().get_mean() == 6);
	}

	void testCyclomaticLogicalTwoFunctions() {
		std::stringstream str("foo()\n{for (;;) {while () switch () {case 2: default:}} if (); do }"
			"bar()\n{ if }");
		CMetricsCalculator calc(str);
		calc.calculate_metrics();
		const QualityMetrics& qm(calc.get_metrics());
		// ((1 + 5) + (1 + 1))
		CPPUNIT_ASSERT(qm.get_cyclomatic().get_count() == 2);
		CPPUNIT_ASSERT(qm.get_cyclomatic().get_mean() == 4);
	}


	void testCyclomaticCombined() {
		std::stringstream str("foo()\n{for while case default}"
			"bar()\n{a && b && d}\nstruct bar { a = d && e}");
		CMetricsCalculator calc(str);
		calc.calculate_metrics();
		const QualityMetrics& qm(calc.get_metrics());
		CPPUNIT_ASSERT(qm.get_cyclomatic().get_count() == 2);
		// ((4 + 1) + (2 + 1)) / 2
		CPPUNIT_ASSERT(qm.get_cyclomatic().get_mean() == 4);
	}

	void testCppDirective() {
		std::stringstream str("#include <stdio.h>\n\t #define a #b\n#ifdef foo\n#if bar\n#elif k\n#endif\n"
			"foo()\n{\n#if FOO\n#undef a\n}");
		CMetricsCalculator calc(str);
		calc.calculate_metrics();
		const QualityMetrics& qm(calc.get_metrics());
		CPPUNIT_ASSERT(qm.get_ncpp_directive() == 8);
		CPPUNIT_ASSERT(qm.get_nfun_cpp_directive() == 2);
		CPPUNIT_ASSERT(qm.get_ncpp_conditional() == 4);
		CPPUNIT_ASSERT(qm.get_nfun_cpp_conditional() == 1);
		CPPUNIT_ASSERT(qm.get_ncpp_include() == 1);
	}

	void testCKeyword() {
		std::stringstream str("typedef int a; typedef double b; typedef short s;\ngoto c; goto d;");
		CMetricsCalculator calc(str);
		calc.calculate_metrics();
		const QualityMetrics& qm(calc.get_metrics());
		CPPUNIT_ASSERT(qm.get_ngoto() == 2);
		CPPUNIT_ASSERT(qm.get_ntypedef() == 3);
	}

	void testIdentifierLength() {
		std::stringstream str("#define aa bab(123)\n if (aa == b)");
		CMetricsCalculator calc(str);
		calc.calculate_metrics();
		const QualityMetrics& qm(calc.get_metrics());
		CPPUNIT_ASSERT(qm.get_identifier_length().get_count() == 4);
		// 2 + 3 + 2 + 1 == 8; 8 / 4 = 2
		CPPUNIT_ASSERT(qm.get_identifier_length().get_mean() == 2);
		CPPUNIT_ASSERT(qm.get_unique_identifier_length().get_count() == 3);
		// 2 + 3 + 1 == 6; 6 / 3 = 2
		CPPUNIT_ASSERT(qm.get_unique_identifier_length().get_mean() == 2);
	}
};
#endif /*  CMETRICSCALCULATORTEST_H */
