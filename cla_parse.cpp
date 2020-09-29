// cla_parse.cpp : Parse given command line arguments.
// Austin Hester CS542o sept 2020
// g++.exe (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0

#include "./include/cla_parse.hpp"


// parse command line arguments
int
parse_arguments(int argc, const char** argv, std::string* input_dir_path, uint* rows, uint* cols)
{
    cv::String keys =
        "{@directory  |<none>| input directory}"             // input directory is the first argument (positional)
        "{rows r      |1080  | --rows=1080}"                 // optional, default value 1080
        "{cols c      |1920  | --cols=1920}"                 // optional, default value 1920
        "{help h      |      | show help message}";          // optional, show help optional

    cv::CommandLineParser parser(argc, argv, keys);

    if (parser.has("h")) {
        parser.printMessage();
        return 0;
    }

    try {
        *input_dir_path = (std::string) parser.get<std::string>(0).c_str();
    } catch(...) {
        std::cerr << "Failed to parse directory argument!:" << std::endl;
        return -1;
    }

    try {
        *rows = (uint) parser.get<uint>("r") ? parser.get<uint>("r") : 1080;
        *cols = (uint) parser.get<uint>("c") ? parser.get<uint>("c") : 1920;
    } catch(...) {
        std::cerr << "Failed to parse size argument." << std::endl;
        return -1;
    }

    if (!parser.check()) {
        parser.printErrors();
        return -1;
    }

    if (*rows == 0 || *cols == 0) {
        std::cerr << "Dimensions cannot be zero." << std::endl;
        return -1;
    }
    if (*rows > 1080 || *cols > 1920) {
        std::cerr << "Dimensions too large. Max 1920x1080" << std::endl;
        return -1;
    }
    return 1;
}
