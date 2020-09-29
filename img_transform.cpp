// img_transform.cpp : Transformation functions
// Austin Hester CS542o sept 2020
// g++.exe (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0

#include "./include/img_transform.hpp"


cv::Mat
scale_image(cv::Mat src, uint rows, uint cols, bool preserve_aspect)
{
    cv::Size new_size = cv::Size(cols, rows);
    if (preserve_aspect) {
        // calculate scaling
        uint d_rows = src.rows, d_cols = src.cols;
        float col_scale = (float) cols / src.cols;
        float row_scale = (float) rows / src.rows;

        // decide which way to scale
        if (src.cols * row_scale > cols) {
            // set columns to maximum provided
            d_cols = cols;
            d_rows = (uint) (src.rows * col_scale);
            std::cout << "Scaled up cols:\t" << col_scale << std::endl;
        } else {
            // set rows to maximum provided
            d_rows = rows;
            d_cols = (uint) (src.cols * row_scale);
            std::cout << "Scaled up rows:\t" << row_scale << std::endl;
        }
        new_size = cv::Size(d_cols, d_rows);
    }
    std::cout << "New image size is:\t" << new_size.width << "x" << new_size.height << std::endl;
            
    cv::Mat dst = cv::Mat::zeros(new_size, CV_64FC1);
    resize(src, dst, dst.size(), 0, 0, cv::INTER_NEAREST);
    return dst;
}
