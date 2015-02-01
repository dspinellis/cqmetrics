#ifndef NESTINGLEVELTEST_H
#define NESTINGLEVELTEST_H

#include <ostream>

#include <cppunit/extensions/HelperMacros.h>

#include "NestingLevel.h"

class NestingLevelTest : public CppUnit::TestFixture  {
	CPPUNIT_TEST_SUITE(NestingLevelTest);
	CPPUNIT_TEST(testCtor);
	CPPUNIT_TEST(testReset);
	CPPUNIT_TEST(testSingleIf);
	CPPUNIT_TEST(testDouble);
	CPPUNIT_TEST(testTrivialBrace);
	CPPUNIT_TEST(testDoubleTrivialBrace);
	CPPUNIT_TEST(testSimpleBrace);
	CPPUNIT_TEST(testNestingBrace);
	CPPUNIT_TEST(testDoubleNestingBrace);
	CPPUNIT_TEST(testBraceProtection);
	CPPUNIT_TEST(testDoubleBraceProtection);
	CPPUNIT_TEST(testIfElseBrace);
	CPPUNIT_TEST(testIfElse);
	CPPUNIT_TEST(testBracedIfElse);
	CPPUNIT_TEST(testGnuBracedIfElse);
	CPPUNIT_TEST(testIfIfElse);
	CPPUNIT_TEST(testElseIf);
	CPPUNIT_TEST(testBracedElseIf);
	CPPUNIT_TEST(testBracedElseIfElse);
	CPPUNIT_TEST(testComplexBracedElseIf);
	CPPUNIT_TEST(testSimpleDo);
	CPPUNIT_TEST(testBracedDo);
	CPPUNIT_TEST(testBracedDoWhile);
	CPPUNIT_TEST(testUnbracedDoWhile);
	CPPUNIT_TEST(testUnbracedDoNestedWhile);
	CPPUNIT_TEST(testUnbracedNestedDoNestedWhile);
	CPPUNIT_TEST(testSingleWhile);
	CPPUNIT_TEST(testAllStatements);
	CPPUNIT_TEST(testBracedAssignment);
	CPPUNIT_TEST(testIfForElse);
	CPPUNIT_TEST(testElseElse);
	CPPUNIT_TEST_SUITE_END();
public:
	void testCtor() {
		NestingLevel n;
		CPPUNIT_ASSERT(n.get_nesting_level() == 0);
	}

	// }}}
	// Uncovered implementation error
	void testReset() {
		NestingLevel n;
		n.saw_close_brace();
		n.saw_close_brace();
		n.saw_close_brace();
		CPPUNIT_ASSERT(n.get_nesting_level() == 0);
	}

	// if (x) 1;
	void testSingleIf() {
		NestingLevel n;
		n.saw_nesting_keyword(CKeyword::IF);
		CPPUNIT_ASSERT(n.get_nesting_level() == 1);
		n.saw_statement_semicolon();
		CPPUNIT_ASSERT(n.get_nesting_level() == 0);
	}

	// if (x) if (y) 2; 0;
	// Uncovered implementation error
	void testDouble() {
		NestingLevel n;
		n.saw_nesting_keyword(CKeyword::IF);
		n.saw_nesting_keyword(CKeyword::IF);
		CPPUNIT_ASSERT(n.get_nesting_level() == 2);
		n.saw_statement_semicolon();
		CPPUNIT_ASSERT(n.get_nesting_level() == 0);
	}

	// { 0; }
	void testTrivialBrace() {
		NestingLevel n;
		n.saw_open_brace();
		CPPUNIT_ASSERT(n.get_nesting_level() == 0);
		n.saw_statement_semicolon();
		CPPUNIT_ASSERT(n.get_nesting_level() == 0);
		n.saw_close_brace();
		CPPUNIT_ASSERT(n.get_nesting_level() == 0);
	}

	// {{ 0; }{
	void testDoubleTrivialBrace() {
		NestingLevel n;
		n.saw_open_brace();
		n.saw_open_brace();
		CPPUNIT_ASSERT(n.get_nesting_level() == 0);
		n.saw_statement_semicolon();
		CPPUNIT_ASSERT(n.get_nesting_level() == 0);
		n.saw_close_brace();
		n.saw_close_brace();
		CPPUNIT_ASSERT(n.get_nesting_level() == 0);
	}

	// if (x) { 1; 1; }
	void testSimpleBrace() {
		NestingLevel n;
		n.saw_nesting_keyword(CKeyword::IF);
		n.saw_open_brace();
		CPPUNIT_ASSERT(n.get_nesting_level() == 1);
		n.saw_statement_semicolon();
		CPPUNIT_ASSERT(n.get_nesting_level() == 1);
		n.saw_statement_semicolon();
		CPPUNIT_ASSERT(n.get_nesting_level() == 1);
		n.saw_close_brace();
		CPPUNIT_ASSERT(n.get_nesting_level() == 0);
	}

	// if (x) if (y) { 2; 2; }
	void testNestingBrace() {
		NestingLevel n;
		n.saw_nesting_keyword(CKeyword::IF);
		n.saw_nesting_keyword(CKeyword::IF);
		n.saw_open_brace();
		CPPUNIT_ASSERT(n.get_nesting_level() == 2);
		n.saw_statement_semicolon();
		CPPUNIT_ASSERT(n.get_nesting_level() == 2);
		n.saw_statement_semicolon();
		CPPUNIT_ASSERT(n.get_nesting_level() == 2);
		n.saw_close_brace();
		CPPUNIT_ASSERT(n.get_nesting_level() == 0);
	}

	// if (x) { 1; if (y) { 2; 2; } 1; 1; } 0;
	void testDoubleNestingBrace() {
		NestingLevel n;
		n.saw_nesting_keyword(CKeyword::IF);
		n.saw_open_brace();
		CPPUNIT_ASSERT(n.get_nesting_level() == 1);
		n.saw_statement_semicolon();
		CPPUNIT_ASSERT(n.get_nesting_level() == 1);
		n.saw_nesting_keyword(CKeyword::IF);
		n.saw_open_brace();
		CPPUNIT_ASSERT(n.get_nesting_level() == 2);
		n.saw_statement_semicolon();
		CPPUNIT_ASSERT(n.get_nesting_level() == 2);
		n.saw_statement_semicolon();
		CPPUNIT_ASSERT(n.get_nesting_level() == 2);
		n.saw_close_brace();
		CPPUNIT_ASSERT(n.get_nesting_level() == 1);
		n.saw_statement_semicolon();
		CPPUNIT_ASSERT(n.get_nesting_level() == 1);
		n.saw_statement_semicolon();
		CPPUNIT_ASSERT(n.get_nesting_level() == 1);
		n.saw_close_brace();
		CPPUNIT_ASSERT(n.get_nesting_level() == 0);
		n.saw_statement_semicolon();
		CPPUNIT_ASSERT(n.get_nesting_level() == 0);
	}

	// if (x) { if (y) 2; 1; 1; } 0;
	void testBraceProtection() {
		NestingLevel n;
		n.saw_nesting_keyword(CKeyword::IF);
		n.saw_open_brace();
		n.saw_nesting_keyword(CKeyword::IF);
		CPPUNIT_ASSERT(n.get_nesting_level() == 2);
		n.saw_statement_semicolon();
		CPPUNIT_ASSERT(n.get_nesting_level() == 1);
		n.saw_statement_semicolon();
		CPPUNIT_ASSERT(n.get_nesting_level() == 1);
		n.saw_statement_semicolon();
		n.saw_close_brace();
		CPPUNIT_ASSERT(n.get_nesting_level() == 0);
	}

	// if (x) { if (y) if (z) 3; 1; } 0;
	void testDoubleBraceProtection() {
		NestingLevel n;
		n.saw_nesting_keyword(CKeyword::IF);
		n.saw_open_brace();
		n.saw_nesting_keyword(CKeyword::IF);
		n.saw_nesting_keyword(CKeyword::IF);
		CPPUNIT_ASSERT(n.get_nesting_level() == 3);
		n.saw_statement_semicolon();
		CPPUNIT_ASSERT(n.get_nesting_level() == 1);
		n.saw_statement_semicolon();
		n.saw_close_brace();
		CPPUNIT_ASSERT(n.get_nesting_level() == 0);
	}

	// if (x) { if (y) 2; else 2; 1; } 0;
	void testIfElseBrace() {
		NestingLevel n;
		n.saw_nesting_keyword(CKeyword::IF);
		n.saw_open_brace();
		n.saw_nesting_keyword(CKeyword::IF);
		CPPUNIT_ASSERT(n.get_nesting_level() == 2);
		n.saw_statement_semicolon();
		CPPUNIT_ASSERT(n.get_nesting_level() == 1);
		n.saw_nesting_keyword(CKeyword::ELSE);
		CPPUNIT_ASSERT(n.get_nesting_level() == 2);
		n.saw_statement_semicolon();
		CPPUNIT_ASSERT(n.get_nesting_level() == 1);
		n.saw_close_brace();
		CPPUNIT_ASSERT(n.get_nesting_level() == 0);
	}

	// if (x) 1; else 1; 0;
	void testIfElse() {
		NestingLevel n;
		n.saw_nesting_keyword(CKeyword::IF);
		CPPUNIT_ASSERT(n.get_nesting_level() == 1);
		n.saw_statement_semicolon();
		n.saw_nesting_keyword(CKeyword::ELSE);
		CPPUNIT_ASSERT(n.get_nesting_level() == 1);
		n.saw_statement_semicolon();
		CPPUNIT_ASSERT(n.get_nesting_level() == 0);
	}

	// if (x) { 1; } else { 1; } 0;
	void testBracedIfElse() {
		NestingLevel n;
		n.saw_nesting_keyword(CKeyword::IF);
		n.saw_open_brace();
		CPPUNIT_ASSERT(n.get_nesting_level() == 1);
		n.saw_statement_semicolon();
		n.saw_close_brace();
		n.saw_nesting_keyword(CKeyword::ELSE);
		n.saw_open_brace();
		CPPUNIT_ASSERT(n.get_nesting_level() == 1);
		n.saw_statement_semicolon();
		n.saw_close_brace();
		CPPUNIT_ASSERT(n.get_nesting_level() == 0);
	}

	/*
	 * if (x)
	 *   {
	 *     2;
	 *   }
	 * else
	 *   {
	 *     2;
	 *   }
	 *   0;
	 */
	void testGnuBracedIfElse() {
		NestingLevel n;
		n.saw_nesting_keyword(CKeyword::IF);
		n.saw_open_brace(true);
		CPPUNIT_ASSERT(n.get_nesting_level() == 2);
		n.saw_statement_semicolon();
		n.saw_close_brace();
		n.saw_nesting_keyword(CKeyword::ELSE);
		n.saw_open_brace(true);
		CPPUNIT_ASSERT(n.get_nesting_level() == 2);
		n.saw_statement_semicolon();
		n.saw_close_brace();
		CPPUNIT_ASSERT(n.get_nesting_level() == 0);
	}

	// if (x) if (y) 2; else 2; 1; 0;
	// Modeled implementation error
	void testIfIfElse() {
		NestingLevel n;
		n.saw_nesting_keyword(CKeyword::IF);
		n.saw_nesting_keyword(CKeyword::IF);
		CPPUNIT_ASSERT(n.get_nesting_level() == 2);
		n.saw_statement_semicolon();
		n.saw_nesting_keyword(CKeyword::ELSE);
		CPPUNIT_ASSERT(n.get_nesting_level() == 2);
		n.saw_statement_semicolon();
		n.saw_nesting_keyword(CKeyword::ELSE);
		CPPUNIT_ASSERT(n.get_nesting_level() == 1);
		n.saw_statement_semicolon();
		CPPUNIT_ASSERT(n.get_nesting_level() == 0);
	}

	// if (x) 1; else if (y) 1; else 1; 0;
	// Uncovered specification error: strictly it is
	// if (x) 1; else if (y) 2; else 1; 0;
	// Uncovered two implementation errors
	void testElseIf() {
		NestingLevel n;
		n.saw_nesting_keyword(CKeyword::IF);
		CPPUNIT_ASSERT(n.get_nesting_level() == 1);
		n.saw_statement_semicolon();
		n.saw_nesting_keyword(CKeyword::ELSE);
		n.saw_nesting_keyword(CKeyword::IF);
		CPPUNIT_ASSERT(n.get_nesting_level() == 1);
		n.saw_statement_semicolon();
		n.saw_nesting_keyword(CKeyword::ELSE);
		CPPUNIT_ASSERT(n.get_nesting_level() == 1);
		n.saw_statement_semicolon();
		CPPUNIT_ASSERT(n.get_nesting_level() == 0);
	}

	// if (x) 1; else { if (y) 2; else 2; } 0;
	void testBracedElseIf() {
		NestingLevel n;
		n.saw_nesting_keyword(CKeyword::IF);
		CPPUNIT_ASSERT(n.get_nesting_level() == 1);
		n.saw_statement_semicolon();
		n.saw_nesting_keyword(CKeyword::ELSE);
		n.saw_open_brace();
		n.saw_nesting_keyword(CKeyword::IF);
		CPPUNIT_ASSERT(n.get_nesting_level() == 2);
		n.saw_statement_semicolon();
		n.saw_nesting_keyword(CKeyword::ELSE);
		CPPUNIT_ASSERT(n.get_nesting_level() == 2);
		n.saw_statement_semicolon();
		n.saw_close_brace();
		CPPUNIT_ASSERT(n.get_nesting_level() == 0);
	}

	// if (x) 1; else { if (y) 2; else if (z) 2; else 2; } 0;
	void testBracedElseIfElse() {
		NestingLevel n;
		n.saw_nesting_keyword(CKeyword::IF);
		CPPUNIT_ASSERT(n.get_nesting_level() == 1);
		n.saw_statement_semicolon();
		n.saw_nesting_keyword(CKeyword::ELSE);
		n.saw_open_brace();
		n.saw_nesting_keyword(CKeyword::IF);
		CPPUNIT_ASSERT(n.get_nesting_level() == 2);
		n.saw_statement_semicolon();
		n.saw_nesting_keyword(CKeyword::ELSE);
		n.saw_nesting_keyword(CKeyword::IF);
		CPPUNIT_ASSERT(n.get_nesting_level() == 2);
		n.saw_statement_semicolon();
		n.saw_nesting_keyword(CKeyword::ELSE);
		CPPUNIT_ASSERT(n.get_nesting_level() == 2);
		n.saw_statement_semicolon();
		n.saw_close_brace();
		CPPUNIT_ASSERT(n.get_nesting_level() == 0);
	}


	// if (x) 1; else { 1; if (y) 2; else 2; 1; } 0;
	void testComplexBracedElseIf() {
		NestingLevel n;
		n.saw_nesting_keyword(CKeyword::IF);
		CPPUNIT_ASSERT(n.get_nesting_level() == 1);
		n.saw_statement_semicolon();
		n.saw_nesting_keyword(CKeyword::ELSE);
		n.saw_open_brace();
		CPPUNIT_ASSERT(n.get_nesting_level() == 1);
		n.saw_statement_semicolon();
		n.saw_nesting_keyword(CKeyword::IF);
		CPPUNIT_ASSERT(n.get_nesting_level() == 2);
		n.saw_statement_semicolon();
		n.saw_nesting_keyword(CKeyword::ELSE);
		CPPUNIT_ASSERT(n.get_nesting_level() == 2);
		n.saw_statement_semicolon();
		CPPUNIT_ASSERT(n.get_nesting_level() == 1);
		n.saw_statement_semicolon();
		CPPUNIT_ASSERT(n.get_nesting_level() == 1);
		n.saw_close_brace();
		CPPUNIT_ASSERT(n.get_nesting_level() == 0);
	}

	// do 1; while(z); 0;
	void testSimpleDo() {
		NestingLevel n;
		n.saw_nesting_keyword(CKeyword::DO);
		CPPUNIT_ASSERT(n.get_nesting_level() == 1);
		n.saw_statement_semicolon();
		n.saw_nesting_keyword(CKeyword::WHILE);
		n.saw_statement_semicolon();
		CPPUNIT_ASSERT(n.get_nesting_level() == 0);
	}

	// do { 1; 1; } while(z); 0;
	void testBracedDo() {
		NestingLevel n;
		n.saw_nesting_keyword(CKeyword::DO);
		n.saw_open_brace();
		CPPUNIT_ASSERT(n.get_nesting_level() == 1);
		n.saw_statement_semicolon();
		CPPUNIT_ASSERT(n.get_nesting_level() == 1);
		n.saw_statement_semicolon();
		n.saw_close_brace();
		n.saw_nesting_keyword(CKeyword::WHILE);
		n.saw_statement_semicolon();
		CPPUNIT_ASSERT(n.get_nesting_level() == 0);
	}

	// do { 1; while (x) 2; 1; } while(z); 0;
	void testBracedDoWhile() {
		NestingLevel n;
		n.saw_nesting_keyword(CKeyword::DO);
		n.saw_open_brace();
		CPPUNIT_ASSERT(n.get_nesting_level() == 1);
		n.saw_statement_semicolon();
		CPPUNIT_ASSERT(n.get_nesting_level() == 1);
		n.saw_nesting_keyword(CKeyword::WHILE);
		CPPUNIT_ASSERT(n.get_nesting_level() == 2);
		n.saw_statement_semicolon();
		CPPUNIT_ASSERT(n.get_nesting_level() == 1);
		n.saw_close_brace();
		n.saw_nesting_keyword(CKeyword::WHILE);
		CPPUNIT_ASSERT(n.get_nesting_level() == 0);
		n.saw_statement_semicolon();
		CPPUNIT_ASSERT(n.get_nesting_level() == 0);
	}

	// do while (x) 2; while(z); 0;
	void testUnbracedDoWhile() {
		NestingLevel n;
		n.saw_nesting_keyword(CKeyword::DO);
		n.saw_nesting_keyword(CKeyword::WHILE);
		CPPUNIT_ASSERT(n.get_nesting_level() == 2);
		n.saw_statement_semicolon();
		CPPUNIT_ASSERT(n.get_nesting_level() == 1);
		n.saw_nesting_keyword(CKeyword::WHILE);
		CPPUNIT_ASSERT(n.get_nesting_level() == 0);
		n.saw_statement_semicolon();
		CPPUNIT_ASSERT(n.get_nesting_level() == 0);
	}

	// do while (x) while(y) 3; while(z); 0;
	void testUnbracedDoNestedWhile() {
		NestingLevel n;
		n.saw_nesting_keyword(CKeyword::DO);
		n.saw_nesting_keyword(CKeyword::WHILE);
		n.saw_nesting_keyword(CKeyword::WHILE);
		CPPUNIT_ASSERT(n.get_nesting_level() == 3);
		n.saw_statement_semicolon();
		CPPUNIT_ASSERT(n.get_nesting_level() == 1);
		n.saw_nesting_keyword(CKeyword::WHILE);
		CPPUNIT_ASSERT(n.get_nesting_level() == 0);
		n.saw_statement_semicolon();
		CPPUNIT_ASSERT(n.get_nesting_level() == 0);
	}

	// do do while (x) while(y) 4; while(z); while (w) 0;
	void testUnbracedNestedDoNestedWhile() {
		NestingLevel n;
		n.saw_nesting_keyword(CKeyword::DO);
		CPPUNIT_ASSERT(n.get_nesting_level() == 1);
		n.saw_nesting_keyword(CKeyword::DO);
		CPPUNIT_ASSERT(n.get_nesting_level() == 2);
		n.saw_nesting_keyword(CKeyword::WHILE);
		CPPUNIT_ASSERT(n.get_nesting_level() == 3);
		n.saw_nesting_keyword(CKeyword::WHILE);
		CPPUNIT_ASSERT(n.get_nesting_level() == 4);
		n.saw_statement_semicolon();
		CPPUNIT_ASSERT(n.get_nesting_level() == 2);
		n.saw_nesting_keyword(CKeyword::WHILE);
		CPPUNIT_ASSERT(n.get_nesting_level() == 1);
		n.saw_statement_semicolon();
		CPPUNIT_ASSERT(n.get_nesting_level() == 1);
		n.saw_nesting_keyword(CKeyword::WHILE);
		CPPUNIT_ASSERT(n.get_nesting_level() == 0);
		n.saw_statement_semicolon();
		CPPUNIT_ASSERT(n.get_nesting_level() == 0);
	}


	// while (x) 1;
	void testSingleWhile() {
		NestingLevel n;
		n.saw_nesting_keyword(CKeyword::WHILE);
		CPPUNIT_ASSERT(n.get_nesting_level() == 1);
		n.saw_statement_semicolon();
		CPPUNIT_ASSERT(n.get_nesting_level() == 0);
	}

	// if (x) for (;;) switch while (y) do 5; while(z); 0;
	void testAllStatements() {
		NestingLevel n;
		n.saw_nesting_keyword(CKeyword::IF);
		CPPUNIT_ASSERT(n.get_nesting_level() == 1);
		n.saw_nesting_keyword(CKeyword::FOR);
		CPPUNIT_ASSERT(n.get_nesting_level() == 2);
		n.saw_nesting_keyword(CKeyword::SWITCH);
		CPPUNIT_ASSERT(n.get_nesting_level() == 3);
		n.saw_nesting_keyword(CKeyword::WHILE);
		CPPUNIT_ASSERT(n.get_nesting_level() == 4);
		n.saw_nesting_keyword(CKeyword::DO);
		CPPUNIT_ASSERT(n.get_nesting_level() == 5);
		n.saw_statement_semicolon();
		CPPUNIT_ASSERT(n.get_nesting_level() == 5);
		n.saw_nesting_keyword(CKeyword::WHILE);
		n.saw_statement_semicolon();
		CPPUNIT_ASSERT(n.get_nesting_level() == 0);
	}

	// a = { 0 }; 0;
	void testBracedAssignment() {
		NestingLevel n;
		n.saw_open_brace();
		CPPUNIT_ASSERT(n.get_nesting_level() == 0);
		n.saw_close_brace();
		n.saw_statement_semicolon();
		CPPUNIT_ASSERT(n.get_nesting_level() == 0);
		n.saw_statement_semicolon();
		CPPUNIT_ASSERT(n.get_nesting_level() == 0);
	}

	// if (x) for (;;) 2; else 1;
	// Added to investigate and verify fixing testElseComment
	void testIfForElse() {
		NestingLevel n;
		CPPUNIT_ASSERT(n.get_nesting_level() == 0);
		n.saw_nesting_keyword(CKeyword::IF);
		CPPUNIT_ASSERT(n.get_nesting_level() == 1);
		n.saw_nesting_keyword(CKeyword::FOR);
		CPPUNIT_ASSERT(n.get_nesting_level() == 2);
		n.saw_statement_semicolon();
		CPPUNIT_ASSERT(n.get_nesting_level() == 0);
		n.saw_nesting_keyword(CKeyword::ELSE);
		CPPUNIT_ASSERT(n.get_nesting_level() == 1);
		n.saw_statement_semicolon();
		CPPUNIT_ASSERT(n.get_nesting_level() == 0);
	}

	// 0; else 1;
	// Added to investigate and verify fixing testNoIf
	void testElse() {
		NestingLevel n;
		CPPUNIT_ASSERT(n.get_nesting_level() == 0);
		n.saw_statement_semicolon();
		CPPUNIT_ASSERT(n.get_nesting_level() == 0);
		n.saw_nesting_keyword(CKeyword::ELSE);
		CPPUNIT_ASSERT(n.get_nesting_level() == 1);
		n.saw_statement_semicolon();
		CPPUNIT_ASSERT(n.get_nesting_level() == 0);
	}

	// 0; else 1; else 1;
	void testElseElse() {
		NestingLevel n;
		CPPUNIT_ASSERT(n.get_nesting_level() == 0);
		n.saw_statement_semicolon();
		CPPUNIT_ASSERT(n.get_nesting_level() == 0);
		n.saw_nesting_keyword(CKeyword::ELSE);
		CPPUNIT_ASSERT(n.get_nesting_level() == 1);
		n.saw_statement_semicolon();
		CPPUNIT_ASSERT(n.get_nesting_level() == 0);
		n.saw_nesting_keyword(CKeyword::ELSE);
		CPPUNIT_ASSERT(n.get_nesting_level() == 1);
		n.saw_statement_semicolon();
		CPPUNIT_ASSERT(n.get_nesting_level() == 0);
	}
};
#endif /*  NESTINGLEVELTEST_H */
