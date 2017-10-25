#pragma once
#ifndef REVERSE_IMAGE_SEARCH_IMAGE_H
#define REVERSE_IMAGE_SEARCH_IMAGE_H

#include <string>
#include <vector>
#include <opencv2/opencv.hpp>


using namespace std;

class Image {
 public:
  Image(string &image_name,
        cv::Mat &image_data,
        string &image_full_path);

  cv::Mat get_image_data() {
    return image_data_;
  }

  string get_full_image_path() {
    return image_full_path_;
  }

 private:
  const string image_name_;
  const cv::Mat image_data_;
  const string image_full_path_;
};

inline Image::Image(
    string &image_name,
    cv::Mat &image_data,
    string &image_full_path) :
    image_name_(image_name),
    image_data_(image_data),
    image_full_path_(image_full_path) {}

#endif //REVERSE_IMAGE_SEARCH_IMAGE_H
