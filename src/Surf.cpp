/**
 * Surf.cpp
 *
 * This class provides key point, and feature vector extraction from an image using the SURF algorithm in a convenient
 * way. Essentially reducing the required steps to get the provided methods
 */
#include <iostream>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/highgui.hpp>

#include "utils.hpp"

using namespace std;

/**
 * Gets the key points for a given image
 * @param input_image cv::Mat a matrix of the image's data
 * @return vector<cv::KeyPoint> a matrix of the key points within the given image
 */
vector<cv::KeyPoint> get_key_points(cv::Mat &input_image) {
  int minHessian = 400; // Modify this as needed
  cv::Ptr<cv::xfeatures2d::SURF> detector = cv::xfeatures2d::SURF::create(minHessian);
  vector<cv::KeyPoint> key_points;
  detector->detect(input_image, key_points);
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
  cv::Ptr<cv::FeatureDetector> detector = cv::xfeatures2d::SURF::create(minHessian);

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

  if (!image.data) {
    return cv::Mat(0, 0, CV_64F);
  }

  int minHessian = 400;  // Modify this as needed
  cv::Ptr<cv::DescriptorExtractor> extractor = cv::xfeatures2d::SURF::create(minHessian);
  // Get key points
  vector<cv::KeyPoint> key_points;
  cv::Mat descriptors;

  extractor->detectAndCompute(image, cv::Mat(), key_points, descriptors);

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
vector<cv::Mat> get_multiple_feature_vectors(vector<string> &file_names) {
  vector<cv::Mat> descriptors;
  cout << "Computing feature vectors..." << endl;
  for (string &file : file_names) {
    string file_label = utils::Utility::get_image_label(file);
    cv::Mat desc = get_single_feature_vector(file);
    if (desc.rows < 1) {
      continue;
    }

    descriptors.push_back(desc);
  }
  return descriptors;
}

/**
 * Given a vector<cv::Mat>, combine all of the elements into one singular matrix in order to create an index when searching
 * for an image match
 * @param descriptors
 * @return
 */
cv::Mat ConcatenateDescriptors(vector<cv::Mat> &descriptors) {

  // TODO: Could probably pre-allocate the size of the matrix based on the dimensions labelled in descriptors
  cv::Mat concatenated_descriptors;

  for (cv::Mat descriptor : descriptors) {
    concatenated_descriptors.push_back(descriptor);
  }

return concatenated_descriptors;
}
