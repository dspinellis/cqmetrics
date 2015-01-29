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

#include <cstring>
#include <fstream>
#include <iostream>
#include <ostream>

#include "unistd.h"

#include "CMetricsCalculator.h"

/* Calculate and print C metrics for the standard input */
int
main(int argc, char *argv[])
{
	CMetricsCalculator cm;
	std::ifstream in;
	bool output_endl = true;
	int opt;

	while ((opt = getopt(argc, argv, "n")) != -1)
		switch (opt) {
		case 'n':
			output_endl = false;
			break;
		default: /* ? */
			std::cerr << "Usage: " << argv[0] <<
				" [-n] [file]" << std::endl;
			exit(EXIT_FAILURE);
		}

	// Read from file, if specified
	if (argv[optind]) {
		in.open(argv[optind], std::ifstream::in);
		if (!in.good()) {
			std::cerr << "Unable to open " << argv[1] << std::endl;
			exit(EXIT_FAILURE);
		}
		std::cin.rdbuf(in.rdbuf());
	}

	cm.calculate_metrics();
	std::cout << cm.get_metrics();
	if (output_endl)
		std::cout << std::endl;
	return 0;
}
