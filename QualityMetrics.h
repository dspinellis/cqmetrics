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

#include <string>
#include <set>

#include "Cyclomatic.h"
#include "Descriptive.h"
#include "Halstead.h"

/** Keep taly of quality metrics */
class QualityMetrics {
private:
	int nchar;
	int nline;
	int ncomment;			// Number of comments
	int ncomment_char;		// Number of comment characters
	int nfunction;			// Number of functions
	int nstatement;			// Number of statements
	int ncpp_directive;		// Number of C preprocessor directives

	int ncpp_include;		// Number of include directives
	int ngoto;			// Number of goto statements
	int ntypedef;			// Number of type definitions
	int nfun_comment;		// Number of comments in functions
	int nfun_cpp_directive;		// Number of C preprocessor directives
					// in functions
	int ncpp_conditional;		// Number of C preprocessor conditionals
	int nfun_cpp_conditional;	// Number of C preprocessor conditionals
					// in functions

	std::set <std::string> identifier;	// Unique identifiers
	Descriptive<int> identifier_length;
	Descriptive<int> unique_identifier_length;

	Descriptive<double> halstead;	// Halstead complexity
	Halstead halstead_tracker;
	Descriptive<double> cyclomatic;	// Cyclomatic complexity
	Cyclomatic cyclomatic_tracker;
public:
	QualityMetrics() :
		nchar(0), nline(0), ncomment(0), ncomment_char(0), nfunction(0),
		nstatement(0), ncpp_directive(0), ncpp_include(0), ngoto(0),
		ntypedef(0), nfun_comment(0), nfun_cpp_directive(0),
		ncpp_conditional(0), nfun_cpp_conditional(0) {}

	void add_line() { nline++; }
	void add_statement() { nstatement++; }
	void add_goto() { ngoto++; }
	void add_typedef() { ntypedef++; }
	void add_comment() { ncomment++; }
	void add_fun_comment() { nfun_comment++; }
	void add_comment_char() { ncomment_char++; }
	void add_cpp_directive() { ncpp_directive++; }
	void add_cpp_conditional() { ncpp_conditional++; }
	void add_cpp_include() { ncpp_include++; }
	void add_fun_cpp_directive() { nfun_cpp_directive++; }
	void add_fun_cpp_conditional() { nfun_cpp_conditional++; }
	void begin_function() {
		halstead_tracker.reset();
		cyclomatic_tracker.reset();
		nfunction++;
	}
	void end_function() {
		halstead.add(halstead_tracker.complexity());
		cyclomatic.add(cyclomatic_tracker.extended_complexity());
	}
	void set_nchar(int n) { nchar = n; }
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
	void add_identifier(const std::string s) {
		if (identifier.find(s) == identifier.end()) {
			unique_identifier_length.add(s.length());
			identifier.insert(s);
		}
		identifier_length.add(s.length());
	}

	int get_nchar() const { return nchar; }
	int get_nline() const { return nline; }
	int get_nfunction() const { return nfunction; }
	int get_nstatement() const { return nstatement; }
	int get_ngoto() const { return ngoto; }
	int get_ntypedef() const { return ntypedef; }

	int get_ncomment() const { return ncomment; }
	int get_ncomment_char() const { return ncomment_char; }
	int get_nfun_comment() const { return nfun_comment; }

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
	const Descriptive<int>& get_identifier_length() const {
		return identifier_length;
	}
	const Descriptive<int>& get_unique_identifier_length() const {
		return unique_identifier_length;
	}
};
#endif /* QUALITYMETRICS_H */
