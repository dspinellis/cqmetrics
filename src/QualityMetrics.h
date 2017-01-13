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

#ifndef QUALITYMETRICS_H
#define QUALITYMETRICS_H

#include <iostream>
#include <ostream>
#include <set>
#include <string>
#include <vector>

#include "Cyclomatic.h"
#include "Descriptive.h"
#include "Halstead.h"

/** Keep taly of quality metrics */
class QualityMetrics {
public:
	typedef std::vector <int> StyleHintContainer;
private:
	Descriptive<int> line_length;	// Line lengths
	int ncomment;			// Number of comments
	int ncomment_char;		// Number of comment characters
	int nboilerplate_comment_char;	// Number of boilerplate (license) comment characters
	int ndox_comment;		// Number of DOxygen comments
	int ndox_comment_char;		// Number of DOxygen comment characters
	int nfunction;			// Number of functions
	int ncpp_directive;		// Number of C preprocessor directives

	int ncpp_include;		// Number of include directives
	int ninternal;			// Number of internal visibility
					// declarations (static in file scope)
	int nconst;			// Number of const keywords
	int nenum;			// Number of enum keywords
	int ngoto;			// Number of goto statements
	int ninline;			// Number of inline keywords
	int nnoalias;			// Number of noalias keywords
	int nregister;			// Number of register keywords
	int nrestrict;			// Number of restrict keywords
	int nsigned;			// Number of signed keywords
	int nstruct;			// Number of struct keywords
	int ntypedef;			// Number of type definitions
	int nunion;			// Number of union keywords
	int nunsigned;			// Number of unsigned keywords
	int nvoid;			// Number of void keywords
	int nvolatile;			// Number of volatile keywords
	int nfun_comment;		// Number of comments in functions
	int nfun_cpp_directive;		// Number of C preprocessor directives
					// in functions
	int ncpp_conditional;		// Number of C preprocessor conditionals
	int nfun_cpp_conditional;	// Number of C preprocessor conditionals
					// in functions

	std::set <std::string> identifier;	// Unique identifiers
	Descriptive<int> identifier_length;
	Descriptive<int> unique_identifier_length;

	Descriptive<int> statement_nesting;	// Statement nesting
	Descriptive<double> halstead;		// Halstead complexity
	Halstead halstead_tracker;
	Descriptive<double> cyclomatic;		// Cyclomatic complexity
	Descriptive<double> indentation_spacing;// Spacing used for indentation
	Cyclomatic cyclomatic_tracker;
	StyleHintContainer nstyle_hint;
public:
	enum StyleHint {
		// These are commonly agreed style rule infractions
		NO_SPACE_AFTER_BINARY_OP,
		NO_SPACE_AFTER_CLOSING_BRACE,
		NO_SPACE_AFTER_COMMA,
		NO_SPACE_AFTER_KEYWORD,
		NO_SPACE_AFTER_OPENING_BRACE,
		NO_SPACE_AFTER_SEMICOLON,
		NO_SPACE_BEFORE_BINARY_OP,
		NO_SPACE_BEFORE_CLOSING_BRACE,
		NO_SPACE_BEFORE_KEYWORD,
		NO_SPACE_BEFORE_OPENING_BRACE,
		SPACE_AFTER_OPENING_SQUARE_BRACKET,
		SPACE_AFTER_STRUCT_OP,
		SPACE_AFTER_UNARY_OP,
		SPACE_AT_END_OF_LINE,
		SPACE_BEFORE_CLOSING_BRACKET,
		SPACE_BEFORE_CLOSING_SQUARE_BRACKET,
		SPACE_BEFORE_COMMA,
		SPACE_BEFORE_OPENING_SQUARE_BRACKET,
		SPACE_BEFORE_SEMICOLON,
		SPACE_BEFORE_STRUCT_OP,
		// We also track the opposite rules to measure inconsistency
		SPACE_AFTER_BINARY_OP,
		SPACE_AFTER_CLOSING_BRACE,
		SPACE_AFTER_COMMA,
		SPACE_AFTER_KEYWORD,
		SPACE_AFTER_OPENING_BRACE,
		SPACE_AFTER_SEMICOLON,
		NO_SPACE_AFTER_STRUCT_OP,
		SPACE_BEFORE_BINARY_OP,
		SPACE_BEFORE_CLOSING_BRACE,
		SPACE_BEFORE_KEYWORD,
		SPACE_BEFORE_OPENING_BRACE,
		NO_SPACE_BEFORE_STRUCT_OP,
		NO_SPACE_AFTER_OPENING_SQUARE_BRACKET,
		NO_SPACE_AFTER_UNARY_OP,
		NO_SPACE_BEFORE_CLOSING_BRACKET,
		NO_SPACE_BEFORE_CLOSING_SQUARE_BRACKET,
		NO_SPACE_BEFORE_COMMA,
		NO_SPACE_BEFORE_OPENING_SQUARE_BRACKET,
		NO_SPACE_BEFORE_SEMICOLON,
		// Add more elements here
		STYLE_HINT_SIZE
	};

	// The name for each metric in the enum. Needed for test reporting.
	static const char *metric_name[];

	QualityMetrics() :
		ncomment(0), ncomment_char(0), nboilerplate_comment_char(0),
		ndox_comment(0), ndox_comment_char(0), nfunction(0),
		ncpp_directive(0), ncpp_include(0), ninternal(0), nconst(0),
		nenum(0), ngoto(0), ninline(0), nnoalias(0), nregister(0),
		nrestrict(0), nsigned(0), nstruct(0), ntypedef(0), nunion(0),
		nunsigned(0), nvoid(0), nvolatile(0), nfun_comment(0),
		nfun_cpp_directive(0), ncpp_conditional(0),
		nfun_cpp_conditional(0), nstyle_hint(STYLE_HINT_SIZE, 0) {}

	void add_line(int length) { line_length.add(length); }
	void add_statement(int nesting) { statement_nesting.add(nesting); }
	void add_internal() { ninternal++; }
	void add_const() { nconst++; }
	void add_enum() { nenum++; }
	void add_goto() { ngoto++; }
	void add_inline() { ninline++; }
	void add_noalias() { nnoalias++; }
	void add_register() { nregister++; }
	void add_restrict() { nrestrict++; }
	void add_signed() { nsigned++; }
	void add_struct() { nstruct++; }
	void add_typedef() { ntypedef++; }
	void add_union() { nunion++; }
	void add_unsigned() { nunsigned++; }
	void add_void() { nvoid++; }
	void add_volatile() { nvolatile++; }
	void add_comment() { ncomment++; }
	void add_fun_comment() { nfun_comment++; }
	void add_comment_char() { ncomment_char++; }
	void add_boilerplate_comment_chars(int n) {
		nboilerplate_comment_char += n;
	}
	void add_dox_comment() { ndox_comment++; }
	void add_dox_comment_char() { ndox_comment_char++; }
	void remove_dox_comment_char() { ndox_comment_char--; }
	void add_cpp_directive() { ncpp_directive++; }
	void add_cpp_conditional() { ncpp_conditional++; }
	void add_cpp_include() { ncpp_include++; }
	void add_fun_cpp_directive() { nfun_cpp_directive++; }
	void add_fun_cpp_conditional() { nfun_cpp_conditional++; }
#if defined(SHOW_STYLE_HINTS)
	void add_style_hint(const std::string& name, enum StyleHint num) {
		std::cerr << line_length.get_count() + 1 <<
			": " << name << std::endl;
		nstyle_hint[num]++;
	}
#else
	void add_style_hint(enum StyleHint num) { nstyle_hint[num]++; }
#endif
	void begin_function() {
		halstead_tracker.reset();
		cyclomatic_tracker.reset();
		nfunction++;
	}
	void end_function() {
		halstead.add(halstead_tracker.complexity());
		cyclomatic.add(cyclomatic_tracker.extended_complexity());
	}
	void add_operator(const std::string &s) {
		halstead_tracker.add(s);
	}
	void add_operand(const std::string &s) {
		halstead_tracker.add(s);
	}
	void add_path() {
		cyclomatic_tracker.add_path();
	}
	void add_short_circuit_operator(const std::string &s) {
		halstead_tracker.add(s);
		cyclomatic_tracker.add_boolean_branch();
	}
	void add_operator(char c) {
		add_operator(std::string(1, c));
	}
	void add_operand(char c) {
		add_operand(std::string(1, c));
	}
	void add_indentation_spacing(double s) {
		indentation_spacing.add(s);
	}
	void add_identifier(const std::string s) {
		if (identifier.find(s) == identifier.end()) {
			unique_identifier_length.add(s.length());
			identifier.insert(s);
		}
		identifier_length.add(s.length());
	}

	const Descriptive<int>& get_line_length() const { return line_length; }
	int get_nchar() const {
		return line_length.get_sum() + line_length.get_count();
	}
	int get_nfunction() const { return nfunction; }
	const Descriptive<int> &get_statement_nesting() const {
		return statement_nesting;
	}
	int get_ninternal() const { return ninternal; }
	int get_nconst() const { return nconst; }
	int get_nenum() const { return nenum; }
	int get_ngoto() const { return ngoto; }
	int get_ninline() const { return ninline; }
	int get_nnoalias() const { return nnoalias; }
	int get_nregister() const { return nregister; }
	int get_nrestrict() const { return nrestrict; }
	int get_nsigned() const { return nsigned; }
	int get_nstruct() const { return nstruct; }
	int get_ntypedef() const { return ntypedef; }
	int get_nunion() const { return nunion; }
	int get_nunsigned() const { return nunsigned; }
	int get_nvoid() const { return nvoid; }
	int get_nvolatile() const { return nvolatile; }
	int get_ncomment() const { return ncomment; }
	int get_ncomment_char() const { return ncomment_char; }
	int get_nboilerplate_comment_char() const { return nboilerplate_comment_char; }
	int get_ndox_comment() const { return ndox_comment; }
	int get_ndox_comment_char() const { return ndox_comment_char; }
	int get_nfun_comment() const { return nfun_comment; }
	/// Return the code's weighted style inconsistency.
	double get_style_inconsistency() const;

	/*
	 * The global preprocessor directive functions include in their
	 * result the directives that occur within function bodies.
	 */
	int get_ncpp_directive() const { return ncpp_directive; }
	int get_ncpp_include() const { return ncpp_include; }
	int get_ncpp_conditional() const { return ncpp_conditional; }
	int get_nfun_cpp_directive() const { return nfun_cpp_directive; }
	int get_nfun_cpp_conditional() const { return nfun_cpp_conditional; }

	const Descriptive<double>& get_halstead() const { return halstead; }
	const Descriptive<double>& get_cyclomatic() const { return cyclomatic; }
	const Descriptive<double>& get_indentation_spacing() const {
		return indentation_spacing;
	}
	const Descriptive<int>& get_identifier_length() const {
		return identifier_length;
	}
	const Descriptive<int>& get_unique_identifier_length() const {
		return unique_identifier_length;
	}
	int get_style_hint(enum StyleHint e) const { return nstyle_hint[e]; }
	const StyleHintContainer& get_style_hint() const { return nstyle_hint; }

	friend class QualityMetricsTest;
private:
	/// Accumulate # cases and sum of two inconsistency measures.
	void inconsistency_accumulate(enum StyleHint a, enum StyleHint b,
		int& ncases, int& inc_sum) const;
	 /// Unweighted inconsistency of two complimentary choices a and b.
	static int inconsistency(int a, int b);
};

std::ostream& operator <<(std::ostream& o, const QualityMetrics &q);

#endif /* QUALITYMETRICS_H */
