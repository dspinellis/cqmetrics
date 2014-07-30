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
	CPPUNIT_TEST(testEofHandler);
	CPPUNIT_TEST_SUITE_END();
public:
	void testCtor() {
		CharSource<> s;
	}

	void testStrCtor() {
		std::stringstream str("hi");

		CharSource<> s(str);
		CPPUNIT_ASSERT(s.get() == 'h');
		CPPUNIT_ASSERT(s.get() == 'i');
	}

	void testPush() {
		std::stringstream str("he");

		CharSource<> s(str);
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

	void testEofHandler() {
		std::stringstream str("he");
		StateHandler *sh = new StateHandler(12);

		CharSource<StateHandler> s(str, sh);
		CPPUNIT_ASSERT(sh->get_state() == 12);
		CPPUNIT_ASSERT(s.get() == 'h');
		CPPUNIT_ASSERT(sh->get_state() == 12);
		CPPUNIT_ASSERT(s.get() == 'e');
		CPPUNIT_ASSERT(sh->get_state() == 12);
		// EOF
		(void)s.get();
		CPPUNIT_ASSERT(sh->get_state() == 42);
	}
};
#endif /*  CHARSOURCETEST_H */
