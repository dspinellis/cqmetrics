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

#include "BolState.h"
#include "CharSource.h"
#include "QualityMetrics.h"
#include "CMetricsCalculator.h"

inline bool
CMetricsCalculator::calculate_metrics_switch()
{
	int n;
	char c0, c1;

#define GET(x) do { \
	if (!src.get(x)) \
		return false; \
} while (0)

	GET(c0);
	switch (c0) {
	/*
	 * Single character C operators and punctuators
	 * ANSI 3.1.5 p. 32 and 3.1.6 p. 33
	 */
	case '\n':
		bol.saw_newline();
		qm.add_line();
		break;
	case ' ': case '\t': case '\v': case '\f': case '\r':
		bol.saw_space();
		break;
	case '[': case '(': case '~': case '?': case ',':
		bol.saw_non_space();
		qm.add_operator(c0);
		break;
	case ']': case ')': case ':':
		bol.saw_non_space();
		break;
	case '{':
		// Heuristic: functions begin with { at first column
		if (bol.at_bol() && current_depth == top_level_depth) {
			qm.begin_function();
			in_function = true;
		}
		bol.saw_non_space();
		current_depth++;
		break;
	case '}':
		bol.saw_non_space();
		current_depth--;
		if (in_function && current_depth == top_level_depth) {
			qm.end_function();
			in_function = false;
		}
		break;
	case ';':
		bol.saw_non_space();
		if (in_function)
			qm.add_statement();
		break;
	}
	return true;
}

void
CMetricsCalculator::calculate_metrics_loop()
{
	while (calculate_metrics_switch())
		;
}
