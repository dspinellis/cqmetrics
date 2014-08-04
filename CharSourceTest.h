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
	CPPUNIT_TEST(testNcharPush);
	CPPUNIT_TEST(testCharAfter);
	CPPUNIT_TEST(testCharBefore);
	CPPUNIT_TEST(testCharBeforeN);
	CPPUNIT_TEST(testCharBeforeNewline);
	CPPUNIT_TEST(testCharBeforePush);
	CPPUNIT_TEST(testCharBeforeQueueShrink);
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

	void testNcharPush() {
		std::stringstream str("he");

		CharSource s(str);
		char c;
		CPPUNIT_ASSERT(s.get(c) && c == 'h');
		CPPUNIT_ASSERT(s.get_nchar() == 1);
		CPPUNIT_ASSERT(s.get(c) && c == 'e');
		s.push('e');
		CPPUNIT_ASSERT(s.get_nchar() == 1);
		CPPUNIT_ASSERT(s.get(c) && c == 'e');
		CPPUNIT_ASSERT(!s.get(c));
		CPPUNIT_ASSERT(s.get_nchar() == 2);
	}

	void testCharAfter() {
		std::stringstream str("he");

		CharSource s(str);
		char c;
		CPPUNIT_ASSERT(s.char_after() == 'h');
		CPPUNIT_ASSERT(s.get(c) && c == 'h');
		CPPUNIT_ASSERT(s.char_after() == 'e');
		s.push('p');
		CPPUNIT_ASSERT(s.char_after() == 'p');
		CPPUNIT_ASSERT(s.get(c) && c == 'p');
		CPPUNIT_ASSERT(s.char_after() == 'e');
		CPPUNIT_ASSERT(s.get(c) && c == 'e');
		CPPUNIT_ASSERT(!s.get(c));
		CPPUNIT_ASSERT(s.char_after() == 0);
		CPPUNIT_ASSERT(s.get_nchar() == 2);
	}

	void testCharBefore() {
		std::stringstream str("he");

		CharSource s(str);
		char c;
		CPPUNIT_ASSERT(s.char_before() == 0);
		CPPUNIT_ASSERT(s.char_after() == 'h');

		CPPUNIT_ASSERT(s.get(c) && c == 'h');
		CPPUNIT_ASSERT(s.char_before() == 0);
		CPPUNIT_ASSERT(s.char_after() == 'e');

		CPPUNIT_ASSERT(s.get(c) && c == 'e');
		CPPUNIT_ASSERT(s.char_before() == 'h');
		CPPUNIT_ASSERT(!s.get(c));
		CPPUNIT_ASSERT(s.char_after() == 0);
		CPPUNIT_ASSERT(s.get_nchar() == 2);
	}

	void testCharBeforeNewline() {
		std::stringstream str("h\ne");

		CharSource s(str);
		char c;
		CPPUNIT_ASSERT(s.char_before() == 0);
		CPPUNIT_ASSERT(s.char_after() == 'h');

		CPPUNIT_ASSERT(s.get(c) && c == 'h');
		CPPUNIT_ASSERT(s.char_before() == 0);

		CPPUNIT_ASSERT(s.get(c) && c == '\n');
		CPPUNIT_ASSERT(s.char_before() == 'h');
	}

	void testCharBeforePush() {
		std::stringstream str("he");

		CharSource s(str);
		char c;
		CPPUNIT_ASSERT(s.char_before() == 0);
		CPPUNIT_ASSERT(s.char_after() == 'h');

		CPPUNIT_ASSERT(s.get(c) && c == 'h');
		CPPUNIT_ASSERT(s.char_before() == 0);
		CPPUNIT_ASSERT(s.char_after() == 'e');
		s.push('h');
		CPPUNIT_ASSERT(s.char_after() == 'h');

		CPPUNIT_ASSERT(s.get(c) && c == 'h');
		CPPUNIT_ASSERT(s.char_before() == 0);
		CPPUNIT_ASSERT(s.char_after() == 'e');

		CPPUNIT_ASSERT(s.get(c) && c == 'e');
		CPPUNIT_ASSERT(s.char_before() == 'h');
		CPPUNIT_ASSERT(!s.get(c));
		CPPUNIT_ASSERT(s.char_after() == 0);
		CPPUNIT_ASSERT(s.get_nchar() == 2);
	}

	void testCharBeforeN() {
		std::stringstream str("hat");

		CharSource s(str);
		char c;
		CPPUNIT_ASSERT(s.char_before() == 0);
		CPPUNIT_ASSERT(s.char_after() == 'h');

		CPPUNIT_ASSERT(s.get(c) && c == 'h');
		CPPUNIT_ASSERT(s.char_before() == 0);

		CPPUNIT_ASSERT(s.get(c) && c == 'a');
		CPPUNIT_ASSERT(s.char_before() == 'h');
		CPPUNIT_ASSERT(s.char_before(2) == 0);

		CPPUNIT_ASSERT(s.get(c) && c == 't');
		CPPUNIT_ASSERT(s.char_before() == 'a');
		CPPUNIT_ASSERT(s.char_before(2) == 'h');
	}


	void testCharBeforeQueueShrink() {
		std::stringstream str("abcdefghijklmnopqrstuvwxyz");

		CharSource s(str);
		char c;

		CPPUNIT_ASSERT(s.get(c) && c == 'a');
		for (char i = 'b'; i < 'z'; i++) {
			CPPUNIT_ASSERT(s.char_after() == i);
			CPPUNIT_ASSERT(s.get(c) && c == i);
			CPPUNIT_ASSERT(s.char_before() == i - 1);
		}
	}
};
#endif /*  CHARSOURCETEST_H */
