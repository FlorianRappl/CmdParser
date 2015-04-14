# Simple C++ command line parser

This project supplies a simple, single-header, command-line parser. It is very lightweight and relies on templates. The easiest way is to use it is as a dependency via *biicode*. The parser requires C++11 and works fine on gcc (v4.8.2 or later, some earlier versions should work as well), icc (v14 or later), clang and msvc (v18 or later).

## Using the parser

Using the parser is straight forward. Only include the header (*cmdparser.h*) in your application source file, most likely the one that contains the `main` method. Pass the command line arguments to the parser:

```cpp
int main(int argc, char** argv) {
	cli::Parser parser(argc, argv);
	/* ... */
}
```

In the following two sections we'll have a look at setting up the parser and using it.

### Setup

Setting up the parser works using the following methods:

* `set_optional<T>()`, to include an optional argument
* `set_required<T>()`, to include a required argument

The third parameter for creating an optional argument is the default value. This value is used if nothing is provided by the user. Otherwise the optional and required methods are pretty similar:

1. The shorthand (if the user uses a single slash) string
2. The longhand (if the user uses two slashes) string
3. The optional description.

The third parameter is the fourth parameter for optional arguments.

Let's look at an example:

```cpp
void configure_parser(cli::Parser& parser) {
	parser.set_optional<std::string>("o", "output", "data", "Strings are naturally included.");
	parser.set_optional<int>("n", "number", 8, "Integers in all forms, e.g., unsigned int, long long, ..., are possible.");
	parser.set_optional<double>("b", "beta", 11.0, "Also floating point values are possible.");
	parser.set_optional<bool>("a", "all", false, "Boolean arguments are simply switched when encountered, i.e. false to true if provided.");
	parser.set_required<std::vector<short>>("v", "values", "By using a vector it is possible to receive a multitude of inputs.");
}
```

Usually it makes sense to pack the Parser's setup in a function. But of course this is not required. The shorthand is not limited to a single character. It could also be the same as the longhand alternative.

### Getting values

Getting values is possible via the `get` method. This is also a template. We need to specify the type of argument. This has to be the same type as defined earlier. It also has to be a valid argument (shorthand) name. At the moment only shorthands are considered here. For instance we could do the following:

```cpp
//auto will be int
auto number = parser.get<int>("n");

//auto will be std::string
auto output = parser.get<std::string>("o");

//auto will be bool
auto all = parser.get<bool>("a");

//auto will be std::vector<short>
auto values = parser.get<std::vector<short>>("v");
```

However, before we can access these values we also need to check if the provided user input was valid. On construction the `Parser` does not examine the input. The parser waits for setup and a potential call to the `run` method. The `run` method runs a boolean value to indicate if the provided command line arguments match the requirements.

What we usually want is something like:

```cpp
void parse_and_exit(cli::Parser& parser) {
	if (parser.parse() == false) {
		exit(1);
	}
}
```

Writing this function seems to be redundant. Hence the parser includes it already:

	parser.run_and_exit_if_error();

The only difference is that the `run_and_exit_if_error` method does not provide overloads for passing custom output and error streams. The `parse` method has overloads to support such scenarios. By default `std::cout` is used the regular output, e.g., the integrated help. Also `std::cerr` is used for displaying error messages.

## Integrated help

The parser comes with a pre-defined command that has the shorthand `-h` and the longhand `--help`. This is the integrated help, which appears if only a single command line argument is given, which happens to be either the shorthand or longhand form.

Finally our `main` method may look as follows:

```cpp
int main(int argc, char** argv) {
	cli::Parser parser(argc, argv);
	configure_parser(parser);
	cmd.run_and_exit_if_error();
	/* ... */
}
```

This passes the arguments to the parser, configures the parser and checks for potential errors. In case of any errors the program is exited immediately.

## Contributions

This is not a huge project and the file should remain a small, single-header command-line parser, which may be useful for small to medium projects. Nevertheless, if you find any bugs, add small, yet useful, new features or improve the cross-compiler compatibility, then contributions are more than welcome.

## License

The application is licensed under the GPLv3 license.

Copyright (C) 2015 Florian Rappl
  
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see http://www.gnu.org/licenses/.
