#pragma once
#ifndef REVERSE_IMAGE_SEARCH_HISTOGRAM_H
#define REVERSE_IMAGE_SEARCH_HISTOGRAM_H

#include <opencv2/core.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <string>
#include <vector>

cv::Mat ReadClassHistogramsFromDisk(const std::string &dir_path, std::string &class_type);

void WriteHistogramToDisk(std::string &file_path, cv::Mat &histogram);

cv::Mat ComputeHistogram(std::string &file_path, cv::Mat &vocabulary);

void ComputeHistogram(std::string &file_path, cv::Mat &training_data, cv::Mat &vocabulary);

void ComputeHistograms(std::vector<std::string> &images, cv::Mat &out_training_data, std::string &vocabulary_name);

#endif //REVERSE_IMAGE_SEARCH_HISTOGRAM_H
