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

#include "errno.h"
#include "unistd.h"

#include "CMetricsCalculator.h"

static bool output_endl = true;
static bool output_filename = false;
static bool indentation_list = false;

// Process and print the metrics of stdin
static void
process_metrics(const char *filename)
{
	CMetricsCalculator cm;

	if (indentation_list)
		cm.enable_indentation_list();
	cm.calculate_metrics();
	std::cout << cm.get_metrics();
	if (output_filename)
		std::cout << '\t' << filename;
	if (output_endl)
		std::cout << std::endl;
}

/* Calculate and print C metrics for the standard input */
int
main(int argc, char * const argv[])
{
	std::ifstream in;
	int opt;

	while ((opt = getopt(argc, argv, "ain")) != -1)
		switch (opt) {
		case 'a':
			output_filename = true;
			break;
		case 'i':
			indentation_list = true;
			break;
		case 'n':
			output_endl = false;
			break;
		default: /* ? */
			std::cerr << "Usage: " << argv[0] <<
				" [-ain] [file ...]" << std::endl;
			exit(EXIT_FAILURE);
		}

	if (!argv[optind]) {
		process_metrics("-");
		exit(EXIT_SUCCESS);
	}

	// Read from file, if specified
	while (argv[optind]) {
		in.open(argv[optind], std::ifstream::in);
		if (!in.good()) {
			std::cerr << "Unable to open " << argv[optind] <<
				": " << strerror(errno) << std::endl;
			exit(EXIT_FAILURE);
		}
		std::cin.rdbuf(in.rdbuf());
		process_metrics(argv[optind]);
		in.close();
		optind++;
	}

	exit(EXIT_SUCCESS);
}
