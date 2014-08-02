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

std::ostream&
operator <<(std::ostream& o, const QualityMetrics &q) {
	o <<
		q.get_nchar() << '\t' <<
		q.get_line_length() << '\t' <<
		q.get_nfunction() << '\t' <<
		q.get_statement_nesting() << '\t' <<
		q.get_ngoto() << '\t' <<
		q.get_ntypedef() << '\t' <<
		q.get_ncomment() << '\t' <<
		q.get_ncomment_char() << '\t' <<
		q.get_nfun_comment() << '\t' <<

		q.get_ncpp_directive() << '\t' <<
		q.get_ncpp_include() << '\t' <<
		q.get_ncpp_conditional() << '\t' <<
		q.get_nfun_cpp_directive() << '\t' <<
		q.get_nfun_cpp_conditional() << '\t' <<

		q.get_halstead() << '\t' <<
		q.get_cyclomatic() << '\t' <<
		q.get_identifier_length() << '\t' <<
		q.get_unique_identifier_length();

	return o;
}
