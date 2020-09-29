// dir_func.hpp : Directory functions
// Austin Hester CS542o sept 2020
// g++.exe (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0

#ifndef _H_DIR_FUNC
#define _H_DIR_FUNC

#include <sys/stat.h>
#include <vector>
#include <iostream>
#include <dirent.h>

std::vector<std::string> open_dir(const char* dir_string);
void open_dir(const std::string dir_string, const std::string line_prefix, std::vector<std::string>* file_paths);
int is_directory(const char* path);

#endif
