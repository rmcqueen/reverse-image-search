/**
 * Histogram.cpp
 *
 * This class is used in order to construct the Bag of Visual Words histogram for each image present within the data set
 * specified by the user. Includes various IO methods for reading/writing the histograms to disk to reduce the required
 * computation time if the system needs to be ran again.
 *
 * There exists no functionality to incorporate new histograms as images are added to the data set.
 */
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <boost/filesystem.hpp>

#include "utils.hpp"
#include "Surf.hpp"
#include "Vocabulary.hpp"
#include "SVM.hpp"

using namespace std;
using namespace boost::filesystem;

/**
 * Read existing histograms computed for each class present within the specified data by the user.
 * These classes are considered to be objects such as 'baseball', 'basketball', etc.
 * @param dir_path std::string the relative path to the class histogram directory (i.e, data/histograms/baseball/)
 * @param class_type std::string the name of the class to be obtained
 * @return cv::Mat containing the concatenated histograms of all items within the specified class_type
 */
cv::Mat ReadClassHistogramsFromDisk(const string &dir_path, string &class_type) {
  cv::Mat histograms;
  path dir(dir_path);
  path type(class_type);
  // Append the class type to the directory path
  path full_path = dir / type;
  recursive_directory_iterator end;

  // Iterate over each histogram in the class histogram directory and append it to a single matrix
  for (recursive_directory_iterator itr(full_path); itr != end; ++itr) {
    cv::Mat temp_hist;
    cv::FileStorage fs(itr->path().string(), cv::FileStorage::READ);
    fs[class_type] >> temp_hist;
    histograms.push_back(temp_hist);
    fs.release();
  }

  return histograms;
}

/**
 * Write the Bag of Visual Words histogram to disk for a particular image. Takes in the full path to the original image
 * and creates a yml file with the same name as the image
 * @param file_path std::string the relative file path to the original image the histogram was computed from
 * @param histogram cv::Mat the Bag of Visual Words histogram for an image. Should be a normalized Mx1 matrix.
 */
void WriteHistogramToDisk(string &file_path, cv::Mat &histogram) {
  string image_name = utils::Utility::get_file_name_from_full_path(file_path);
  image_name = image_name.substr(0, image_name.find(".jpg"));
  assert(histogram.rows == 1);

  // Get the class label associated with the image (i.e, bathtub)
  string image_label = utils::Utility::get_image_label(file_path);

  // If the directory for the histogram to write to does not exist, create it
  if (!exists("data/histograms/" + image_label)) {
    create_directory("data/histograms/" + image_label);
  }
  string full_file_path = "data/histograms/" + image_label + "/";
  full_file_path += image_name + ".yml";

  // Write the histogram to disk
  cv::FileStorage file_histogram(full_file_path, cv::FileStorage::WRITE);
  file_histogram << image_label << histogram;
  file_histogram.release();
}

/**
 * Computes the Bag of Visual Words histogram for a particular image. Used when no training data is required to be
 * added onto after computation.
 * @param file_path std:;string the relative file path to the image which is to have its histogram computed
 * @param vocabulary cv::Mat the pre-constructed Bag of Visual Words dictionary containing the words to use when
 * constructing the histogram for an image
 * @return cv::Mat the normalized histogram for an image
 */
cv::Mat ComputeHistogram(string &file_path, cv::Mat &vocabulary) {
  // Initialize the SURF extractor/detector and BoW extractor
  cv::Ptr<cv::xfeatures2d::SurfFeatureDetector> detector = cv::xfeatures2d::SURF::create();
  cv::Ptr<cv::DescriptorExtractor> extractor = cv::xfeatures2d::SURF::create();
  cv::Ptr<cv::DescriptorMatcher> matcher = cv::BFMatcher::create();
  cv::BOWImgDescriptorExtractor bow_extractor(extractor, matcher);

  bow_extractor.setVocabulary(vocabulary);

  cv::Mat temp_img = cv::imread(file_path);
  cv::Mat bow_descriptor;

  // Get the key points of the temp_img
  vector<cv::KeyPoint> key_points = get_key_points(temp_img);

  // Extract SURF descriptors for the temp_img, and then extract the Bag of Visual Words histogram for it
  extractor->detect(temp_img, key_points);
  bow_extractor.compute(temp_img, key_points, bow_descriptor);
  return bow_descriptor;
}

/**
 * Computes the Bag of Visual Words histogram for a particular image, and appends the results to a training data object.
 * Used typically during the initial construction of the system. Used as a helper function for ComputeHistograms()
 * @param file_path std::string the relative path to the file to compute a Bag of Visual Words histogram for
 * @param out_training_data cv::Mat the training object to append a histogram onto
 * @param vocabulary cv::Mat the pre-computed Bag of Visual Words dictionary containing the words to use when
 * constructing the histogram for an image
 */
void ComputeHistogram(string &file_path, cv::Mat &out_training_data, cv::Mat &vocabulary) {
  // Initialize the SURF extractor/detector and BoW extractor
  cv::Ptr<cv::xfeatures2d::SurfFeatureDetector> detector = cv::xfeatures2d::SURF::create();
  cv::Ptr<cv::DescriptorExtractor> extractor = cv::xfeatures2d::SURF::create();
  cv::Ptr<cv::DescriptorMatcher> matcher = cv::BFMatcher::create();
  cv::BOWImgDescriptorExtractor bow_extractor(extractor, matcher);

  bow_extractor.setVocabulary(vocabulary);

  cv::Mat temp_img = cv::imread(file_path);
  cv::Mat bow_descriptor;

  // Get the key points of the temp_img
  vector<cv::KeyPoint> key_points = get_key_points(temp_img);

  // Extract SURF descriptors for the temp_img, and then extract the Bag of Visual Words histogram for it.
  extractor->detect(temp_img, key_points);
  bow_extractor.compute(temp_img, key_points, bow_descriptor);

  /* If the out_training_data matrix has not yet been initialized then initialize it based on the number of features
   * computed by SURF, and the type of these features i.e, float, double, int, etc.
   * See OpenCV documentation for a detailed explanation of the types.
   */
  if (out_training_data.empty()) {
    out_training_data.create(0, bow_descriptor.cols, bow_descriptor.type());
  }
  out_training_data.push_back(bow_descriptor);

  WriteHistogramToDisk(file_path, bow_descriptor);
}

/**
 * Recursive function to generate a Bag of Visual Words histogram for each image present within the specified vector
 * of images
 * @param images vector<std::string> a vector containing the relative file paths of the images within the user
 * specified data set. See main::readme() for more information.
 * @param out_training_data cv::Mat an output matrix object to append each histogram onto
 * @param vocabulary_name std::string the name of the vocabulary file to use.
 */
void ComputeHistograms(vector<string> &images, cv::Mat &out_training_data, string &vocabulary_name) {
  /* If the histograms directory does not already exist:
   *  1. Construct the histogram directory
   *  2. Compute each histogram for the data located within the data/images/ folder
   *  3. Write the histograms to disk to simply read from on the next run instead of needing to recompute them
   *
   * Otherwise, simply read the already constructed histograms from disk.
   */
  string file_path = "data/classifier/svm_training.yml";

  if (!exists("data/histograms")) {
    create_directory("data/histograms");
    cv::Mat vocabulary = ReadVocabularyFromDisk(vocabulary_name);
    cout << "Constructing histograms" << endl;
    for (string &img : images) {
      ComputeHistogram(img, out_training_data, vocabulary);
    }
    WriteSVMTrainingDataToDisk(file_path, out_training_data);
  }

  out_training_data = ReadSVMTrainingDataFromDisk(file_path);
}