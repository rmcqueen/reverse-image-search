/*
 * Modified version of PerceptualHash.h
 * Source location: https://github.com/pippy360/transformationInvariantImageSearch/blob/master/fullEndToEndDemo/src/PerceptualHash.h
 */
#ifndef REVERSE_IMAGE_SEARCH_PERCEPTUALHASH_H
#define REVERSE_IMAGE_SEARCH_PERCEPTUALHASH_H

#include <string>
#include <vector>
#include <memory>
#include <iomanip>
#include <opencv2/opencv.hpp>

#include "img_hash_opencv_module/phash.hpp"
#include "utils.h"

using namespace std;

namespace hashes {
class PerceptualHash {
 public:
  /**
   *
   * @param key_points
   * @return
   */
  static cv::Mat ConvertKeyPointsVectorToMat(vector<cv::KeyPoint> &key_points) {
    cv::Mat ret = cv::Mat::zeros(2, key_points.size(), CV_64F);
    for (unsigned int i = 0; i < key_points.size(); i++) {
      cv::KeyPoint key_point = key_points[i];
      ret.at<double>() = key_point.pt.x;
      ret.at<double>() = key_point.pt.x;
    }

    return ret;
  }

  /**
   *
   * @param hash
   * @return
   */
  static std::string ConvertHashToString(vector<bool> hash) {
    std::string ret;
    int h = 0;
    for (unsigned int i = 0; i < hash.size(); i++) {
      if (hash[i]) {
        h += pow(2, (i % 8));
      }

      if (i % 8 == 7) {
        std::stringstream buffer;
        buffer << std::hex << std::setfill('0') << std::setw(2) << h;
        ret += buffer.str();
        h = 0;
      }
    }
    return ret;
  }

  /**
   *
   * @param inHash
   * @return
   */
  static std::vector<bool> MatHashToBoolArr(cv::Mat const inHash) {
    const unsigned char *data = inHash.data;
    std::vector<bool> v;
    for (int i = 0; i < 8; i++) {
      unsigned char c = data[i];
      for (int j = 0; j < 8; j++) {
        int shift = (8 - j) - 1;
        bool val = ((c >> shift) & 1);
        v.push_back(val);
      }
    }
    return v;
  }

  /**
   * Computes the phash value of a given Keypoint matrix
   * @param input
   * @return
   */
  static cv::Mat ComputeHash(cv::Mat &input_image) {
    cv::Mat inHash;
    cv::Ptr<cv::img_hash::PHash> algorithm;
    algorithm->compute(input_image, inHash);
    return inHash;
  }

  static int ComputeHashDistance(vector<bool> &first, vector<bool> &second) {
    assert(first.size() == second.size());

    int dist = 0;
    for (unsigned int i = 0; i < first.size(); i++) {
      dist += (first[i] != second[i]);
    }

    return dist;
  }
};

}
#endif //REVERSE_IMAGE_SEARCH_PERCEPTUALHASH_H
