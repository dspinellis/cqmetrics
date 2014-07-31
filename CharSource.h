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
	int nchar;		// Number of characters read

public:
	CharSource(std::istream &s = std::cin) : in(s), nchar(0) {}

	/** Obtain the next character from the source */
	bool get(char &c) {
		if (st.empty()) {
			if (in.get(c))
				nchar++;
			else
				return false;
		} else {
			c = st.top();
			st.pop();
		}
		return true;
	}

	/** Return number of characters read */
	int get_nchar() const { return nchar; }

	/** Push the specified character back into the source */
	void push(char c) {
		st.push(c);
	}
};
#endif /* CHARSOURCE_H */
