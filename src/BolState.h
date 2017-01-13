#ifndef BOLSTATE_H
#define BOLSTATE_H

#include "CKeyword.h"

class BolState {
private:
	bool bol_state;
	bool bol_space_state;
	int indentation;
public:
	BolState() { saw_newline(); }

	/** True at the beginning of a line */
	bool at_bol() const { return bol_state; }

	/** True at the beginning of a line, possibly with spaces */
	bool at_bol_space() const { return bol_space_state; }

	/** Return the current line's indentation. */
	int get_indentation() const { return indentation; }

	/** Called when processing a newline */
	void saw_newline() {
		bol_state = true;
		bol_space_state = true;
		indentation = 0;
	}

	/** Called when processing a space character c0 */
	void saw_space(int c) {
		bol_state = false;
		if (bol_space_state) {
			if (c == ' ')
				indentation++;
			/*
			 * 0 8
			 * 1 8
			 * ...
			 * 7 8
			 * 8 16
			 * ...
			 */
			else if (c == '\t')
				indentation = (indentation / 8 + 1) * 8;
		}
	}

	/** Called when processing a non-space character */
	void saw_non_space() { bol_state = bol_space_state = false; }
};
#endif /* BOLSTATE_H */
