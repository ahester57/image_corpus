// dir_func.hpp : Directory functions
// Austin Hester CS542o sept 2020
// g++.exe (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0

#include "./include/dir_func.hpp"


// Wrapper for open_dir, returns vector of strings (filenames)
std::vector<std::string>
open_dir(const char* dir_string)
{
    std::cout << "Selected input directory:\t" << dir_string << std::endl;
    std::vector<std::string> file_paths;
    open_dir(dir_string, " \n", &file_paths);
    return file_paths;
}

// recursively open all directories and assemble a list of filenames
void
open_dir(const std::string dir_string, const std::string line_prefix, std::vector<std::string>* file_paths)
{
    DIR *dr;
    try {
        dr = opendir(dir_string.c_str()); //open all directories
    } catch (...) {
        std::cerr << "Failed to open directory: [" << dir_string << "]" << std::endl;
        return;
    }
    if (!dr) {
        std::cerr << "Failed to open directory: [" << dir_string << "]" << std::endl;
        return;
    }
    bool is_dir = false;
    struct dirent *drnt;
    while ((drnt = readdir(dr)) != NULL) {
        if (strcmp(drnt->d_name, ".") == 0 || strcmp(drnt->d_name, "..") == 0) {
            continue;
        }
        // get the relative path
        const std::string rel_path = dir_string + (dir_string.back() == '/' ? "" : std::string("/")) + std::string(drnt->d_name);
        // check if is a directory
        is_dir = is_directory(rel_path.c_str());
        // print files in an organized manner
        std::cout << line_prefix << drnt->d_name;
        if (is_dir) {
            // open next directory
            open_dir(rel_path.c_str(), line_prefix + "\t", file_paths);
        } else {
            (*file_paths).push_back(rel_path);
        }
    }
    closedir(dr); //close the directory
}

// is the given path a directory?
int
is_directory(const char* path)
{
    // cred: https://stackoverflow.com/a/146938
    struct stat s;
    if( stat(path, &s) == 0 )
    {
        if( s.st_mode & S_IFDIR )
        {
            return true;
        }
    }
    return false;
}
