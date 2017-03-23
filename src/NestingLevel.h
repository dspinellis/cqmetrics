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

#include <list>
#include <vector>
#include <iostream>

#include "CKeyword.h"

/** Track the details of a single nesting */
class NestingDetails {
public:
	int brace_balance() const {	// Number of matching {} pairs
		return brace_indentation.size() - 1;
	}

	CKeyword::IdentifierType key;	// Keyword that introduced the nesting
	bool saw_statement;		// True after seeing a statement

	int indent() const {		// How much to indent at this level
		int level = 0;
		for (std::vector <bool>::const_iterator i =
				brace_indentation.begin();
				i != brace_indentation.end(); i++)
			level += *i;
		return level;
	}

	/*
	 * One element for each brace.
	 * True if that level was indented, false if not.
	 */
	std::vector <bool> brace_indentation;


	NestingDetails() : key(CKeyword::OTHER), saw_statement(false) {
			brace_indentation.push_back(true);
			// indent() = 1; brace_balance() == 0
	}
	NestingDetails(CKeyword::IdentifierType k) : key(k),
			saw_statement(false) {
			brace_indentation.push_back(true);
			// indent() = 1; brace_balance() == 0
	}
};

/** Track nesting level */
class NestingLevel {
private:
	typedef std::list<NestingDetails> NDList;	// Details for each nesting level
	NDList nd;		// Details for each nesting level
	NDList backtrack;	// Details for each nesting level
	/**
	 * Pop nesting within a function that is not protected by braces.
	 * Ensure that at least one level remains, in case the parsing algorithm
	 * gets thrown off by the use of macros.
	 */
	void pop() {
		bool saved_backtrack = false;

		while (!nd.empty() && nd.back().brace_balance() == 0 &&
				nd.back().key != CKeyword::DO) {
			// Save if stack for possible else
			if ((nd.back().key == CKeyword::IF ||
					nd.back().key == CKeyword::ELIF) &&
					!saved_backtrack) {
				backtrack = nd;
				saved_backtrack = true;
			}
			nd.pop_back();
		}
		if (nd.empty())
			reset();	// We lost track of the state
	}

	/**
	 * To be called after encountering a statement's semicolon
	 * or a closing brace.
	 */
	void saw_statement() {
		if (nd.back().brace_balance() == 0) {
			pop();
			nd.back().saw_statement = true;
		}
	}

public:
	NestingLevel() {
		reset();
	}
	/** Reset state after a function's opening brace. */
	void reset() {
		nd = NDList();
		nd.push_back(NestingDetails());
	}

	/**
	 * To be called after encountering an opening brace.
	 * indented is set to true if there is an additional level
	 * of indentation associated with a standalone brace.
	 */
	void saw_open_brace(bool indented=false) {
		nd.back().brace_indentation.push_back(indented);
	}

	/** To be called after encountering a closing brace */
	void saw_close_brace() {
		if (nd.back().brace_balance() > 0)
			nd.back().brace_indentation.pop_back();
		saw_statement();
	}

	/** To be called after encountering a statement's semicolon */
	void saw_statement_semicolon() {
		saw_statement();
	}

	/** To be called after encountering a keyword associated with nesting */
	void saw_nesting_keyword(CKeyword::IdentifierType t) {
		if (t == CKeyword::WHILE && nd.back().key == CKeyword::DO &&
				nd.back().saw_statement)
			// Handle while of do while
			nd.pop_back();
		else if (t == CKeyword::IF && nd.back().key == CKeyword::ELSE &&
				nd.back().brace_balance() == 0)
			// else if -> elif
			nd.back().key = CKeyword::ELIF;
		else {
			/*
			 * On an "else" keyword backtrack: undo preceding
			 * full pop and add a single level one.
			 */
			if (t == CKeyword::ELSE && !backtrack.empty()) {
				nd = backtrack;
				if (!nd.empty())
					nd.pop_back();
				backtrack = nd;
			}
			nd.push_back(NestingDetails(t));
		}
	}

	/** Return the current level of nesting. */
	int get_nesting_level() {
		int nesting = 0;
		// One day: for (auto& level : nd)
		for (NDList::const_iterator level = nd.begin();
				level != nd.end(); level++)
			nesting += level->indent();
		return nesting - 1;
	}
};

#endif /* NESTINGLEVEL_H */
