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

#include <cctype>
#include <string>

#include "BolState.h"
#include "CharSource.h"
#include "QualityMetrics.h"
#include "CMetricsCalculator.h"

#if defined(SHOW_STYLE_HINTS)
#define STYLE_HINT(x) qm.add_style_hint(#x, QualityMetrics::x)
#else
#define STYLE_HINT(x) qm.add_style_hint(QualityMetrics::x)
#endif

static inline bool
is_eol_char(char c) {
	return (c == '\r' || c == '\n');
}

void
CMetricsCalculator::binary_style(char before)
{
	// Avoid complaining about missing space after <stdio.h>
	if (scan_cpp_line)
		return;
	if (isspace(before))
		STYLE_HINT(SPACE_BEFORE_BINARY_OP);
	else
		STYLE_HINT(NO_SPACE_BEFORE_BINARY_OP);
	if (isspace(src.char_after()))
		STYLE_HINT(SPACE_AFTER_BINARY_OP);
	else
		STYLE_HINT(NO_SPACE_AFTER_BINARY_OP);
}

void
CMetricsCalculator::keyword_style(char before, char allowed)
{
	if (scan_cpp_line)
		return;
	if (before != allowed)
		if (isspace(before))
			STYLE_HINT(SPACE_BEFORE_KEYWORD);
		else
			STYLE_HINT(NO_SPACE_BEFORE_KEYWORD);
	if (isspace(src.char_after()))
		STYLE_HINT(SPACE_AFTER_KEYWORD);
	else
		STYLE_HINT(NO_SPACE_AFTER_KEYWORD);
}

void
CMetricsCalculator::keyword_style_left_space(char before)
{
	if (scan_cpp_line)
		return;
	if (isspace(before))
		STYLE_HINT(SPACE_BEFORE_KEYWORD);
	else
		STYLE_HINT(NO_SPACE_BEFORE_KEYWORD);
}

void
CMetricsCalculator::newline()
{
	char c = src.char_before();
	// Skip over \r in \r\n
	if (c == '\r')
		c = src.char_before(2);
	// Allow empty lines
	if (c != '\n' && isspace(c))
		STYLE_HINT(SPACE_AT_END_OF_LINE);

	bol.saw_newline();
	// Line length minus the newline
	qm.add_line(src.get_nchar() - chars_read_at_bol - 1);
	chars_read_at_bol = src.get_nchar();
}

inline bool
CMetricsCalculator::calculate_metrics_switch()
{
	int n;
	char c0, c1;
	char before;	// The character before the current token
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
		newline();
		scan_cpp_line = false;
		break;
	case ' ': case '\t': case '\v': case '\f': case '\r':
		bol.saw_space();
		break;
	case '?':
		bol.saw_non_space();
		binary_style(src.char_before());
		qm.add_short_circuit_operator("?");
		break;
	case '[':
		if (isspace(src.char_before()))
			STYLE_HINT(SPACE_BEFORE_OPENING_SQUARE_BRACKET);
		else
			STYLE_HINT(NO_SPACE_BEFORE_OPENING_SQUARE_BRACKET);
		if (isspace(src.char_after()))
			STYLE_HINT(SPACE_AFTER_OPENING_SQUARE_BRACKET);
		else
			STYLE_HINT(NO_SPACE_AFTER_OPENING_SQUARE_BRACKET);
		bol.saw_non_space();
		qm.add_operator(c0);
		break;
	case '(':
		bol.saw_non_space();
		qm.add_operator(c0);
		break;
	case '~':
		/*
		 * Unary operators can have space before them
		 * as in "a | ~b", or not, as in "foo(~b)".
		 */
		if (isspace(src.char_after()))
			STYLE_HINT(SPACE_AFTER_UNARY_OP);
		else
			STYLE_HINT(NO_SPACE_AFTER_UNARY_OP);
		bol.saw_non_space();
		qm.add_operator(c0);
		break;
	case ',':
		if (isspace(src.char_before()))
			STYLE_HINT(SPACE_BEFORE_COMMA);
		else
			STYLE_HINT(NO_SPACE_BEFORE_COMMA);
		if (isspace(src.char_after()))
			STYLE_HINT(SPACE_AFTER_COMMA);
		else
			STYLE_HINT(NO_SPACE_AFTER_COMMA);
		bol.saw_non_space();
		qm.add_operator(c0);
		break;
	case ']':
		if (isspace(src.char_before()))
			STYLE_HINT(SPACE_BEFORE_CLOSING_SQUARE_BRACKET);
		else
			STYLE_HINT(NO_SPACE_BEFORE_CLOSING_SQUARE_BRACKET);
		bol.saw_non_space();
		break;
	case ')':
		if (isspace(src.char_before()))
			STYLE_HINT(SPACE_BEFORE_CLOSING_BRACKET);
		else
			STYLE_HINT(NO_SPACE_BEFORE_CLOSING_BRACKET);
		bol.saw_non_space();
		break;
	case '{':
		if (in_function)
			if (isspace(src.char_before()))
				STYLE_HINT(SPACE_BEFORE_OPENING_BRACE);
			else
				STYLE_HINT(NO_SPACE_BEFORE_OPENING_BRACE);
		if (in_function)
			if (isspace(src.char_after()))
				STYLE_HINT(SPACE_AFTER_OPENING_BRACE);
			else
				STYLE_HINT(NO_SPACE_AFTER_OPENING_BRACE);
		// Heuristic: functions begin with { at first column
		if (bol.at_bol() && current_depth == top_level_depth) {
			qm.begin_function();
			in_function = true;
		}
		bol.saw_non_space();
		current_depth++;
		break;
	case '}':
		if (in_function)
			if (isspace(src.char_before())) {
				if (!is_eol_char(src.char_before()))
					STYLE_HINT(SPACE_BEFORE_CLOSING_BRACE);
			} else
				STYLE_HINT(NO_SPACE_BEFORE_CLOSING_BRACE);
		if (in_function)
			if (isspace(src.char_after())) {
				if (!is_eol_char(src.char_after()))
					STYLE_HINT(SPACE_AFTER_CLOSING_BRACE);
			} else
				STYLE_HINT(NO_SPACE_AFTER_CLOSING_BRACE);
		bol.saw_non_space();
		current_depth--;
		if (in_function && current_depth == top_level_depth) {
			qm.end_function();
			in_function = false;
		}
		break;
	case ';':
		// Allow a single ; on a line
		if (!bol.at_bol_space())
			if (isspace(src.char_before()))
				STYLE_HINT(SPACE_BEFORE_SEMICOLON);
			else
				STYLE_HINT(NO_SPACE_BEFORE_SEMICOLON);
		if (src.char_after() != ';' &&		// Handle "for (;;)"
		    src.char_after() != ')')
			if (isspace(src.char_after())) {
				if (!is_eol_char(src.char_after()))
					STYLE_HINT(SPACE_AFTER_SEMICOLON);
			} else
				STYLE_HINT(NO_SPACE_AFTER_SEMICOLON);
		bol.saw_non_space();
		if (in_function)
			qm.add_statement(current_depth);
		break;
	/*
	 * Double character C tokens with more than 2 different outcomes
	 * (e.g. +, +=, ++)
	 */
	case '+':
		bol.saw_non_space();
		before = src.char_before();
		GET(c0);
		switch (c0) {
		case '+':
			// Could be prefix or postfix; no style checks
			if (in_function) qm.add_operator("++");
			break;
		case '=':
			binary_style(before);
			if (in_function) qm.add_operator("+=");
			break;
		default:
			// Could be unary operator, no style critique here
			src.push(c0);
			if (in_function) qm.add_operator('+');
			break;
		}
		break;
	case '-':
		bol.saw_non_space();
		before = src.char_before();
		GET(c0);
		switch (c0) {
		case '-':
			// Could be prefix or postfix; no style checks
			if (in_function) qm.add_operator("--");
			break;
		case '=':
			binary_style(before);
			if (in_function) qm.add_operator("-=");
			break;
		case '>':
			if (isspace(before))
				STYLE_HINT(SPACE_BEFORE_STRUCT_OP);
			else
				STYLE_HINT(NO_SPACE_BEFORE_STRUCT_OP);
			if (isspace(src.char_after()))
				STYLE_HINT(SPACE_AFTER_STRUCT_OP);
			else
				STYLE_HINT(NO_SPACE_AFTER_STRUCT_OP);
			if (in_function) qm.add_operator("->");
			break;
		default:
			// Could be unary operator, no style critique here
			src.push(c0);
			if (in_function) qm.add_operator('-');
			break;
		}
		break;
	case '&':
		bol.saw_non_space();
		before = src.char_before();
		GET(c0);
		switch (c0) {
		case '&':
			binary_style(before);
			if (in_function) qm.add_short_circuit_operator("&&");
			break;
		case '=':
			binary_style(before);
			if (in_function) qm.add_operator("&=");
			break;
		default:
			// Could be unary operator, no style critique here
			src.push(c0);
			if (in_function) qm.add_operator('&');
			break;
		}
		break;
	case '|':
		bol.saw_non_space();
		before = src.char_before();
		GET(c0);
		switch (c0) {
		case '|':
			binary_style(before);
			if (in_function) qm.add_short_circuit_operator("||");
			break;
		case '=':
			binary_style(before);
			if (in_function) qm.add_operator("|=");
			break;
		default:
			src.push(c0);
			binary_style(before);
			if (in_function) qm.add_operator('|');
			break;
		}
		break;
	case ':':
		bol.saw_non_space();
		GET(c0);
		switch (c0) {
		case ':':		// C++ ::
			break;
		default:
			src.push(c0);
			// Can be "a ? b : c" or "case 42:"
			if (isspace(src.char_after()))
				STYLE_HINT(SPACE_AFTER_BINARY_OP);
			else
				STYLE_HINT(NO_SPACE_AFTER_BINARY_OP);
			break;
		}
		break;
	/* Simple single/double character tokens (e.g. !, !=) */
	case '!':
		bol.saw_non_space();
		before = src.char_before();
		GET(c0);
		if (c0 == '=') {
			binary_style(before);
			if (in_function) qm.add_operator("!=");
		} else {
			src.push(c0);
			if (isspace(src.char_after()))
				STYLE_HINT(SPACE_AFTER_UNARY_OP);
			else
				STYLE_HINT(NO_SPACE_AFTER_UNARY_OP);
			if (in_function) qm.add_operator('!');
		}
		break;
	case '%':
		bol.saw_non_space();
		before = src.char_before();
		GET(c0);
		if (c0 == '=') {
			binary_style(before);
			if (in_function) qm.add_operator("%=");
			break;
		}
		src.push(c0);
		binary_style(before);
		if (in_function) qm.add_operator('%');
		break;
	case '*':
		bol.saw_non_space();
		before = src.char_before();
		GET(c0);
		if (c0 == '=') {
			binary_style(before);
			if (in_function) qm.add_operator("*=");
		} else {
			// Could be unary or binary
			// All bets are off regarding style
			src.push(c0);
			if (in_function) qm.add_operator('*');
		}
		break;
	case '=':
		bol.saw_non_space();
		before = src.char_before();
		GET(c0);
		if (c0 == '=') {
			binary_style(before);
			if (in_function) qm.add_operator("==");
		} else {
			src.push(c0);
			binary_style(before);
			if (in_function) qm.add_operator('=');
		}
		break;
	case '^':
		bol.saw_non_space();
		before = src.char_before();
		GET(c0);
		if (c0 == '=') {
			binary_style(before);
			if (in_function) qm.add_operator("^=");
		} else {
			src.push(c0);
			binary_style(before);
			if (in_function) qm.add_operator('^');
		}
		break;
	case '#':
		if (bol.at_bol_space()) {
			qm.add_cpp_directive();
			if (in_function)
				qm.add_fun_cpp_directive();
			scan_cpp_directive = true;
			scan_cpp_line = true;
		}
		bol.saw_non_space();
		break;
	/* Operators starting with < or > */
	case '>':
		bol.saw_non_space();
		before = src.char_before();
		GET(c0);
		switch (c0) {
		case '=':				/* >= */
			binary_style(before);
			if (in_function) qm.add_operator(">=");
			break;
		case '>':
			GET(c0);
			if (c0 == '=') {	/* >>= */
				binary_style(before);
				if (in_function) qm.add_operator(">>=");
			} else {			/* >> */
				src.push(c0);
				binary_style(before);
				if (in_function) qm.add_operator(">>");
			}
			break;
		default:				/* > */
			src.push(c0);
			binary_style(before);
			if (in_function) qm.add_operator('>');
			break;
		}
		break;
	case '<':
		bol.saw_non_space();
		before = src.char_before();
		GET(c0);
		switch (c0) {
		case '=':				/* <= */
			binary_style(before);
			if (in_function) qm.add_operator("<=");
			break;
		case '<':
			GET(c0);
			if (c0 == '=') {	/* <<= */
				binary_style(before);
				if (in_function) qm.add_operator("<<=");
			} else {			/* << */
				src.push(c0);
				binary_style(before);
				if (in_function) qm.add_operator("<<");
			}
			break;
		default:				/* < */
			src.push(c0);
			binary_style(before);
			if (in_function) qm.add_operator('<');
			break;
		}
		break;
	/* Comments and / operators */
	case '/':
		bol.saw_non_space();
		before = src.char_before();
		GET(c0);
		switch (c0) {
		case '=':				/* /= */
			binary_style(before);
			if (in_function) qm.add_operator("/=");
			break;
		case '*':				/* Block comment */
			qm.add_comment();
			if (in_function)
				qm.add_fun_comment();
			GET(c0);
			if (c0 == '\n')
				newline();
			for (;;) {
				while (c0 != '*') {
					qm.add_comment_char();
					GET(c0);
					if (c0 == '\n')
						newline();
				}
				GET(c0);
				if (c0 == '/')
					break;
				else {
					if (c0 == '\n')
						newline();
					qm.add_comment_char();
				}
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
			binary_style(before);
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
			if (isspace(src.char_before()))
				STYLE_HINT(SPACE_BEFORE_STRUCT_OP);
			else
				STYLE_HINT(NO_SPACE_BEFORE_STRUCT_OP);
			if (isspace(src.char_after()))
				STYLE_HINT(SPACE_AFTER_STRUCT_OP);
			else
				STYLE_HINT(NO_SPACE_AFTER_STRUCT_OP);
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
		before = src.char_before();
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
		case CKeyword::CASE:
			keyword_style(before);
			qm.add_path();
			break;
		case CKeyword::DEFAULT:
			keyword_style_left_space(before);
			qm.add_path();
			break;
		case CKeyword::GOTO:
			keyword_style(before);
			qm.add_goto();
			break;
		case CKeyword::REGISTER:
			keyword_style(before);
			qm.add_register();
			break;
		case CKeyword::TYPEDEF:
			qm.add_typedef();
			break;
		case CKeyword::ENUM:
		case CKeyword::STRUCT:
		case CKeyword::UNION:
			keyword_style(before, '(');
			break;
		case CKeyword::DO:
		case CKeyword::SWITCH:
			keyword_style(before);
			break;
		case CKeyword::BREAK:
		case CKeyword::CONTINUE:
		case CKeyword::RETURN:
			keyword_style_left_space(before);
			break;
		case CKeyword::OTHER:
			break;
		case CKeyword::INCLUDE:
			if (scan_cpp_directive) {
				qm.add_cpp_include();
				break;
			}
			goto identifier;
		case CKeyword::ELSE:
			if (!scan_cpp_directive)
				keyword_style(before);
			break;
		case CKeyword::IF:
			if (scan_cpp_directive) {
				qm.add_cpp_conditional();
				if (in_function)
					qm.add_fun_cpp_conditional();
			} else {
				keyword_style(before);
				qm.add_path();
			}
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
				val += c0;
				continue;
			}
			if (c0 == '"')
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
