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
		if (bol.at_bol_space()) {
			qm.add_cpp_directive();
			if (in_function)
				qm.add_fun_cpp_directive();
			scan_cpp_directive = true;
		}
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
			if (in_function)
				qm.add_fun_comment();
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
			if (in_function)
				qm.add_fun_comment();
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
			goto number;
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
	/* Could be a long character or string */
	case 'L':
		bol.saw_non_space();
		GET(c1);
		switch (c1) {
		case '\'':
			goto char_literal;
		case '"':
			goto string_literal;
		default:
			src.push(c1);
			goto identifier;
		}
	case '_': case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
	case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm':
	case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't':
	case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
	case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
	case 'H': case 'I': case 'J': case 'K': case 'M': case 'N': case 'O':
	case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V':
	case 'W': case 'X': case 'Y': case 'Z':
	identifier:
		bol.saw_non_space();
		val = c0;
		for (;;) {
			GET(c0);
			if (!isalnum(c0) && c0 != '_')
				break;
			val += c0;
		}
		src.push(c0);
		switch (ckeyword.identifier_type(val)) {
		case CKeyword::WHILE:
		case CKeyword::FOR:
		case CKeyword::DEFAULT:
		case CKeyword::CASE:
			qm.add_path();
			break;
		case CKeyword::GOTO:
			qm.add_goto();
			break;
		case CKeyword::TYPEDEF:
			qm.add_typedef();
			break;
		case CKeyword::OTHER:
			break;
		case CKeyword::INCLUDE:
			if (scan_cpp_directive) {
				qm.add_cpp_include();
				break;
			}
			goto identifier;
		case CKeyword::IF:
			if (scan_cpp_directive) {
				qm.add_cpp_conditional();
				if (in_function)
					qm.add_fun_cpp_conditional();
			} else
				qm.add_path();
			break;
		case CKeyword::IFDEF:
		case CKeyword::ELIF:
			if (scan_cpp_directive) {
				// #if
				if (in_function)
					qm.add_fun_cpp_conditional();
				else
					qm.add_cpp_conditional();
				break;
			}
			goto plain_identifier;
		case CKeyword::IDENTIFIER:
		plain_identifier:
			if (!scan_cpp_directive) {
				qm.add_operand(val);
				qm.add_identifier(val);
			}
			break;
		}
		scan_cpp_directive = false;
		break;
	case '\'':
		bol.saw_non_space();
	char_literal:
		val = "";
		for (;;) {
			GET(c0);
			if (c0 == '\\') {
				// Consume one character after the backslash
				// ... to deal with the '\'' problem
				val += '\\';
				GET(c0);
				val += c0;
				continue;
			}
			if (c0 == '\'')
				break;
			val += c0;
		}
		qm.add_operand(val);
		break;
	case '"':
		bol.saw_non_space();
	string_literal:
		val = "";
		for (;;) {
			GET(c0);
			if (c0 == '\\') {
				val += '\\';
				// Consume one character after the backslash
				GET(c0);
				if (c0 == '\n')
					break;
				val += c0;
				// We will deal with escapes later
				continue;
			}
			if (c0 == '\n' || c0 == '"')
				break;
			val += c0;
		}
		qm.add_operand(val);
		break;
	/* Various numbers */
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
		bol.saw_non_space();
		val = c0;
	number:
		for (;;) {
			GET(c0);
			if (c0 == 'e' || c0 == 'E') {
				val += c0;
				GET(c0);
				if (c0 == '+' || c0 == '-') {
					val += c0;
					continue;
				}
			}
			if (!isalnum(c0) && c0 != '.' && c0 != '_')
				break;
			val += c0;
		}
		src.push(c0);
		qm.add_operand(val);
		break;
	default:
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
