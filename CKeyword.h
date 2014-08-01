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

#ifndef CH
#define CH

#include <map>
#include <string>

/** Classify identifiers into C keywords */
class CKeyword {
public:
	enum IdentifierType {
		BRANCH_PATH,	// Keywords that introduce branch path
		GOTO,		// goto
		OTHER,		// All other keywords
		IDENTIFIER,	// Plain identifier (not a keyword)
	};
private:
	// Keyword map
	typedef std::map <std::string, enum IdentifierType> KeywordMap;
	KeywordMap km;
public:
	CKeyword() {
		km["auto"] = OTHER;
		km["break"] = OTHER;
		km["case"] = BRANCH_PATH;
		km["char"] = OTHER;
		km["const"] = OTHER;
		km["continue"] = OTHER;
		km["default"] = BRANCH_PATH;
		km["do"] = OTHER;	// Counter through while
		km["double"] = OTHER;
		km["else"] = OTHER;
		km["enum"] = OTHER;
		km["extern"] = OTHER;
		km["float"] = OTHER;
		km["for"] = BRANCH_PATH;
		km["goto"] = GOTO;
		km["if"] = BRANCH_PATH;
		km["int"] = OTHER;
		km["long"] = OTHER;
		km["register"] = OTHER;
		km["return"] = OTHER;
		km["short"] = OTHER;
		km["signed"] = OTHER;
		km["sizeof"] = OTHER;
		km["static"] = OTHER;
		km["struct"] = OTHER;
		km["switch"] = OTHER;
		km["typedef"] = OTHER;
		km["union"] = OTHER;
		km["unsigned"] = OTHER;
		km["void"] = OTHER;
		km["volatile"] = OTHER;
		km["while"] = BRANCH_PATH;
	}

	enum IdentifierType identifier_type(const std::string &s) {
		KeywordMap::const_iterator f = km.find(s);
		if (f == km.end())
			return IDENTIFIER;
		else
			return f->second;
	}
};
#endif /* CH */
