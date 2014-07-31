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
	CPPUNIT_TEST(testNchar);
	CPPUNIT_TEST_SUITE_END();
public:
	void testCtor() {
		CharSource s;
	}

	void testStrCtor() {
		std::stringstream str("hi");

		CharSource s(str);
		char c;
		CPPUNIT_ASSERT(s.get(c) && c == 'h');
		CPPUNIT_ASSERT(s.get(c) && c == 'i');
	}

	void testPush() {
		std::stringstream str("he");

		CharSource s(str);
		char c;
		CPPUNIT_ASSERT(s.get(c) && c == 'h');
		// Push one
		s.push('o');
		CPPUNIT_ASSERT(s.get(c) && c == 'o');
		// Again
		s.push('p');
		CPPUNIT_ASSERT(s.get(c) && c == 'p');
		// Two
		s.push('q');
		s.push('r');
		CPPUNIT_ASSERT(s.get(c) && c == 'r');
		CPPUNIT_ASSERT(s.get(c) && c == 'q');
		CPPUNIT_ASSERT(s.get(c) && c == 'e');
		// Push at EOF
		s.push('s');
		CPPUNIT_ASSERT(s.get(c) && c == 's');
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

	void testNchar() {
		std::stringstream str("he");

		CharSource s(str);
		char c;
		CPPUNIT_ASSERT(s.get(c) && c == 'h');
		CPPUNIT_ASSERT(s.get(c) && c == 'e');
		CPPUNIT_ASSERT(!s.get(c));
		CPPUNIT_ASSERT(s.get_nchar() == 2);
	}

};
#endif /*  CHARSOURCETEST_H */
