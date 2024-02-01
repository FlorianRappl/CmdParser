/*
  This file is part of the C++ CmdParser utility.
  Copyright (c) 2015 - 2016 Florian Rappl
*/

#include "catch.hpp"
#include <sstream>
#include "../cmdparser.hpp"

using namespace cli;

TEST_CASE( "Parse help", "[help]" ) {
	std::stringstream output { };
	std::stringstream errors { };

	const char* args[2] = {
		"myapp",
		"--help"
	};

	Parser parser(2, args);
	const auto value = parser.run(output, errors);
	const std::string prefix = "Available parameters:";

	REQUIRE(parser.has_help() == true);
	REQUIRE(parser.app_name() == "myapp");
	REQUIRE(value == false);
	REQUIRE(output.str().substr(0, prefix.size()) == prefix);
}

TEST_CASE( "No help", "[help]" ) {
	std::stringstream output { };
	std::stringstream errors { };

	const char* args[2] = {
		"myapp",
		"--help"
	};

	Parser parser(2, args);
	parser.disable_help();
	const auto value = parser.run(output, errors);
	const std::string prefix = "Available parameters:";

	REQUIRE(parser.has_help() == false);
	REQUIRE(parser.app_name() == "myapp");
	REQUIRE(value == false);
	REQUIRE(output.str().substr(0, prefix.size()) != prefix);
}

TEST_CASE( "Custom help", "[help]" ) {
	std::stringstream output { };
	std::stringstream errors { };

	const std::string prefix = "Test";
	const char* args[2] = {
		"myapp",
		"--help"
	};

	Parser parser(2, args);
	parser.disable_help();
	parser.set_callback("h", "help", std::function<bool(CallbackArgs&)>([prefix](CallbackArgs& args){
		args.output << "Test";
		throw std::bad_cast();
		return false;
	}));
	const auto value = parser.run(output, errors);

	REQUIRE(parser.has_help() == true);
	REQUIRE(parser.app_name() == "myapp");
	REQUIRE(value == false);
	REQUIRE(output.str().substr(0, prefix.size()) == prefix);
}

TEST_CASE( "Parse nothing", "[nothing]" ) {
	std::stringstream output { };
	std::stringstream errors { };

	const char* args[1] = {
		"myapp"
	};

	Parser parser(1, args);
	const auto value = parser.run(output, errors);

	REQUIRE(value == true);
	REQUIRE(parser.app_name() == "myapp");
}

TEST_CASE( "Parse required bool provided", "[v]" ) {
	std::stringstream output { };
	std::stringstream errors { };

	const char* args[2] = {
		"myapp",
		"-v"
	};

	Parser parser(2, args);
	parser.set_optional<bool>("v", "verbose", false);
	const auto value = parser.run(output, errors);

	REQUIRE(value == true);
	REQUIRE(parser.app_name() == "myapp");

	const auto ret = parser.get<bool>("v");

	REQUIRE(ret == true);
}

TEST_CASE( "Parse required bool not provided", "[required] [bool] [missing]" ) {
	std::stringstream output { };
	std::stringstream errors { };

	const char* args[1] = {
		"myapp"
	};

	Parser parser(1, args);
	parser.set_optional<bool>("v", "verbose", false);
	const auto value = parser.run(output, errors);

	REQUIRE(value == true);
	REQUIRE(parser.app_name() == "myapp");

	const auto ret = parser.get<bool>("v");

	REQUIRE(ret == false);
}

TEST_CASE( "Parse required bool provided but inverted", "[required] [bool] [available]" ) {
	std::stringstream output { };
	std::stringstream errors { };

	const char* args[2] = {
		"myapp",
		"-v"
	};

	Parser parser(2, args);
	parser.set_optional<bool>("v", "verbose", true);
	const auto value = parser.run(output, errors);

	REQUIRE(value == true);
	REQUIRE(parser.app_name() == "myapp");

	const auto ret = parser.get<bool>("v");

	REQUIRE(ret == false);
}
TEST_CASE( "Parse default argument", "[default]") {
	std::stringstream output { };
	std::stringstream errors { };

	const char* args[2] = {
		"myapp",
		"default_arg"
	};
	Parser parser(2, args);
	parser.set_default<std::string>(true, "default argument");
	const auto value = parser.run(output, errors);
	REQUIRE(value == true);
	REQUIRE(parser.app_name() == "myapp");
	const auto ret = parser.get_default<std::string>();
	REQUIRE(ret == "default_arg");
}

TEST_CASE( "Parse default argument with default value", "[default`] [missing]") {
	std::stringstream output { };
	std::stringstream errors { };

	const char* args[1] = {
		"myapp"
	};
	Parser parser(1, args);
	parser.set_default<std::string>(false, "Optional default argument", "default_value");
	const auto value = parser.run(output, errors);
	REQUIRE(value == true);
	REQUIRE(parser.app_name() == "myapp");
	const auto ret = parser.get_default<std::string>();
	REQUIRE(ret == "default_value");
}
TEST_CASE( "Parse required bool", "[required] [bool] [missing]" ) {
	std::stringstream output { };
	std::stringstream errors { };

	const char* args[1] = {
		"myapp"
	};

	Parser parser(1, args);
	parser.set_optional<bool>("v", "verbose", true);
	const auto value = parser.run(output, errors);

	REQUIRE(value == true);
	REQUIRE(parser.app_name() == "myapp");

	const auto ret = parser.get<bool>("v");

	REQUIRE(ret == true);
}

TEST_CASE( "Parse required integer", "[required] [integer] [available]" ) {
	std::stringstream output { };
	std::stringstream errors { };

	const char* args[3] = {
		"myapp",
		"-n",
		"42"
	};

	Parser parser(3, args);
	parser.set_required<int>("n", "number");
	const auto value = parser.run(output, errors);

	REQUIRE(value == true);
	REQUIRE(parser.app_name() == "myapp");

	const auto ret = parser.get<int>("n");

	REQUIRE(ret == 42);
}

TEST_CASE( "Parse required integer not provided", "[required] [integer] [missing]" ) {
	std::stringstream output { };
	std::stringstream errors { };

	const char* args[2] = {
		"myapp",
		"-n"
	};

	Parser parser(2, args);
	parser.set_required<int>("n", "number");
	const auto value = parser.run(output, errors);

	REQUIRE(parser.app_name() == "myapp");
	REQUIRE(value == false);
}

TEST_CASE( "Parse required double provided", "[required] [double] [available]" ) {
	std::stringstream output { };
	std::stringstream errors { };

	const char* args[3] = {
		"myapp",
		"--delta",
		"3.5"
	};

	Parser parser(3, args);
	parser.set_required<double>("d", "delta");
	const auto value = parser.run(output, errors);

	REQUIRE(parser.app_name() == "myapp");
	REQUIRE(value == true);

	const auto ret = parser.get<double>("d");

	REQUIRE(ret == 3.5);
}

TEST_CASE( "Parse required string provided", "[required] [string] [available]" ) {
	std::stringstream output { };
	std::stringstream errors { };

	const char* args[3] = {
		"myapp",
		"-n",
		"bert"
	};

	Parser parser(3, args);
	parser.set_required<std::string>("n", "name");
	const auto value = parser.run(output, errors);

	REQUIRE(parser.app_name() == "myapp");
	REQUIRE(value == true);

	const auto ret = parser.get<std::string>("n");

	REQUIRE(ret == "bert");
}

TEST_CASE( "Parse non required string not provided", "[required] [string] [missing]" ) {
	std::stringstream output { };
	std::stringstream errors { };

	const char* args[1] = {
		"myapp"
	};

	Parser parser(1, args);
	parser.set_optional<std::string>("n", "name", "hans");
	const auto value = parser.run(output, errors);

	REQUIRE(parser.app_name() == "myapp");
	REQUIRE(value == true);

	const auto ret = parser.get<std::string>("n");

	REQUIRE(ret == "hans");
}

TEST_CASE( "Parse not found parameter", "[parameter] [missing]" ) {
	std::stringstream output { };
	std::stringstream errors { };

	const char* args[2] = {
		"myapp",
		"-o"
	};

	Parser parser(2, args);
	const auto value = parser.run(output, errors);

	REQUIRE(parser.app_name() == "myapp");
	REQUIRE(value == false);
}

TEST_CASE( "Parse found parameter but empty", "[parameter] [empty]" ) {
	std::stringstream output { };
	std::stringstream errors { };

	const char* args[2] = {
		"myapp",
		"-o"
	};

	Parser parser(2, args);
	parser.set_optional<std::string>("o", "output", "hans");
	const auto value = parser.run(output, errors);

	REQUIRE(parser.app_name() == "myapp");
	REQUIRE(value == false);

	const auto ret = parser.get<std::string>("o");

	REQUIRE(ret == "hans");
}

TEST_CASE( "Parse list of integers with 3 values", "[integers] [available]" ) {
	std::stringstream output { };
	std::stringstream errors { };

	const char* args[5] = {
		"myapp",
		"--values",
		"0",
		"1",
		"2"
	};

	Parser parser(5, args);
	parser.set_required<std::vector<int>>("v", "values");
	const auto value = parser.run(output, errors);

	REQUIRE(parser.app_name() == "myapp");
	REQUIRE(value == true);

	const auto ret = parser.get<std::vector<int>>("v");

	REQUIRE(ret.size() == 3u);
	REQUIRE(ret[0] == 0);
	REQUIRE(ret[1] == 1);
	REQUIRE(ret[2] == 2);
}

TEST_CASE( "Parse list of integers with 4 values, but partially double", "[v]" ) {
	std::stringstream output { };
	std::stringstream errors { };

	const char* args[6] = {
		"myapp",
		"-v",
		"0.3",
		"1.7",
		"2",
		"-5e2"
	};

	Parser parser(6, args);
	parser.set_required<std::vector<double>>("v", "values");
	const auto value = parser.run(output, errors);

	REQUIRE(parser.app_name() == "myapp");
	REQUIRE(value == true);

	const auto ret = parser.get<std::vector<double>>("v");

	REQUIRE(ret.size() == 4u);
	REQUIRE(ret[0] == 0.3);
	REQUIRE(ret[1] == 1.7);
	REQUIRE(ret[2] == 2.0);
	REQUIRE(ret[3] == -5e2);
}

TEST_CASE( "Parse non-required integer with get if", "[n]" ) {
	std::stringstream output { };
	std::stringstream errors { };

	const char* args[3] = {
		"myapp",
		"-n",
		"42"
	};

	Parser parser(3, args);
	parser.set_required<int>("n", "number");
	const auto value = parser.run(output, errors);

	REQUIRE(value == true);
	REQUIRE(parser.app_name() == "myapp");

	SECTION("constrain value between (0, 10) otherwise -1 should be -1") {
		const auto ret = parser.get_if<int>("n", [&](int value) { 
			return value > 0 && value < 10 ? value : -1; 
		});
		REQUIRE(ret == -1);
	}

	SECTION("constrain value between (0, 10) otherwise value should be value") {
		const auto ret = parser.get_if<int>("n", [&](int value) { 
			return value > 0 && value < 10 ? -1 : value; 
		});
		REQUIRE(ret == 42);
	}
}
