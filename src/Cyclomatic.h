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

#ifndef CYCLOMATIC_H
#define CYCLOMATIC_H

/** Measure cyclomatic complexity */
class Cyclomatic {
private:
	int paths;
	int boolean_branches;
public:
	Cyclomatic() { reset(); }

	void reset() {
		paths = 1;
		boolean_branches = 0;
	}

	/** Called to add a path established by a predicate, e.g. if, while */
	void add_path() { paths++; }

	/** Called to add paths established by short-circuit Boolean operators */
	void add_boolean_branch() { boolean_branches++; }

	/** Return number of paths through the code */
	int complexity() const { return paths; }

	/** Return number of paths, taking into account Boolean operators */
	int extended_complexity() const { return paths + boolean_branches; }
};
#endif /* CYCLOMATIC_H */
