#ifndef BOLSTATE_H
#define BOLSTATE_H

#include "CKeyword.h"

class BolState {
private:
	bool bol_state;
	bool bol_space_state;
	CKeyword ckeyword;		// Can identify keywords from identifiers
public:
	BolState() { saw_newline(); }

	/** True at the beginning of a line */
	bool at_bol() const { return bol_state; }

	/** True at the beginning of a line, possibly with spaces */
	bool at_bol_space() const { return bol_space_state; }

	/** Called when processing a newline */
	void saw_newline() {
		bol_state = true;
		bol_space_state = true;
	}

	/** Called when processing a space character */
	void saw_space() { bol_state = false; }

	/** Called when processing a non-space character */
	void saw_non_space() { bol_state = bol_space_state = false; }
};
#endif /* BOLSTATE_H */
