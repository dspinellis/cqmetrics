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

		// VAL: Number of `goto` statements
		ANNOTATE(" ngoto: " <<)
		q.get_ngoto() << '\t' <<

		// VAL: Number of `register` keywords
		ANNOTATE(" nregister: " <<)
		q.get_nregister() << '\t' <<

		// VAL: Number of `typedef`
		ANNOTATE(" ntypedef: " <<)
		q.get_ntypedef() << '\t' <<

		// VAL: Number of comments
		ANNOTATE(" comment: " <<)
		q.get_ncomment() << '\t' <<

		// VAL: Number of comment characters
		q.get_ncomment_char() << '\t' <<

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

		// VAL: Number spaces used for one level indentation, minimum, mean, maximum, standard deviation of spaces used for one level of indentation
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
