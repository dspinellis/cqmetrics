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

#ifndef CMETRICSCALCULATOR_H
#define CMETRICSCALCULATOR_H

#include <iostream>

#include "CharSource.h"
#include "BolState.h"
#include "QualityMetrics.h"

/** Collect quality metrics from C-like source code */
class CMetricsCalculator {
private:
	CharSource src;
	QualityMetrics qm;
	BolState bol;
	int top_level_depth;		// 0 for C, 1 for Java
	int current_depth;		// Keeps track of { }
	bool scan_cpp_directive;	// After a C preprocessor #
	void calculate_metrics_loop();
	bool calculate_metrics_switch();
	bool in_function;		// True when scanning functions
	CKeyword ckeyword;
public:
	CMetricsCalculator(std::istream &s = std::cin) : src(s),
	top_level_depth(0), current_depth(0), in_function(false),
	scan_cpp_directive(false) {}
	void calculate_metrics() {
		calculate_metrics_loop();
		qm.set_nchar(src.get_nchar());
	}
	const QualityMetrics& get_metrics() const { return qm; }
};
#endif /* CMETRICSCALCULATOR_H */
