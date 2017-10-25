/**
 * This class provides key point, and feature vector extraction from an image using the SURF algorithm in a convenient
 * way. Essentially reducing the required steps to get the provided methods
 */
#include <iostream>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/highgui.hpp>
#include <IndicesMapping.h>

#include "utils.h"
using namespace std;

/**
 * Gets the key points for a given image
 * @param input_image cv::Mat a matrix of the image's data
 * @return vector<cv::KeyPoint> a matrix of the key points within the given image
 */
vector<cv::KeyPoint> get_key_points(cv::Mat &input_image) {
  int minHessian = 400; // Modify this as needed
  cv::Ptr<cv::xfeatures2d::SURF> detector = cv::xfeatures2d::SURF::create();
  detector->setHessianThreshold(minHessian);
  vector<cv::KeyPoint> key_points;

  return key_points;
}

/**
 * Compute a feature vector for an image given the image, and its key points
 * @param image cv::Mat a matrix of the image's data
 * @param key_points vector<cv::KeyPoint> a matrix of the key points within the given image
 * @return cv::Mat a feature vector in the form of a matrix
 */
cv::Mat get_single_feature_vector(cv::Mat &image, vector<cv::KeyPoint> &key_points) {
  int minHessian = 400; // Modify this as needed
  cv::Ptr<cv::xfeatures2d::SURF> detector = cv::xfeatures2d::SURF::create();
  detector->setHessianThreshold(minHessian);

  cv::Mat descriptors;
  detector->compute(image, key_points, descriptors);

  return descriptors;
}

/**
 * Compute a feature vector for a single file
 * @param file_name std::string the file name of the image
 * @return cv::Mat a feature vector in the form of a matrix
 */
cv::Mat get_single_feature_vector(string &file_name) {
  // Read in file
  cv::Mat image = cv::imread(file_name);

  int minHessian = 400;  // Modify this as needed
  cv::Ptr<cv::xfeatures2d::SURF> detector = cv::xfeatures2d::SURF::create();
  detector->setHessianThreshold(minHessian);

  // Get key points
  vector<cv::KeyPoint> key_points;
  detector->detect(image, key_points);

  // Get descriptors (feature vectors)
  cv::Mat descriptors;
  detector->compute(image, key_points, descriptors);

  return descriptors;
}

/**
 * Given a directory of images, compute a feature vector for every image within that directory, and store information
 * such as the file name, and range of rows within the master feature vector matrix to later return the highest similarity
 * images
 * @param file_names vector<string> contains each file name within a directory
 * @param indices_mapping vector<IndicesMapping> a matrix composed of relevant IndicesMapping objects for each image
 * @return vector<cv::Mat> a vector composed of each image's feature vector within a directory (vector of image matrices)
 */
vector<cv::Mat> get_multiple_feature_vectors(vector<string> file_names, vector<IndicesMapping> &indices_mapping) {
  vector<cv::Mat> descriptors;
  int idx_row = 0;

  for (int i = 0; i < file_names.size(); i++) {
    auto desc = get_single_feature_vector(file_names[i]);
    descriptors.push_back(desc);
    IndicesMapping mapping = IndicesMapping(file_names[i], idx_row, idx_row + desc.rows - 1);
    indices_mapping.push_back(IndicesMapping(mapping));
    idx_row += desc.rows;
  }
  return descriptors;
}
