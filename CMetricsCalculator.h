/*-
 * Copyright 2014 Diomidis Spinellis
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

#ifndef CMETRICSCALCULATOR_H
#define CMETRICSCALCULATOR_H

#include <iostream>

#include "CharSource.h"
#include "BolState.h"
#include "QualityMetrics.h"
#include "NestingLevel.h"

/** Collect quality metrics from C-like source code */
class CMetricsCalculator {
private:
	CharSource src;
	QualityMetrics qm;
	BolState bol;
	int top_level_depth;		// 0 for C, 1 for Java
	int current_depth;		// Keeps track of { }
	bool scan_cpp_directive;	// Keyword after a C preprocessor #
	bool scan_cpp_line;		// Line after a C preprocessor #
	void calculate_metrics_loop();
	bool calculate_metrics_switch();
	bool in_function;		// True when scanning functions
	bool in_dox_comment;		// True if processing a DOxygen comment
	int chars_read_at_bol;		// Characters that were read
					// at the beginning of a line
	/** Bracket balance for control statememts. */
	int stmt_bracket_balance;
	int line_bracket_balance;	// Bracket balance for each line
	int line_nesting;		// Nesting of current line
	/** Indentation of preceding indented line. */
	int previous_indentation;
	bool continuation;		// True if a statement continuation line
	/** True for keywords that don't end with semicolon */
	bool saw_non_semicolon_keyword;
	bool saw_unindent;		// True if line is unindented
	bool saw_comment;		// True after a comment
	bool saw_cpp_directive;		// True after c preprocessor directive
	bool indentation_list;		// List indentation for each line
	NestingLevel nesting;		// Track nesting level
	CKeyword ckeyword;
	/** Verify the coding style of binary operators */
	void binary_style(char before);
	/** Verify the coding style of keywords requiring spaces */
	void keyword_style(char before, char allowed = 0);
	void keyword_style_left_space(char before);
	/** Called at every encountered newline */
	void newline(bool in_non_code_block = false);
public:
	CMetricsCalculator(std::istream &s = std::cin) : src(s),
	top_level_depth(0), current_depth(0),
	scan_cpp_directive(false), scan_cpp_line(false),
	in_function(false), in_dox_comment(false),
	chars_read_at_bol(0), stmt_bracket_balance(0), line_bracket_balance(0),
	saw_non_semicolon_keyword(false), saw_unindent(false),
	saw_comment(false), saw_cpp_directive(false), indentation_list(false) {}
	void calculate_metrics() {
		calculate_metrics_loop();
		// No newline at EOF
		if (chars_read_at_bol != src.get_nchar())
			qm.add_line(src.get_nchar() - chars_read_at_bol);
	}
	const QualityMetrics& get_metrics() const { return qm; }
	void enable_indentation_list() { indentation_list = true; }
};
#endif /* CMETRICSCALCULATOR_H */
