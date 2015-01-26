/*-
 * Copyright 2015 Diomidis Spinellis
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

#ifndef NESTINGLEVEL_H
#define NESTINGLEVEL_H

#include <stack>

#include "CKeyword.h"

/** Track the details of a single nesting */
class NestingDetails {
public:
	int brace_balance;	// Matching {} pairs
	bool is_do;		// True if nesting of do while statement
	NestingDetails(bool id) : brace_balance(0), is_do(id) {}
};

/** Track nesting level */
class NestingLevel {
private:
	typedef std::stack<NestingDetails> NDStack;	// Details for each nesting level
	NDStack nd;	// Details for each nesting level
	/**
	 * Pop one level of nesting within a function.
	 * Ensure that at least one level remains, in case the parsing algorithm
	 * gets thrown off by the use of macros.
	 */
	void pop() {
		nd.pop();
		if (nd.empty())
			reset();	// We lost track of the state
	}
public:
	NestingLevel() {
		reset();
	}
	/** Reset state after a function's opening brace. */
	void reset() {
		nd = NDStack();
		nd.push(NestingDetails(false));
		nd.top().brace_balance++;
	}

	/** To be called after encountering an opening brace */
	void saw_open_brace() {
		nd.top().brace_balance++;
	}

	/** To be called after encountering a closing brace */
	void saw_close_brace() {
		nd.top().brace_balance--;
		if (nd.top().brace_balance == 0 && !nd.top().is_do)
			pop();
	}

	/** To be called after encountering a statement's semicolon */
	void saw_statement_semicolon() {
		if (nd.top().brace_balance == 0 && !nd.top().is_do)
			pop();
	}

	/** To be called after encountering a keyword associated with nesting */
	void saw_nesting_keyword(CKeyword::IdentifierType t) {
		if (nd.top().brace_balance == 0 && nd.top().is_do &&
				t == CKeyword::WHILE)
			pop();
		else
			nd.push(NestingDetails(t == CKeyword::DO));
	}

	/** Return the current level of nesting. */
	int get_nesting_level() {
		return nd.size() - 1;
	}
};

#endif /* NESTINGLEVEL_H */
