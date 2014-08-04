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

std::ostream&
operator <<(std::ostream& o, const QualityMetrics &q) {
	o <<
		q.get_nchar() << '\t' <<
		ANNOTATE(" line len: " <<)
		q.get_line_length() << '\t' <<
		ANNOTATE(" fun: " <<)
		q.get_nfunction() << '\t' <<
		ANNOTATE(" nest: " <<)
		q.get_statement_nesting() << '\t' <<
		ANNOTATE(" ngoto: " <<)
		q.get_ngoto() << '\t' <<
		ANNOTATE(" ntypedef: " <<)
		q.get_ntypedef() << '\t' <<
		ANNOTATE(" comment: " <<)
		q.get_ncomment() << '\t' <<
		q.get_ncomment_char() << '\t' <<
		q.get_nfun_comment() << '\t' <<

		ANNOTATE(" cpp: " <<)
		q.get_ncpp_directive() << '\t' <<
		q.get_ncpp_include() << '\t' <<
		q.get_ncpp_conditional() << '\t' <<
		q.get_nfun_cpp_directive() << '\t' <<
		q.get_nfun_cpp_conditional() << '\t' <<

		ANNOTATE(" halstead: " <<)
		q.get_halstead() << '\t' <<
		ANNOTATE(" cyclomatic: " <<)
		q.get_cyclomatic() << '\t' <<
		ANNOTATE(" id len: " <<)
		q.get_identifier_length() << '\t' <<
		ANNOTATE(" unique id len: " <<)
		q.get_unique_identifier_length();

	ANNOTATE(o << " style: ";)
	// One day: for (auto& e : get_style_error())
	for (QualityMetrics::StyleErrorContainer::const_iterator e =
			q.get_style_error().begin();
			e != q.get_style_error().end(); e++)
		o << '\t' << *e;
	return o;
}
