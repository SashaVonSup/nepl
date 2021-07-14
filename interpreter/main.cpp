#include <iostream>
#include <fstream>
#include <boost/program_options.hpp>

#include "Lexer.h"

namespace po = boost::program_options;

int main(int argc, char *argv[]) {
    po::options_description desc;
    desc.add_options()
            ("help,h", "Show help")
            ("source,s", po::value<std::string>(), "Source code filename");
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << '\n';
        return EXIT_SUCCESS;
    }

    if (vm.count("source")) {
        std::ifstream source(vm["source"].as<std::string>());
        if (!source) {
            std::cerr << "File \"" << vm["source"].as<std::string>() << "\" not available!\n";
            return EXIT_FAILURE;
        }

        nepl::Lexer lexer(source);

        std::cout << "Found tokens:\n";
        while (lexer.source)
            std::cout << lexer.nextToken() << '\n';
    } else {
        std::cerr << "No input files specified. Use --help or -h to see help.\n";
    }

    return EXIT_SUCCESS;
}
