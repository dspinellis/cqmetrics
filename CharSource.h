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

	/**
	 * Return the next character from source without removing it
	 * Return 0 on EOF
	 */
	char peek() {
		char c;
		if (get(c)) {
			push(c);
			return c;
		} else
			return 0;
	}

	/** Return number of characters read */
	int get_nchar() const { return nchar - st.size(); }

	/** Push the specified character back into the source */
	void push(char c) {
		st.push(c);
	}
};
#endif /* CHARSOURCE_H */
