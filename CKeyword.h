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
		WHILE,
		IF,
		FOR,
		DEFAULT,
		CASE,
		GOTO,
		TYPEDEF,
		ELIF,
		IFDEF,
		INCLUDE,
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
		km["case"] = CASE;
		km["char"] = OTHER;
		km["const"] = OTHER;
		km["continue"] = OTHER;
		km["default"] = DEFAULT;
		km["do"] = OTHER;
		km["double"] = OTHER;
		km["elif"] = ELIF;		// Preprocessor only
		km["else"] = OTHER;
		km["enum"] = OTHER;
		km["extern"] = OTHER;
		km["float"] = OTHER;
		km["for"] = FOR;
		km["goto"] = GOTO;
		km["if"] = IF;
		km["ifdef"] = IFDEF;		// Preprocessor only
		km["include"] = INCLUDE;	// Preprocessor only
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
		km["typedef"] = TYPEDEF;
		km["union"] = OTHER;
		km["unsigned"] = OTHER;
		km["void"] = OTHER;
		km["volatile"] = OTHER;
		km["while"] = WHILE;
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
