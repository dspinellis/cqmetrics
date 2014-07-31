#ifndef CHARSOURCETEST_H
#define CHARSOURCETEST_H

#include <sstream>

#include <cppunit/extensions/HelperMacros.h>

#include "CharSource.h"

class CharSourceTest : public CppUnit::TestFixture  {
	CPPUNIT_TEST_SUITE(CharSourceTest);
	CPPUNIT_TEST(testCtor);
	CPPUNIT_TEST(testStrCtor);
	CPPUNIT_TEST(testPush);
	CPPUNIT_TEST(testEof);
	CPPUNIT_TEST(testEofPush);
	CPPUNIT_TEST(testNchar);
	CPPUNIT_TEST_SUITE_END();
public:
	void testCtor() {
		CharSource s;
	}

	void testStrCtor() {
		std::stringstream str("hi");

		CharSource s(str);
		CPPUNIT_ASSERT(s.get() == 'h');
		CPPUNIT_ASSERT(s.get() == 'i');
	}

	void testPush() {
		std::stringstream str("he");

		CharSource s(str);
		CPPUNIT_ASSERT(s.get() == 'h');
		// Push one
		s.push('o');
		CPPUNIT_ASSERT(s.get() == 'o');
		// Again
		s.push('p');
		CPPUNIT_ASSERT(s.get() == 'p');
		// Two
		s.push('q');
		s.push('r');
		CPPUNIT_ASSERT(s.get() == 'r');
		CPPUNIT_ASSERT(s.get() == 'q');
		CPPUNIT_ASSERT(s.get() == 'e');
		// Push at EOF
		s.push('s');
		CPPUNIT_ASSERT(s.get() == 's');
	}

	struct StateHandler {
	private:
		int state;
	public:
		StateHandler(int s) : state(s) {}
		int get_state() { return state; }
		void operator()() {
			state = 42;
		}
	};

	void testEof() {
		std::stringstream str("he");

		CharSource s(str);
		CPPUNIT_ASSERT(!s.eof());
		CPPUNIT_ASSERT(s.get() == 'h');
		CPPUNIT_ASSERT(!s.eof());
		CPPUNIT_ASSERT(s.get() == 'e');
		CPPUNIT_ASSERT(!s.eof());
		// EOF
		(void)s.get();
		CPPUNIT_ASSERT(s.eof());
	}

	void testEofPush() {
		std::stringstream str("a");

		CharSource s(str);
		CPPUNIT_ASSERT(s.get() == 'a');
		CPPUNIT_ASSERT(!s.eof());
		s.push('s');
		CPPUNIT_ASSERT(s.get() == 's');
		CPPUNIT_ASSERT(!s.eof());
		// EOF
		(void)s.get();
		CPPUNIT_ASSERT(s.eof());
	}

	void testNchar() {
		std::stringstream str("he");

		CharSource s(str);
		CPPUNIT_ASSERT(s.get() == 'h');
		CPPUNIT_ASSERT(s.get() == 'e');
		// EOF
		(void)s.get();
		CPPUNIT_ASSERT(s.get_nchar() == 2);
	}

};
#endif /*  CHARSOURCETEST_H */
