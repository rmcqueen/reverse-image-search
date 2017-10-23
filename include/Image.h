/*
 * Modified version of ShapeAndPositionInvariantImage.h
 * Source location: https://github.com/pippy360/transformationInvariantImageSearch/blob/master/fullEndToEndDemo/src/ShapeAndPositionInvariantImage.h
*/

#pragma once
#ifndef REVERSE_IMAGE_SEARCH_IMAGE_H
#define REVERSE_IMAGE_SEARCH_IMAGE_H

#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

#include "KeyPoint.h"

using namespace std;

class Image {
 public:
  Image(string imageName,
        cv::Mat imageData,
        string imageFullPath);

  cv::Mat get_image_data() const {
    return image_data_;
  }

  const string &get_full_image_path() const {
    return image_full_path_;
  }

 private:
  const string image_name_;
  const cv::Mat image_data_;
  const string image_full_path_;
};

inline Image::Image(const string image_name,
                    const cv::Mat image_data,
                    const string image_full_path) : image_name_(image_name),
                                                  image_data_(image_data),
                                                   image_full_path_(image_full_path) {}

#endif //REVERSE_IMAGE_SEARCH_IMAGE_H
