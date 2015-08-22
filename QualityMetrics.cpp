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

#include <ostream>

#include "QualityMetrics.h"

#if defined(ANNOTATE_OUTPUT)
#define ANNOTATE(x) x
#else
#define ANNOTATE(x)
#endif

const char *QualityMetrics::metric_name[] = {
#include "QualityMetricNames.h"
};

/**
 * Unweighted inconsistency of two complimentary choices a and b.
 * 0: none, N: N cases at odds with the rest, e.g. 2N cases, split
 * exactly in half (perfect inconsistency), or N < A.
 * This function is also known as min. :-)
 */
int
QualityMetrics::inconsistency(int a, int b)
{
	if (a < b)
		return a;
	else
		return b;
}

/// Accumulate the number of cases and sum of two complimentary inconsistency
/// measures.
void
QualityMetrics::inconsistency_accumulate(enum StyleHint a, enum StyleHint b,
		int& ncases, int& inc_sum) const
{
	int ca = nstyle_hint[a];
	int cb = nstyle_hint[b];

	ncases += ca + cb;
	inc_sum += inconsistency(ca, cb);
}

/// Calculate the code's weighted style inconsistency.
double
QualityMetrics::get_style_inconsistency() const
{
	int ncases = 0;
	int inc_sum = 0;

#define IA(a) inconsistency_accumulate(a, NO_ ## a, ncases, inc_sum)

	IA(SPACE_AFTER_BINARY_OP);
	IA(SPACE_AFTER_CLOSING_BRACE);
	IA(SPACE_AFTER_COMMA);
	IA(SPACE_AFTER_KEYWORD);
	IA(SPACE_AFTER_OPENING_BRACE);
	IA(SPACE_AFTER_SEMICOLON);
	IA(SPACE_AFTER_STRUCT_OP);
	IA(SPACE_BEFORE_BINARY_OP);
	IA(SPACE_BEFORE_CLOSING_BRACE);
	IA(SPACE_BEFORE_KEYWORD);
	IA(SPACE_BEFORE_OPENING_BRACE);
	IA(SPACE_BEFORE_STRUCT_OP);
	IA(SPACE_AFTER_OPENING_SQUARE_BRACKET);
	IA(SPACE_AFTER_UNARY_OP);
	IA(SPACE_BEFORE_CLOSING_BRACKET);
	IA(SPACE_BEFORE_CLOSING_SQUARE_BRACKET);
	IA(SPACE_BEFORE_COMMA);
	IA(SPACE_BEFORE_OPENING_SQUARE_BRACKET);
	IA(SPACE_BEFORE_SEMICOLON);

	return (double)inc_sum / (double)ncases;
}

std::ostream&
operator <<(std::ostream& o, const QualityMetrics &q) {
	// Use the VAL prefix in order to have README.md correctly
	// updated. (See make-readme.sh.)
	o <<
		// VAL: Number of characters
		q.get_nchar() << '\t' <<

		// VAL: Number of lines, minimum, mean, maximum, standard deviation of line length
		ANNOTATE(" line len: " <<)
		q.get_line_length() << '\t' <<

		// VAL: Number of functions (A function is considered to start with `{` at column 1)
		ANNOTATE(" fun: " <<)
		q.get_nfunction() << '\t' <<

		// VAL: Number of statements, minimum, mean, maximum, standard deviation of statement nesting
		ANNOTATE(" nest: " <<)
		q.get_statement_nesting() << '\t' <<

		// VAL: Number of declarations with internal linkage (`static`) visibility
		ANNOTATE(" ninternal: " <<)
		q.get_ninternal() << '\t' <<

		// VAL: Number of `const` keywords
		ANNOTATE(" nconst: " <<)
		q.get_nconst() << '\t' <<

		// VAL: Number of `enum` keywords
		ANNOTATE(" nenum: " <<)
		q.get_nenum() << '\t' <<

		// VAL: Number of `goto` statements
		ANNOTATE(" ngoto: " <<)
		q.get_ngoto() << '\t' <<

		// VAL: Number of `inline` keywords
		ANNOTATE(" ninline: " <<)
		q.get_ninline() << '\t' <<

		// VAL: Number of `noalias` keywords
		ANNOTATE(" nnoalias: " <<)
		q.get_nnoalias() << '\t' <<

		// VAL: Number of `register` keywords
		ANNOTATE(" nregister: " <<)
		q.get_nregister() << '\t' <<

		// VAL: Number of `restrict` keywords
		ANNOTATE(" nrestrict: " <<)
		q.get_nrestrict() << '\t' <<


		// VAL: Number of `signed` keywords
		ANNOTATE(" signed: " <<)
		q.get_nsigned() << '\t' <<

		// VAL: Number of `struct` keywords
		ANNOTATE(" struct: " <<)
		q.get_nstruct() << '\t' <<

		// VAL: Number of `union` keywords
		ANNOTATE(" union: " <<)
		q.get_nunion() << '\t' <<

		// VAL: Number of `unsigned` keywords
		ANNOTATE(" nunsigned: " <<)
		q.get_nunsigned() << '\t' <<

		// VAL: Number of `void` keywords
		ANNOTATE(" nvoid: " <<)
		q.get_nvoid() << '\t' <<

		// VAL: Number of `volatile` keywords
		ANNOTATE(" nvolatile: " <<)
		q.get_nvolatile() << '\t' <<

		// VAL: Number of `typedef` keywords
		ANNOTATE(" ntypedef: " <<)
		q.get_ntypedef() << '\t' <<

		// VAL: Number of comments
		ANNOTATE(" comment: " <<)
		q.get_ncomment() << '\t' <<

		// VAL: Number of comment characters
		q.get_ncomment_char() << '\t' <<

		// VAL: Number of boilerplate (license) comment characters
		q.get_nboilerplate_comment_char() << '\t' <<

		// VAL: Number of DOxygen comments
		ANNOTATE(" comment: " <<)
		q.get_ndox_comment() << '\t' <<

		// VAL: Number of DOxygen comment characters
		q.get_ndox_comment_char() << '\t' <<

		// VAL: Number of comments inside function bodies
		q.get_nfun_comment() << '\t' <<

		// VAL: Number of C preprocessor directives
		ANNOTATE(" cpp: " <<)
		q.get_ncpp_directive() << '\t' <<

		// VAL: Number of C preprocessor include directives
		q.get_ncpp_include() << '\t' <<

		// VAL: Number of C preprocessor conditional directives (`#if`, `#ifdef`, `#elif`)
		q.get_ncpp_conditional() << '\t' <<

		// VAL: Number of C preprocessor directives in function bodies
		q.get_nfun_cpp_directive() << '\t' <<

		// VAL: Number of C preprocessor conditional directives in function bodies
		q.get_nfun_cpp_conditional() << '\t' <<

		// VAL: Style inconsistency
		ANNOTATE(" inconsistency: " <<)
		q.get_style_inconsistency() << '\t' <<

		// VAL: Number of functions, minimum, mean, maximum, standard deviation of Halstead complexity per function
		ANNOTATE(" halstead: " <<)
		q.get_halstead() << '\t' <<

		// VAL: Number of functions, minimum, mean, maximum, standard deviation of cyclomatic complexity per function
		ANNOTATE(" cyclomatic: " <<)
		q.get_cyclomatic() << '\t' <<

		// VAL: Number of identifiers, minimum, mean, maximum, standard deviation of identifier length
		ANNOTATE(" id len: " <<)
		q.get_identifier_length() << '\t' <<

		// VAL: Number of unique identifiers, minimum, mean, maximum, standard deviation of unique identifier length
		ANNOTATE(" unique id len: " <<)
		q.get_unique_identifier_length() << '\t' <<

		// VAL: Number spaces used for one level of indentation, minimum, mean, maximum, standard deviation of spaces used for one level of indentation
		ANNOTATE(" identation spacing: " <<)
		q.get_indentation_spacing();

	ANNOTATE(o << " style: ";)
	// One day: for (auto& e : get_style_hint())
	for (QualityMetrics::StyleHintContainer::const_iterator e =
			q.get_style_hint().begin();
			e != q.get_style_hint().end(); e++)
		o << '\t' << *e;
	return o;
}
