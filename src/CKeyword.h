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

#ifndef CKEYWORD_H
#define CKEYWORD_H

#include <map>
#include <string>

/** Classify identifiers into C keywords */
class CKeyword {
public:
	enum IdentifierType {
		BREAK,
		CASE,
		CONST,
		CONTINUE,
		DEFAULT,
		DO,
		ELIF,
		ELSE,
		ENUM,
		FOR,
		GOTO,
		IF,
		IFDEF,
		INCLUDE,
		INLINE,
		NOALIAS,
		REGISTER,
		RESTRICT,
		RETURN,
		SIGNED,
		STATIC,
		STRUCT,
		SWITCH,
		TYPEDEF,
		UNION,
		UNSIGNED,
		VOID,
		VOLATILE,
		WHILE,
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
		km["break"] = BREAK;
		km["case"] = CASE;
		km["char"] = OTHER;
		km["const"] = CONST;
		km["continue"] = CONTINUE;
		km["default"] = DEFAULT;
		km["do"] = DO;
		km["double"] = OTHER;
		km["elif"] = ELIF;		// Preprocessor only
		km["else"] = ELSE;
		km["enum"] = ENUM;
		km["extern"] = OTHER;
		km["float"] = OTHER;
		km["for"] = FOR;
		km["goto"] = GOTO;
		km["if"] = IF;
		km["ifdef"] = IFDEF;		// Preprocessor only
		km["include"] = INCLUDE;	// Preprocessor only
		km["inline"] = INLINE;
		km["int"] = OTHER;
		km["long"] = OTHER;
		km["noalias"] = NOALIAS;
		km["register"] = REGISTER;
		km["restrict"] = RESTRICT;
		km["return"] = RETURN;
		km["short"] = OTHER;
		km["signed"] = SIGNED;
		km["sizeof"] = OTHER;
		km["static"] = STATIC;
		km["struct"] = STRUCT;
		km["switch"] = SWITCH;
		km["typedef"] = TYPEDEF;
		km["union"] = UNION;
		km["unsigned"] = UNSIGNED;
		km["void"] = VOID;
		km["volatile"] = VOLATILE;
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
#endif /* CKEYWORD_H */
