// corpus.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Austin Hester CS542o sept 2020
// g++.exe (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0

#include "./include/corpus.hpp"
#include "./include/cla_parse.hpp"
#include "./include/dir_func.hpp"
#include "./include/img_display.hpp"
#include "./include/img_transform.hpp"


int
main(int argc, const char** argv)
{
    // CLA variables
    uint rows;
    uint cols;
    bool preserve_aspect;
    bool grayscale;
    std::string input_dir_path;
    std::string output_dir_path;
    std::string file_type;

    // parse and save command line args
    int parse_result = parse_arguments(
        argc, argv,
        &input_dir_path, &output_dir_path,
        &rows, &cols,
        &preserve_aspect, &grayscale,
        &file_type
    );
    if (parse_result != 1) return parse_result;

    // open input directory and return list of relative file paths
    std::vector<std::string> file_paths = open_dir(input_dir_path.c_str());

    // display images to screen
    // display_images(file_paths, rows, cols);
    std::vector<cv::Mat> image_vector = get_images_from_path_vector(file_paths);

    for (cv::Mat img : image_vector) {
        cv::Mat new_img = scale_image(img, rows, cols, preserve_aspect, cv::INTER_LANCZOS4);
        if (grayscale) {
            new_img = apply_grayscale(new_img);
        }
        cv::imshow("hi", new_img);
        cv::waitKey(0);
    }
	return 0;
}
