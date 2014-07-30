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
	CPPUNIT_TEST_SUITE_END();
public:
	void testCtor() {
		BolState b;
		CPPUNIT_ASSERT(b.at_bol());
		CPPUNIT_ASSERT(b.at_bol_space());
	}

	void testAfterSpace() {
		BolState b;
		b.saw_space();
		CPPUNIT_ASSERT(!b.at_bol());
		CPPUNIT_ASSERT(b.at_bol_space());
	}

	void testAfterNonSpace() {
		BolState b;
		b.saw_space();
		b.saw_non_space();
		b.saw_space();
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
};
#endif /*  BOLSTATETEST_H */
