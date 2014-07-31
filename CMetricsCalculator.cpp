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

#include <string>

#include "BolState.h"
#include "CharSource.h"
#include "QualityMetrics.h"
#include "CMetricsCalculator.h"

inline bool
CMetricsCalculator::calculate_metrics_switch()
{
	int n;
	char c0, c1;
	std::string val;

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
	case '?':
		bol.saw_non_space();
		qm.add_short_circuit_operator("?");
		break;
	case '[': case '(': case '~': case ',':
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
	/*
	 * Double character C tokens with more than 2 different outcomes
	 * (e.g. +, +=, ++)
	 */
	case '+':
		bol.saw_non_space();
		GET(c0);
		switch (c0) {
		case '+': if (in_function) qm.add_operator("++"); break;
		case '=': if (in_function) qm.add_operator("+="); break;
		default:  src.push(c0); if (in_function) qm.add_operator('+'); break;
		}
		break;
	case '-':
		bol.saw_non_space();
		GET(c0);
		switch (c0) {
		case '-': if (in_function) qm.add_operator("--"); break;
		case '=': if (in_function) qm.add_operator("-="); break;
		case '>': if (in_function) qm.add_operator("->"); break;
		default:  src.push(c0); if (in_function) qm.add_operator('-'); break;
		}
		break;
	case '&':
		bol.saw_non_space();
		GET(c0);
		switch (c0) {
		case '&': if (in_function) qm.add_short_circuit_operator("&&"); break;
		case '=': if (in_function) qm.add_operator("&="); break;
		default:  src.push(c0); if (in_function) qm.add_operator('&'); break;
		}
		break;
	case '|':
		bol.saw_non_space();
		GET(c0);
		switch (c0) {
		case '|': if (in_function) qm.add_short_circuit_operator("||"); break;
		case '=': if (in_function) qm.add_operator("|="); break;
		default:  src.push(c0); if (in_function) qm.add_operator('|'); break;
		}
		break;
	/* Simple single/double character tokens (e.g. !, !=) */
	case '!':
		bol.saw_non_space();
		GET(c0);
		if (c0 == '=') {
			if (in_function) qm.add_operator("!=");
		} else {
			src.push(c0);
			if (in_function) qm.add_operator('!');
		}
		break;
	case '%':
		bol.saw_non_space();
		GET(c0);
		if (c0 == '=') {
			if (in_function) qm.add_operator("%=");
			break;
		}
		src.push(c0);
		if (in_function) qm.add_operator('%');
		break;
	case '*':
		bol.saw_non_space();
		GET(c0);
		if (c0 == '=') {
			if (in_function) qm.add_operator("*=");
		} else {
			src.push(c0);
			if (in_function) qm.add_operator('*');
		}
		break;
	case '=':
		bol.saw_non_space();
		GET(c0);
		if (c0 == '=') {
			if (in_function) qm.add_operator("==");
		} else {
			src.push(c0);
			if (in_function) qm.add_operator('=');
		}
		break;
	case '^':
		bol.saw_non_space();
		GET(c0);
		if (c0 == '=') {
			if (in_function) qm.add_operator("^=");
		} else {
			src.push(c0);
			if (in_function) qm.add_operator('^');
		}
		break;
	case '#':
		bol.saw_non_space();
		break;
	/* Operators starting with < or > */
	case '>':
		bol.saw_non_space();
		GET(c0);
		switch (c0) {
		case '=':				/* >= */
			if (in_function) qm.add_operator(">=");
			break;
		case '>':
			GET(c0);
			if (c0 == '=') {	/* >>= */
				if (in_function) qm.add_operator(">>=");
			} else {			/* >> */
				src.push(c0);
				if (in_function) qm.add_operator(">>");
			}
			break;
		default:				/* > */
			src.push(c0);
			if (in_function) qm.add_operator('>');
			break;
		}
		break;
	case '<':
		bol.saw_non_space();
		GET(c0);
		switch (c0) {
		case '=':				/* <= */
			if (in_function) qm.add_operator("<=");
			break;
		case '<':
			GET(c0);
			if (c0 == '=') {	/* <<= */
				if (in_function) qm.add_operator("<<=");
			} else {			/* << */
				src.push(c0);
				if (in_function) qm.add_operator("<<");
			}
			break;
		default:				/* < */
			src.push(c0);
			if (in_function) qm.add_operator('<');
			break;
		}
		break;
	/* Comments and / operators */
	case '/':
		bol.saw_non_space();
		GET(c0);
		switch (c0) {
		case '=':				/* /= */
			if (in_function) qm.add_operator("/=");
			break;
		case '*':				/* Block comment */
			qm.add_comment();
			GET(c0);
			for (;;) {
				while (c0 != '*') {
					qm.add_comment_char();
					GET(c0);
				}
				GET(c0);
				if (c0 == '/')
					break;
				else
					qm.add_comment_char();
			}
			break;
		case '/':				/* Line comment */
			qm.add_comment();
			for (;;) {
				GET(c0);
				if (c0 == '\n')
					break;
				else
					qm.add_comment_char();
			}
			src.push(c0);
			break;
		default:				/* / */
			src.push(c0);
			if (in_function) qm.add_operator('/');
			break;
		}
		break;
	case '.':	/* . and ... */
		bol.saw_non_space();
		GET(c0);
		if (isdigit(c0)) {
			val = std::string(".") + (char)(c0);
			// XXX goto pp_number;
		}
		if (c0 != '.') {
			src.push(c0);
			if (in_function) qm.add_operator('.');
			break;
		}
		GET(c1);
		if (c1 != '.') {
			src.push(c1);
			src.push(c0);
			if (in_function) qm.add_operator('.');
			break;
		}
		// Elipsis
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
