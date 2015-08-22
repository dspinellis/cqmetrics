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
	CPPUNIT_TEST(testInternal);
	CPPUNIT_TEST(testCKeyword);
	CPPUNIT_TEST(testCKeyword2);
	CPPUNIT_TEST(testCKeyword3);
	CPPUNIT_TEST(testCKeyword4);
	CPPUNIT_TEST(testIdentifierLength);
	CPPUNIT_TEST(testStyle);
	CPPUNIT_TEST(testBinaryOperatorStyle);
	CPPUNIT_TEST(testKeywordStyle);
	CPPUNIT_TEST(testDeclKeywordStyle);
	CPPUNIT_TEST(testKeywordStyleLeft);
	CPPUNIT_TEST(testIndentationSimple);
	CPPUNIT_TEST(testIndentationIf);
	CPPUNIT_TEST(testIndentationBrace);
	CPPUNIT_TEST(testIndentationGnuBrace);
	CPPUNIT_TEST(testIfFor);
	CPPUNIT_TEST(testLineComment);
	CPPUNIT_TEST(testDoxComment);
	CPPUNIT_TEST(testCppLine);
	CPPUNIT_TEST(testCppCommentLine);
	CPPUNIT_TEST(testElseCppCommentLine);
	CPPUNIT_TEST(testBlockCommentIndent);
	CPPUNIT_TEST(testIfElseCppComment);
	CPPUNIT_TEST(testBracedAssignment);
	CPPUNIT_TEST(testElseComment);
	CPPUNIT_TEST(testElseNoIf);
	CPPUNIT_TEST(testInconsistency);
	CPPUNIT_TEST_SUITE_END();
public:
	void testCtor() {
		CMetricsCalculator calc;

		const QualityMetrics& qm(calc.get_metrics());
		CPPUNIT_ASSERT(qm.get_nchar() == 0);
		CPPUNIT_ASSERT(qm.get_line_length().get_count() == 0);
		CPPUNIT_ASSERT(qm.get_line_length().get_sum() == 0);
	}

	void testLine() {
		std::stringstream str("hi\nt-\naa");
		CMetricsCalculator calc(str);
		calc.calculate_metrics();
		const QualityMetrics& qm(calc.get_metrics());
		CPPUNIT_ASSERT(qm.get_nchar() == 8);
		CPPUNIT_ASSERT(qm.get_line_length().get_count() == 3);
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
		std::stringstream str("foo()\n{a;if (1) {b; if (2) c;}}");
		CMetricsCalculator calc(str);
		calc.calculate_metrics();
		const QualityMetrics& qm(calc.get_metrics());
		CPPUNIT_ASSERT(qm.get_statement_nesting().get_count() == 3);
		// (0 + 1 + 2) / 3 == 1
		CPPUNIT_ASSERT(qm.get_statement_nesting().get_mean() == 1);
	}

	void testStatementNestingTwoFunction() {
		std::stringstream str("foo()\n{a;if(1) if(2) c;\n}\n"
				       //      0             3
			"bar()\n{if(1){a;if(1){b;if(1){if(1){if(1){c;}}} d; }}}");
			//             1       2                   6     2
		CMetricsCalculator calc(str);
		calc.calculate_metrics();
		const QualityMetrics& qm(calc.get_metrics());
		CPPUNIT_ASSERT(qm.get_statement_nesting().get_count() == 6);
		// (0 + 3 + 1 + 2 + 6 + 2)  == 12; 12 / 6 == 2
		CPPUNIT_ASSERT(qm.get_statement_nesting().get_mean() == 2);
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

	void testDoxComment() {
		std::stringstream str("/** hi\n */\n/* no dox */\n/// foo\n// No dox\n");
		CMetricsCalculator calc(str);
		calc.calculate_metrics();
		const QualityMetrics& qm(calc.get_metrics());
		CPPUNIT_ASSERT(qm.get_ndox_comment() == 2);
		CPPUNIT_ASSERT(qm.get_ncomment() == 4);

		std::stringstream str2("///Hi\n");
		CMetricsCalculator calc2(str2);
		calc2.calculate_metrics();
		const QualityMetrics& qm2(calc2.get_metrics());
		CPPUNIT_ASSERT(qm2.get_ndox_comment_char() == 3);

		std::stringstream str3("/**hi*/\n");
		CMetricsCalculator calc3(str3);
		calc3.calculate_metrics();
		const QualityMetrics& qm3(calc3.get_metrics());
		CPPUNIT_ASSERT(qm3.get_ndox_comment_char() == 2);
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

	void testInternal() {
		std::stringstream str("static a; static b;\nint\nfoo()\n{\n\tstatic d;\n}\n" );
		CMetricsCalculator calc(str);
		calc.calculate_metrics();
		const QualityMetrics& qm(calc.get_metrics());
		CPPUNIT_ASSERT(qm.get_ninternal() == 2);
	}

	void testCKeyword() {
		std::stringstream str("register x; typedef int a; typedef double b; typedef short s;\ngoto c; goto d; void, void, void, void;" );
		CMetricsCalculator calc(str);
		calc.calculate_metrics();
		const QualityMetrics& qm(calc.get_metrics());
		CPPUNIT_ASSERT(qm.get_ngoto() == 2);
		CPPUNIT_ASSERT(qm.get_ntypedef() == 3);
		CPPUNIT_ASSERT(qm.get_nregister() == 1);
		CPPUNIT_ASSERT(qm.get_nvoid() == 4);
	}

	void testCKeyword2() {
		std::stringstream str("signed unsigned unsigned const const const volatile volatile volatile volatile noalias noalias noalias noalias noalias;" );
		CMetricsCalculator calc(str);
		calc.calculate_metrics();
		const QualityMetrics& qm(calc.get_metrics());
		CPPUNIT_ASSERT(qm.get_nsigned() == 1);
		CPPUNIT_ASSERT(qm.get_nunsigned() == 2);
		CPPUNIT_ASSERT(qm.get_nconst() == 3);
		CPPUNIT_ASSERT(qm.get_nvolatile() == 4);
		CPPUNIT_ASSERT(qm.get_nnoalias() == 5);
	}

	void testCKeyword3() {
		std::stringstream str("struct union union enum enum enum;" );
		CMetricsCalculator calc(str);
		calc.calculate_metrics();
		const QualityMetrics& qm(calc.get_metrics());
		CPPUNIT_ASSERT(qm.get_nstruct() == 1);
		CPPUNIT_ASSERT(qm.get_nunion() == 2);
		CPPUNIT_ASSERT(qm.get_nenum() == 3);
	}

	void testCKeyword4() {
		std::stringstream str("restrict inline inline;" );
		CMetricsCalculator calc(str);
		calc.calculate_metrics();
		const QualityMetrics& qm(calc.get_metrics());
		CPPUNIT_ASSERT(qm.get_nrestrict() == 1);
		CPPUNIT_ASSERT(qm.get_ninline() == 2);
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

	struct PSTest {
		const char *prefix;
		const char *suffix;
		enum QualityMetrics::StyleHint e;
		int result;
	};

	const std::string message(struct PSTest *t) {
		std::stringstream m;
		m << "Prefix \"" << t->prefix <<
			"\" Suffix: \"" << t->suffix <<
			"\" Hint: " << QualityMetrics::metric_name[t->e] <<
			" Expected: " << t->result << ' ';
		return m.str();
	}

	void testPrefixSuffix(const char *strings[], struct PSTest test[]) {
		for (const char **o = strings; *o; o++)
			for (struct PSTest *t = test; t->prefix; t++) {
				std::string code(t->prefix);
				code.append(*o);
				code.append(t->suffix);
				std::stringstream str(code);
				CMetricsCalculator calc(str);
				calc.calculate_metrics();
				const QualityMetrics& qm(calc.get_metrics());
				CPPUNIT_ASSERT_MESSAGE(message(t),
						qm.get_style_hint(t->e) == t->result);
			}
	}

	void testBinaryOperatorStyle() {
		const char *binary_operator[] = {
			// By order of precedence from manual, to verify we
			// got them all. (Excluding those we can't handle.)
			"/", "%", "<<", ">>", "<", "<=", ">", ">=", "==", "!=",
			"^", "|", "&&", "||", "?", "=", "+=", "-=", "*=", "/=",
			"%=", "<<=", ">>=", "&=", "^=", "|=",
			NULL
		};

		struct PSTest test[] = {
		  { "a ", " b", QualityMetrics::NO_SPACE_AFTER_BINARY_OP, 0 },
		  { "a ", "b", QualityMetrics::NO_SPACE_AFTER_BINARY_OP, 1 },
		  { "a ", " b", QualityMetrics::SPACE_AFTER_BINARY_OP, 1 },
		  { "a ", "b", QualityMetrics::SPACE_AFTER_BINARY_OP, 0 },
		  { "a ", " b", QualityMetrics::NO_SPACE_BEFORE_BINARY_OP, 0 },
		  { "a", " b", QualityMetrics::NO_SPACE_BEFORE_BINARY_OP, 1 },
		  { "a ", " b", QualityMetrics::SPACE_BEFORE_BINARY_OP, 1 },
		  { "a", " b", QualityMetrics::SPACE_BEFORE_BINARY_OP, 0 },
		  { "a ", " b", QualityMetrics::SPACE_BEFORE_BINARY_OP, 1 },
		  { "a", " b", QualityMetrics::SPACE_BEFORE_BINARY_OP, 0 },
		  NULL
		};

		testPrefixSuffix(binary_operator, test);
	}

	void testKeywordStyle() {
		const char *keyword[] = {
			// By alphabetic order from manual, to verify we
			// got them all. (Excluding those we can't handle.)
			"case", "do", "else", "for", "goto", "if", "switch",
			"while",
			NULL
		};

		struct PSTest test[] = {
		  { "; ", " (1)", QualityMetrics::NO_SPACE_BEFORE_KEYWORD, 0 },
		  { ";",  " (1)", QualityMetrics::NO_SPACE_BEFORE_KEYWORD, 1 },
		  { "; ", " (1)", QualityMetrics::NO_SPACE_AFTER_KEYWORD, 0 },
		  { "; ", "(1)", QualityMetrics::NO_SPACE_AFTER_KEYWORD, 1 },
		  { " #include <", ">", QualityMetrics::NO_SPACE_BEFORE_KEYWORD, 0 },
		  { " #define <", ">", QualityMetrics::NO_SPACE_AFTER_KEYWORD, 0 },
		  { "; ", " (1)", QualityMetrics::SPACE_BEFORE_KEYWORD, 1 },
		  { ";",  " (1)", QualityMetrics::SPACE_BEFORE_KEYWORD, 0 },
		  { "; ", " (1)", QualityMetrics::SPACE_AFTER_KEYWORD, 1 },
		  { "; ", "(1)", QualityMetrics::SPACE_AFTER_KEYWORD, 0 },
		  { " #include <", ">", QualityMetrics::SPACE_BEFORE_KEYWORD, 0 },
		  { " #define <", ">", QualityMetrics::SPACE_AFTER_KEYWORD, 0 },
		  NULL
		};

		testPrefixSuffix(keyword, test);
	}

	void testDeclKeywordStyle() {
		const char *keyword[] = {
			"enum", "struct", "union",
			NULL
		};

		struct PSTest test[] = {
		  { "; ", " (1)", QualityMetrics::NO_SPACE_BEFORE_KEYWORD, 0 },
		  { "foo(", " (1)", QualityMetrics::NO_SPACE_BEFORE_KEYWORD, 0 },
		  { ";",  " (1)", QualityMetrics::NO_SPACE_BEFORE_KEYWORD, 1 },
		  { "; ", " (1)", QualityMetrics::NO_SPACE_AFTER_KEYWORD, 0 },
		  { "; ", "(1)", QualityMetrics::NO_SPACE_AFTER_KEYWORD, 1 },
		  { " #include <", ">", QualityMetrics::NO_SPACE_BEFORE_KEYWORD, 0 },
		  { " #define <", ">", QualityMetrics::NO_SPACE_AFTER_KEYWORD, 0 },
		  { "; ", " (1)", QualityMetrics::SPACE_BEFORE_KEYWORD, 1 },
		  { "foo(", " (1)", QualityMetrics::SPACE_BEFORE_KEYWORD, 0 },
		  { ";",  " (1)", QualityMetrics::SPACE_BEFORE_KEYWORD, 0 },
		  { "; ", " (1)", QualityMetrics::SPACE_AFTER_KEYWORD, 1 },
		  { "; ", "(1)", QualityMetrics::SPACE_AFTER_KEYWORD, 0 },
		  { " #include <", ">", QualityMetrics::SPACE_BEFORE_KEYWORD, 0 },
		  { " #define <", ">", QualityMetrics::SPACE_AFTER_KEYWORD, 0 },
		  NULL
		};

		testPrefixSuffix(keyword, test);
	}

	void testKeywordStyleLeft() {
		const char *keyword[] = {
			// By alphabetic order from manual, to verify we
			// got them all. (Excluding those we can't handle.)
			"break", "continue", "default", "return",
			NULL
		};

		struct PSTest test[] = {
		  { "; ", " (1)", QualityMetrics::NO_SPACE_BEFORE_KEYWORD, 0 },
		  { ";",  " (1)", QualityMetrics::NO_SPACE_BEFORE_KEYWORD, 1 },
		  { "; ", " (1)", QualityMetrics::NO_SPACE_AFTER_KEYWORD, 0 },
		  { "; ", "(1)", QualityMetrics::NO_SPACE_AFTER_KEYWORD, 0 },
		  { " #include <", ">", QualityMetrics::NO_SPACE_BEFORE_KEYWORD, 0 },
		  { " #define <", ">", QualityMetrics::NO_SPACE_AFTER_KEYWORD, 0 },
		  { "; ", " (1)", QualityMetrics::SPACE_BEFORE_KEYWORD, 1 },
		  { ";",  " (1)", QualityMetrics::SPACE_BEFORE_KEYWORD, 0 },
		  { "; ", " (1)", QualityMetrics::SPACE_AFTER_KEYWORD, 0 },
		  { "; ", "(1)", QualityMetrics::SPACE_AFTER_KEYWORD, 0 },
		  { " #include <", ">", QualityMetrics::SPACE_BEFORE_KEYWORD, 0 },
		  { " #define <", ">", QualityMetrics::SPACE_AFTER_KEYWORD, 0 },
		  NULL
		};

		testPrefixSuffix(keyword, test);
	}

	struct Test {
		const char *code;
		enum QualityMetrics::StyleHint e;
		int result;
	};

	const std::string message(struct Test *t) {
		std::stringstream m;
		m << "Code \"" << t->code << "\" Hint: " <<
			QualityMetrics::metric_name[t->e] <<
			" Expected: " << t->result << ' ';
		return m.str();
	}

	void testStyle() {
		struct Test test[] = {
{ "a[4]", QualityMetrics::SPACE_BEFORE_OPENING_SQUARE_BRACKET, 0 },
{ "a [4]", QualityMetrics::SPACE_BEFORE_OPENING_SQUARE_BRACKET, 1 },
{ "a[4]", QualityMetrics::NO_SPACE_BEFORE_OPENING_SQUARE_BRACKET, 1 },
{ "a [4]", QualityMetrics::NO_SPACE_BEFORE_OPENING_SQUARE_BRACKET, 0 },
{ "a[4]", QualityMetrics::SPACE_AFTER_OPENING_SQUARE_BRACKET, 0 },
{ "a[ 4]", QualityMetrics::SPACE_AFTER_OPENING_SQUARE_BRACKET, 1 },
{ "a[4]", QualityMetrics::NO_SPACE_AFTER_OPENING_SQUARE_BRACKET, 1 },
{ "a[ 4]", QualityMetrics::NO_SPACE_AFTER_OPENING_SQUARE_BRACKET, 0 },
{ "~0xff", QualityMetrics::SPACE_AFTER_UNARY_OP, 0 },
{ "~ 0xff", QualityMetrics::SPACE_AFTER_UNARY_OP, 1 },
{ "~0xff", QualityMetrics::NO_SPACE_AFTER_UNARY_OP, 1 },
{ "~ 0xff", QualityMetrics::NO_SPACE_AFTER_UNARY_OP, 0 },
{ "a, b", QualityMetrics::SPACE_BEFORE_COMMA, 0 },
{ "a , b", QualityMetrics::SPACE_BEFORE_COMMA, 1 },
{ "a, b", QualityMetrics::NO_SPACE_BEFORE_COMMA, 1 },
{ "a , b", QualityMetrics::NO_SPACE_BEFORE_COMMA, 0 },
{ "a, b", QualityMetrics::NO_SPACE_AFTER_COMMA, 0 },
{ "a,b", QualityMetrics::NO_SPACE_AFTER_COMMA, 1 },
{ "a, b", QualityMetrics::SPACE_AFTER_COMMA, 1 },
{ "a,b", QualityMetrics::SPACE_AFTER_COMMA, 0 },
{ "foo(int a)", QualityMetrics::SPACE_BEFORE_CLOSING_BRACKET, 0 },
{ "foo(int a )", QualityMetrics::SPACE_BEFORE_CLOSING_BRACKET, 1 },
{ "foo(int a)", QualityMetrics::NO_SPACE_BEFORE_CLOSING_BRACKET, 1 },
{ "foo(int a )", QualityMetrics::NO_SPACE_BEFORE_CLOSING_BRACKET, 0 },
{ "foo()\n{x;\n}", QualityMetrics::NO_SPACE_BEFORE_OPENING_BRACE, 0 },
{ "foo[a]", QualityMetrics::SPACE_BEFORE_CLOSING_SQUARE_BRACKET, 0 },
{ "foo[a ]", QualityMetrics::SPACE_BEFORE_CLOSING_SQUARE_BRACKET, 1 },
{ "foo[a]", QualityMetrics::NO_SPACE_BEFORE_CLOSING_SQUARE_BRACKET, 1 },
{ "foo[a ]", QualityMetrics::NO_SPACE_BEFORE_CLOSING_SQUARE_BRACKET, 0 },
{ "foo()\n{x;\n}", QualityMetrics::NO_SPACE_BEFORE_OPENING_BRACE, 0 },
{ "foo()\n{x;\n}", QualityMetrics::SPACE_BEFORE_OPENING_BRACE, 0 },
{ "foo()\n{\nif (1){x;}}\n", QualityMetrics::SPACE_BEFORE_OPENING_BRACE, 0 },
{ "foo()\n{\nif (1) {x;}}\n", QualityMetrics::SPACE_BEFORE_OPENING_BRACE, 1 },
{ "foo()\n{\nx;\n}", QualityMetrics::NO_SPACE_AFTER_OPENING_BRACE, 0 },
{ "foo()\n{\nif (1){x;}}\n", QualityMetrics::NO_SPACE_AFTER_OPENING_BRACE, 1 },
{ "foo()\n{\nx;\n}", QualityMetrics::SPACE_AFTER_OPENING_BRACE, 0 },
{ "foo()\n{\nif (1){ x;}}\n", QualityMetrics::SPACE_AFTER_OPENING_BRACE, 1 },
{ "foo()\n{x;\n}", QualityMetrics::NO_SPACE_BEFORE_CLOSING_BRACE, 0 },
{ "foo()\n{x;\n}", QualityMetrics::SPACE_BEFORE_CLOSING_BRACE, 0 },
{ "foo()\n{\nx;}\n", QualityMetrics::NO_SPACE_BEFORE_CLOSING_BRACE, 1 },
{ "foo()\n{\nx;}\n", QualityMetrics::SPACE_BEFORE_CLOSING_BRACE, 0 },
{ "foo()\n{\nx; }\n", QualityMetrics::SPACE_BEFORE_CLOSING_BRACE, 1 },
{ "foo()\n{x;\n}\n", QualityMetrics::NO_SPACE_AFTER_CLOSING_BRACE, 0 },
{ "foo()\n{x;\n}\n", QualityMetrics::SPACE_AFTER_CLOSING_BRACE, 0 },
{ "foo()\n{x;\n}\n", QualityMetrics::SPACE_AFTER_CLOSING_BRACE, 0 },
{ "foo()\n{\nif(1){foo;}}\n", QualityMetrics::NO_SPACE_AFTER_CLOSING_BRACE, 1 },
{ "foo()\n{\nif(1){foo;}}\n", QualityMetrics::SPACE_AFTER_CLOSING_BRACE, 0 },
{ "foo()\n{\nif(1){foo;} }\n", QualityMetrics::SPACE_AFTER_CLOSING_BRACE, 1 },
{ "int a;", QualityMetrics::SPACE_BEFORE_SEMICOLON, 0 },
{ "int a ;", QualityMetrics::SPACE_BEFORE_SEMICOLON, 1 },
{ " \t;", QualityMetrics::SPACE_BEFORE_SEMICOLON, 0 },
{ "int a;", QualityMetrics::NO_SPACE_BEFORE_SEMICOLON, 1 },
{ "int a ;", QualityMetrics::NO_SPACE_BEFORE_SEMICOLON, 0 },
{ " \t;", QualityMetrics::NO_SPACE_BEFORE_SEMICOLON, 0 },
{ "int a; int b;\n", QualityMetrics::NO_SPACE_AFTER_SEMICOLON, 0 },
{ "int a;int b;\n", QualityMetrics::NO_SPACE_AFTER_SEMICOLON, 1 },
{ "int a; int b;\n", QualityMetrics::SPACE_AFTER_SEMICOLON, 1 },
{ "int a;int b;\n", QualityMetrics::SPACE_AFTER_SEMICOLON, 0 },
{ "a->b", QualityMetrics::SPACE_BEFORE_STRUCT_OP, 0 },
{ "a ->b", QualityMetrics::SPACE_BEFORE_STRUCT_OP, 1 },
{ "a->b", QualityMetrics::NO_SPACE_BEFORE_STRUCT_OP, 1 },
{ "a ->b", QualityMetrics::NO_SPACE_BEFORE_STRUCT_OP, 0 },
{ "a->b", QualityMetrics::SPACE_AFTER_STRUCT_OP, 0 },
{ "a->b", QualityMetrics::NO_SPACE_AFTER_STRUCT_OP, 1 },
{ "a-> b", QualityMetrics::SPACE_AFTER_STRUCT_OP, 1 },
{ "a-> b", QualityMetrics::NO_SPACE_AFTER_STRUCT_OP, 0 },
{ "a.b", QualityMetrics::SPACE_BEFORE_STRUCT_OP, 0 },
{ "a .b", QualityMetrics::SPACE_BEFORE_STRUCT_OP, 1 },
{ "a.b", QualityMetrics::NO_SPACE_BEFORE_STRUCT_OP, 1 },
{ "a .b", QualityMetrics::NO_SPACE_BEFORE_STRUCT_OP, 0 },
{ "a.b", QualityMetrics::SPACE_AFTER_STRUCT_OP, 0 },
{ "a.b", QualityMetrics::NO_SPACE_AFTER_STRUCT_OP, 1 },
{ "a. b", QualityMetrics::SPACE_AFTER_STRUCT_OP, 1 },
{ "a. b", QualityMetrics::NO_SPACE_AFTER_STRUCT_OP, 0 },
{ "a ? b : c", QualityMetrics::NO_SPACE_AFTER_BINARY_OP, 0 },
{ "a ? b :c", QualityMetrics::NO_SPACE_AFTER_BINARY_OP, 1 },
{ "a ? b : c", QualityMetrics::SPACE_AFTER_BINARY_OP, 2 },
{ "a ?b :c", QualityMetrics::SPACE_AFTER_BINARY_OP, 0 },
{ "case 42:\n", QualityMetrics::NO_SPACE_BEFORE_BINARY_OP, 0 },
{ "case 42 :\n", QualityMetrics::SPACE_BEFORE_BINARY_OP, 0 },
{ "!a", QualityMetrics::SPACE_AFTER_UNARY_OP, 0 },
{ "! a", QualityMetrics::SPACE_AFTER_UNARY_OP, 1 },
{ "!a", QualityMetrics::NO_SPACE_AFTER_UNARY_OP, 1 },
{ "! a", QualityMetrics::NO_SPACE_AFTER_UNARY_OP, 0 },
{ "\nreturn;", QualityMetrics::NO_SPACE_BEFORE_KEYWORD, 0 },
{ ";return;", QualityMetrics::NO_SPACE_BEFORE_KEYWORD, 1 },
{ ";return;", QualityMetrics::SPACE_AT_END_OF_LINE, 0 },
{ ";return; \n", QualityMetrics::SPACE_AT_END_OF_LINE, 1 },
		  NULL
		};

		for (struct Test *t = test; t->code; t++) {
			std::stringstream str(t->code);
			CMetricsCalculator calc(str);
			calc.calculate_metrics();
			const QualityMetrics& qm(calc.get_metrics());
			CPPUNIT_ASSERT_MESSAGE(message(t),
					qm.get_style_hint(t->e) == t->result);
		}
	}

	void testIndentationSimple() {
		std::stringstream str("foo()\n{\n\treturn;\n}");
		CMetricsCalculator calc(str);
		calc.calculate_metrics();
		const QualityMetrics& qm(calc.get_metrics());
		CPPUNIT_ASSERT(qm.get_indentation_spacing().get_count() == 1);
		CPPUNIT_ASSERT(qm.get_indentation_spacing().get_mean() == 8);
		CPPUNIT_ASSERT(qm.get_indentation_spacing().get_standard_deviation() == 0);
	}

	void testIndentationIf() {
		std::stringstream str("foo()\n{\n\tif (a)\n\t\treturn;\n}");
		CMetricsCalculator calc(str);
		calc.calculate_metrics();
		const QualityMetrics& qm(calc.get_metrics());
		CPPUNIT_ASSERT(qm.get_indentation_spacing().get_count() == 2);
		CPPUNIT_ASSERT(qm.get_indentation_spacing().get_mean() == 8);
		CPPUNIT_ASSERT(qm.get_indentation_spacing().get_standard_deviation() == 0);
	}

	void testIndentationBrace() {
		std::stringstream str("foo()\n{\n\tif (a) { \n\t\treturn;\n\t}\n}\n");
		CMetricsCalculator calc(str);
		calc.calculate_metrics();
		const QualityMetrics& qm(calc.get_metrics());
		CPPUNIT_ASSERT(qm.get_indentation_spacing().get_count() == 3);
		CPPUNIT_ASSERT(qm.get_indentation_spacing().get_mean() == 8);
		CPPUNIT_ASSERT(qm.get_indentation_spacing().get_standard_deviation() == 0);
	}

	void testIndentationGnuBrace() {
		std::stringstream str("foo()\n{\n  if (a)\n    {\n      return;\n    }\n}\n");
		CMetricsCalculator calc(str);
		calc.calculate_metrics();
		const QualityMetrics& qm(calc.get_metrics());
		CPPUNIT_ASSERT(qm.get_indentation_spacing().get_count() == 4);
		CPPUNIT_ASSERT(qm.get_indentation_spacing().get_mean() == 2);
		CPPUNIT_ASSERT(qm.get_indentation_spacing().get_standard_deviation() == 0);
	}

	// Written to find implementation error
	void testIfFor() {
		std::stringstream str("foo()\n{\n\tif (a)\n\t\tfor (;;)\n\t\t\tbar();\n}\n");
		CMetricsCalculator calc(str);
		calc.calculate_metrics();
		const QualityMetrics& qm(calc.get_metrics());
		CPPUNIT_ASSERT(qm.get_indentation_spacing().get_count() == 3);
		CPPUNIT_ASSERT(qm.get_indentation_spacing().get_mean() == 8);
		CPPUNIT_ASSERT(qm.get_indentation_spacing().get_standard_deviation() == 0);
	}

	// Written to investigate implementation error
	void testLineComment() {
		std::stringstream str("foo()\n{\n\tif (x) {\n\t\t// foo\n\t\tbar();\n\t}\n}\n");
		CMetricsCalculator calc(str);
		calc.calculate_metrics();
		const QualityMetrics& qm(calc.get_metrics());
		CPPUNIT_ASSERT(qm.get_indentation_spacing().get_count() == 4);
		CPPUNIT_ASSERT(qm.get_indentation_spacing().get_mean() == 8);
		CPPUNIT_ASSERT(qm.get_indentation_spacing().get_standard_deviation() == 0);
	}

	// Written to investigate implementation error
	void testCppLine() {
		std::stringstream str("foo()\n{\n\tif (x) {\n\t\t#define x 1\n\t\tbar();\n\t}\n}\n");
		CMetricsCalculator calc(str);
		calc.calculate_metrics();
		const QualityMetrics& qm(calc.get_metrics());
		CPPUNIT_ASSERT(qm.get_indentation_spacing().get_count() == 3);
		CPPUNIT_ASSERT(qm.get_indentation_spacing().get_mean() == 8);
		CPPUNIT_ASSERT(qm.get_indentation_spacing().get_standard_deviation() == 0);
	}

	// Written to investigate implementation error
	void testCppCommentLine() {
		std::stringstream str("foo()\n{\n\tif (x) {\n\t\t#define x 1\n\t\t// Comment\n\t\tbar();\n\t}\n}\n");
		CMetricsCalculator calc(str);
		calc.calculate_metrics();
		const QualityMetrics& qm(calc.get_metrics());
		CPPUNIT_ASSERT(qm.get_indentation_spacing().get_count() == 4);
		CPPUNIT_ASSERT(qm.get_indentation_spacing().get_mean() == 8);
		CPPUNIT_ASSERT(qm.get_indentation_spacing().get_standard_deviation() == 0);
	}

	// Written to investigate implementation error
	void testElseCppCommentLine() {
		std::stringstream str("foo()\n{\n\tif (x) {\n\t\tfoo();\n\t} else {\n#define x 1\n\t\t// Comment\n\t\tbar();\n\t}\n}\n");
		CMetricsCalculator calc(str);
		calc.calculate_metrics();
		const QualityMetrics& qm(calc.get_metrics());
		CPPUNIT_ASSERT(qm.get_indentation_spacing().get_count() == 6);
		CPPUNIT_ASSERT(qm.get_indentation_spacing().get_mean() == 8);
		CPPUNIT_ASSERT(qm.get_indentation_spacing().get_standard_deviation() == 0);
	}

	void testBlockCommentIndent() {
		std::stringstream str("foo()\n{\n\tif (x)\n\t\t/*\n\t\t * comment\n\t\t */\n\t\tfoo();\n}\n");
		CMetricsCalculator calc(str);
		calc.calculate_metrics();
		const QualityMetrics& qm(calc.get_metrics());
		CPPUNIT_ASSERT(qm.get_indentation_spacing().get_count() == 2);
		CPPUNIT_ASSERT(qm.get_indentation_spacing().get_mean() == 8);
		CPPUNIT_ASSERT(qm.get_indentation_spacing().get_standard_deviation() == 0);
	}

	// Added after problem that set indentation level to -1
	void testIfElseCppComment() {
		std::stringstream str("foo()\n{\n\tif (x) {\n#if x\n\t\t\n\t\tfoo();\n#else\n\t\tbar();\n#endif\n\t}\n");
		CMetricsCalculator calc(str);
		calc.calculate_metrics();
		const QualityMetrics& qm(calc.get_metrics());
		CPPUNIT_ASSERT(qm.get_indentation_spacing().get_count() == 5);
		CPPUNIT_ASSERT(qm.get_indentation_spacing().get_mean() == 8);
		CPPUNIT_ASSERT(qm.get_indentation_spacing().get_standard_deviation() == 0);
	}

	// Added after problem that set indentation level to -1
	void testBracedAssignment() {
		std::stringstream str("foo()\n{\n\tx[] = {3};\n\tfoo();\n}\n");
		CMetricsCalculator calc(str);
		calc.calculate_metrics();
		const QualityMetrics& qm(calc.get_metrics());
		CPPUNIT_ASSERT(qm.get_indentation_spacing().get_count() == 2);
		CPPUNIT_ASSERT(qm.get_indentation_spacing().get_mean() == 8);
		CPPUNIT_ASSERT(qm.get_indentation_spacing().get_standard_deviation() == 0);
	}

	// Added after problem that set indentation level to -1
	void testElseComment() {
		std::stringstream str("foo()\n{\n\tif(x)\n\t\tfor (;;) foo();\n\telse\n\t\tbar();\n}\n");
		CMetricsCalculator calc(str);
		calc.calculate_metrics();
		const QualityMetrics& qm(calc.get_metrics());
		CPPUNIT_ASSERT(qm.get_indentation_spacing().get_count() == 4);
		CPPUNIT_ASSERT(qm.get_indentation_spacing().get_mean() == 8);
		CPPUNIT_ASSERT(qm.get_indentation_spacing().get_standard_deviation() == 0);
	}

	// Added after problem that set indentation level to -1
	void testElseNoIf() {
		std::stringstream str("foo()\n{\n\tIF(x)\n\t\tfoo();\n\telse\n\t\tbar();\n}\n");
		CMetricsCalculator calc(str);
		calc.calculate_metrics();
		const QualityMetrics& qm(calc.get_metrics());
		CPPUNIT_ASSERT(qm.get_indentation_spacing().get_count() == 3);
		CPPUNIT_ASSERT(qm.get_indentation_spacing().get_mean() == 8);
		CPPUNIT_ASSERT(qm.get_indentation_spacing().get_standard_deviation() == 0);
	}

	void testInconsistency() {
		std::stringstream str("foo()\n{\n"
				"foo(a,b);\n"
				"foo(a,b) ;\n"
				"foo(a, b);;;;\n"
			"}\n");
		CMetricsCalculator calc(str);
		calc.calculate_metrics();
		const QualityMetrics& qm(calc.get_metrics());
		/*
		 * (NO)SPACE_AFTER_COMMA 3 (1 inconistent)
		 * (NO)SPACE_BEFORE_CLOSING_BRACKET 4
		 * (NO)SPACE_BEFORE_COMMA 3
		 * (NO)SPACE_BEFORE_SEMICOLON 6 (1 inconsistent)
		 * Total=16
		 */
		CPPUNIT_ASSERT(qm.get_style_inconsistency() == 2 / 16.);
	}
};
#endif /*  CMETRICSCALCULATORTEST_H */
