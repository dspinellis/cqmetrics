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

#ifndef HALSTEAD_H
#define HALSTEAD_H

#include <set>
#include <string>
#include <cmath>

/** Maintain and calculate Halstead complexity */
class Halstead {
private:
	/* Unique operators and operands */
	std::set<std::string> unique;

	/* All operators and operands */
	int total;
public:
	Halstead() { reset(); }

	/** Reset tracking */
	void reset() {
		unique.clear();
		total = 0;
	}

	/** Add an operator or operand */
	void add(const std::string &s) {
		unique.insert(s);
		total++;
	}

	/** Return the Halstead complexity */
	double complexity() const {
		if (total == 0)
			return 0;
		else
			return total * log(unique.size()) / log(2);
	}
};
#endif /* HALSTEAD_H */
