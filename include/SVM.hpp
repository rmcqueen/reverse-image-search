#pragma once
#ifndef REVERSE_IMAGE_SEARCH_SVM_H
#define REVERSE_IMAGE_SEARCH_SVM_H

#include <opencv2/ml.hpp>
#include <opencv2/core.hpp>

cv::Mat ReadSVMTrainingDataFromDisk(std::string &file_path);

void WriteSVMTrainingDataToDisk(std::string &file_path, cv::Mat &training_data);

void TrainSVM(const std::string &class_file_path, int response_cols, int response_type, cv::Ptr<cv::ml::SVM> &svm);

std::string TestSVM(cv::Mat &test_img, cv::Ptr<cv::ml::SVM> &svm);

#endif //REVERSE_IMAGE_SEARCH_SVM_H
