//
// Created by marcel on 12/21/17.
//

#include <iostream>
#include "../cmdparser.hpp"

int main(int argc, char**argv)
{
	cli::Parser parser(argc, argv);
	parser.disable_help();
	const auto value = parser.run(std::cout, std::cerr);

	return 0;
}
