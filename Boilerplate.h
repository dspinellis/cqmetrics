/*-
 * Copyright 2015 Diomidis Spinellis
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

#ifndef BOILERPLATE_H
#define BOILERPLATE_H

#include <string>

/**
 * Detect boiplerplate comments
 * Those are comments containing the word "Copyright" or the word
 * "WARRANTIES".
 */
class Boilerplate {
private:
	std::string word;
	bool found_boilerplate;
	int nchar;
public:
	Boilerplate() : found_boilerplate(false), nchar(0) {}

	/** Process a single comment character */
	inline void process_char(char c) {
		nchar++;
		if (found_boilerplate)
			return;

		if (isalpha(c))
			word += c;
		else {
			if (word == "Copyright" || word == "WARRANTIES")
				found_boilerplate = true;
			word.erase();
		}
	}

	/** Return number of characters in boilerplate comment */
	int get_boilerplate_chars() const {
		return found_boilerplate ? nchar : 0;
	}

	/** Reset state to that at the beginning of a new comment */
	void begin_comment() {
		word.erase();
		found_boilerplate = false;
		nchar = 0;
	}
};
#endif /* BOILERPLATE_H */
