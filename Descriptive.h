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

#ifndef DESCRIPTIVE_H
#define DESCRIPTIVE_H

#include <limits>

/** Maintain simple descriptive statistics values */
template <typename T>
class Descriptive {
private:
	T sum;
	int count;
	T min, max;
public:
	Descriptive() :
		sum(0), count(0),
		min(std::numeric_limits<T>::max()),
		max(std::numeric_limits<T>::min())
	{}

	T get_sum() const {
		return (sum);
	}

	int get_count() const {
		return (count);
	}

	T get_min() const {
		return (min);
	}

	T get_max() const {
		return (max);
	}

	/** Return the artihmetic mean of the measured value */
	double get_mean() const {
		return ((double)sum / count);
	}

	void add(T v) {
		sum += v;
		count++;
		if (v > max)
			max = v;
		if (v < min)
			min = v;
	}
};
#endif /* DESCRIPTIVE_H */
