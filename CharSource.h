#ifndef CHARSOURCE_H
#define CHARSOURCE_H

#include <cstdlib>
#include <iostream>
#include <stack>

struct DefaultHandler {
	void operator()() {
		std::cerr << "EOF encountered" << std::endl;
		exit(1);
	}
};

/**
 * A source of characters coming from standard input with
 * infinite push back capability.
 */
template <class OnEof=DefaultHandler>
class CharSource {
private:
	std::stack<char> st;
	std::istream &in;
	OnEof *eof_handler;

public:
	CharSource(std::istream &s = std::cin,
			OnEof *eh = new DefaultHandler()) :
		in(s), eof_handler(eh) {}

	/** Obtain the next character from the source */
	char get() {
		char c;

		if (st.empty()) {
			in.get(c);
			if (in.eof())
				(*eof_handler)();
		} else {
			c = st.top();
			st.pop();
		}
		return (c);
	}

	/** Push the specified character back into the source */
	void push(char c) {
		st.push(c);
	}
};
#endif /* CHARSOURCE_H */
