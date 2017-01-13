#ifndef BOLSTATETEST_H
#define BOLSTATETEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "BolState.h"

class BolStateTest : public CppUnit::TestFixture  {
	CPPUNIT_TEST_SUITE(BolStateTest);
	CPPUNIT_TEST(testCtor);
	CPPUNIT_TEST(testAfterSpace);
	CPPUNIT_TEST(testAfterNonSpace);
	CPPUNIT_TEST(testAfterNewline);
	CPPUNIT_TEST(testIndentation);
	CPPUNIT_TEST_SUITE_END();
public:
	void testCtor() {
		BolState b;
		CPPUNIT_ASSERT(b.at_bol());
		CPPUNIT_ASSERT(b.at_bol_space());
	}

	void testAfterSpace() {
		BolState b;
		b.saw_space(' ');
		CPPUNIT_ASSERT(!b.at_bol());
		CPPUNIT_ASSERT(b.at_bol_space());
	}

	void testAfterNonSpace() {
		BolState b;
		b.saw_space(' ');
		b.saw_non_space();
		b.saw_space(' ');
		CPPUNIT_ASSERT(!b.at_bol());
		CPPUNIT_ASSERT(!b.at_bol_space());
	}

	void testAfterNewline() {
		BolState b;
		b.saw_non_space();
		b.saw_newline();
		CPPUNIT_ASSERT(b.at_bol());
		CPPUNIT_ASSERT(b.at_bol_space());
	}

	void testIndentation() {
		BolState b;
		// Test space
		CPPUNIT_ASSERT_EQUAL(b.get_indentation(), 0);
		b.saw_space(' ');
		CPPUNIT_ASSERT_EQUAL(b.get_indentation(), 1);
		b.saw_space(' ');
		CPPUNIT_ASSERT_EQUAL(b.get_indentation(), 2);
		// Test tab
		b.saw_space('\t');
		CPPUNIT_ASSERT_EQUAL(b.get_indentation(), 8);
		// Space after tab
		b.saw_space(' ');
		CPPUNIT_ASSERT_EQUAL(b.get_indentation(), 9);
		// Edge case
		while (b.get_indentation() != 15)
			b.saw_space(' ');
		b.saw_space('\t');
		CPPUNIT_ASSERT_EQUAL(b.get_indentation(), 16);
		// Edge case
		b.saw_space('\t');
		CPPUNIT_ASSERT_EQUAL(b.get_indentation(), 24);
		// Non-space
		b.saw_non_space();
		CPPUNIT_ASSERT_EQUAL(b.get_indentation(), 24);
	}
};
#endif /*  BOLSTATETEST_H */
