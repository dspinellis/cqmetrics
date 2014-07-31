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

#include "Cyclomatic.h"
#include "Descriptive.h"
#include "Halstead.h"

/** Keep taly of quality metrics */
class QualityMetrics {
private:
	int nchar;
	int nline;
	int ncomment;				// Number of comments
	int ncomment_char;			// Number of comment characters
	int nfunction;				// Number of functions
	int nstatement;				// Number of statements
	Descriptive<double> halstead;		// Halstead complexity
	Halstead halstead_tracker;
	Descriptive<double> cyclomatic;		// Cyclomatic complexity
	Cyclomatic cyclomatic_tracker;
public:
	QualityMetrics() :
		nchar(0), nline(0), ncomment(0), ncomment_char(0), nfunction(0),
		nstatement(0) {}

	void add_line() { nline++; }
	void add_statement() { nstatement++; }
	void add_comment() { ncomment++; }
	void add_comment_char() { ncomment_char++; }
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
	void add_short_circuit_operator(const std::string &s) {
		halstead_tracker.add(s);
		cyclomatic_tracker.add_boolean_branch();
	}
	void add_operator(char c) {
		add_operator(std::string(1, c));
	}

	int get_nchar() const { return nchar; }
	int get_nline() const { return nline; }
	int get_nfunction() const { return nfunction; }
	int get_nstatement() const { return nstatement; }
	int get_ncomment() const { return ncomment; }
	int get_ncomment_char() const { return ncomment_char; }
	const Descriptive<double>& get_halstead() const { return halstead; }
	const Descriptive<double>& get_cyclomatic() const { return cyclomatic; }
};
#endif /* QUALITYMETRICS_H */
