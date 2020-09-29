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

std::vector<std::string>
split (std::string s, char delim)
{
    // cred: https://stackoverflow.com/a/46931770
    std::vector<std::string> result;
    std::stringstream ss (s);
    std::string item;

    while (std::getline (ss, item, delim)) {
        result.push_back (item);
    }
    return result;
}

std::vector<img_struct_t>
get_images_from_path_vector(std::vector<std::string> file_paths)
{
    std::vector<img_struct_t> images;
    if (file_paths.size() == 0) return images;
    std::vector<std::string>::iterator it = file_paths.begin();

    while (it != file_paths.end()) { // loop until exit condition

        std::cout << std::endl << "File info:" << std::endl;
        std::cout << ' ' << *it << std::endl;

        try {
            // attempt to read the image
            cv::Mat src = cv::imread(*it);
            if (src.empty()) {
                std::cerr << "Cannot open input image: " + *it << std::endl;
                it = file_paths.erase(it);
                continue;
            }
            std::cout << "Image size is:\t\t\t" << src.cols << "x" << src.rows << std::endl;

            // create the img_struct_t
            img_struct_t image_struct = {src, *it, *it};

            images.push_back(image_struct);
            it++;
        } catch (std::string &str) {
            std::cerr << "Error: " << *it << ": " << str << std::endl;
            return images;
        } catch (cv::Exception &e) {
            std::cerr << "Error: " << *it << ": " << e.msg << std::endl;
            return images;
        }
    }
    return images;
}

int
write_to_dir(std::vector<img_struct_t> image_vector, std::string output_dir, std::string file_type)
{
    // std::vector<int> compression_params;
    // compression_params.push_back(cv::IMWRITE_PNG_COMPRESSION);
    // compression_params.push_back(100);
    for (img_struct_t image_struct : image_vector) {
        try {
            std::string dst_file = output_dir + "/" + image_struct.file_name;
            std::cout << "Writing " << dst_file << std::endl;
            if (!cv::imwrite(dst_file, image_struct.image)) {
                throw std::runtime_error("Could not write file " + dst_file);
            }
            cv::waitKey(100);
            std::cout << "Wrote " << dst_file << std::endl;
        } catch (std::string &str) {
            std::cerr << "Error: " << str << std::endl;
            return -1;
        } catch (cv::Exception &e) {
            std::cerr << "Error: " << e.msg << std::endl;
            return -1;
        } catch (std::runtime_error &re) {
            std::cerr << "Error: " << re.what() << std::endl;
        }
    }
    return 0;
}
