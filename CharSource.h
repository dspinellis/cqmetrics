#ifndef CHARSOURCE_H
#define CHARSOURCE_H

#include <cstdlib>
#include <iostream>
#include <stack>

/**
 * A source of characters coming from standard input with
 * infinite push back capability.
 */
class CharSource {
private:
	std::stack<char> st;
	std::istream &in;

public:
	CharSource(std::istream &s = std::cin) : in(s) {}

	/** Obtain the next character from the source */
	char get() {
		char c;

		if (st.empty())
			in.get(c);
		else {
			c = st.top();
			st.pop();
		}
		return (c);
	}

	/** Return true if the input has reached its end */
	bool eof() const {
		return (st.empty() && in.eof());
	}

	/** Push the specified character back into the source */
	void push(char c) {
		st.push(c);
	}
};
#endif /* CHARSOURCE_H */
