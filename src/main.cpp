/**
 * main.cpp
 *
 * The "main" file for the system in which every component is incorporated and provides full functionality of the system.
 *
 * Example 1: The first initial compilation, and run of the system:
 *
  string db_dir = argv[2];

  // Iterate over all images in DB and obtain their feature vectors
  vector<string> db_images = Utility::get_image_names_from_dir(db_dir);
  vector<cv::Mat> feature_descriptors = get_multiple_feature_vectors(db_images);
  cv::Mat concatenated_descriptors = ConcatenateDescriptors(feature_descriptors);


  string vocabulary_name = "vocabulary.yml";
  ConstructVocabulary(concatenated_descriptors, "vocabulary.yml", false);

  cv::Mat training_data;
  ComputeHistograms(db_images, training_data, vocabulary_name);

  cv::Ptr<cv::ml::SVM> svm = cv::ml::SVM::create();

  // Note: these params were found from performing parameter estimation on a smaller subset of the data.
  // More desirable results may be acquired from modifying the gamma and C values.
  svm->setType(cv::ml::SVM::C_SVC);
  svm->setKernel(cv::ml::SVM::RBF);
  svm->setGamma(0.50625);
  svm->setC(34389);

  string _dir = "data/histograms/";
  TrainSVM(_dir, 64, CV_32FC1, svm);
  cv::Mat vocabulary = ReadVocabularyFromDisk(vocabulary_name);


  string query_path = argv[1];
  cv::Mat query_hist = ComputeHistogram(query_path, vocabulary);

  string best_match = TestSVM(query_hist, svm);
  cout << best_match << endl;

 * After this has been ran for the first time, the feature descriptors are no longer required to be computed and
 * concatenated as they already exist. If you wish to incorporate new images, or a new data set, then it must be ran as
 * shown in the previous example.
 *
 * Example 2: Additional compilation with NO modifications to the original data set:
  string db_dir = argv[2];

  string vocabulary_name = "vocabulary.yml";
  cv::Ptr<cv::ml::SVM> svm = cv::ml::SVM::create();

  svm->setType(cv::ml::SVM::C_SVC);
  svm->setKernel(cv::ml::SVM::RBF);
  svm->setGamma(0.50625);
  svm->setC(34389);

  string _dir = "data/histograms/";
  // Will not re-train the SVM, checks to see if the predictor.yml file already exists on disk, and uses that if so.
  TrainSVM(_dir, 64, CV_32FC1, svm);

  cv::Mat vocabulary = ReadVocabularyFromDisk(vocabulary_name);

  // Compute the Bag of Visual Words for the query image and feed it into the SVM predictor
  string query_path = argv[1];
  cv::Mat query_hist = ComputeHistogram(query_path, vocabulary);

  string best_match = TestSVM(query_hist, svm);
  cout << best_match << endl;
 */
#include <vector>
#include <opencv2/opencv.hpp>
#include <boost/filesystem.hpp>

#include "utils.hpp"
#include "Surf.hpp"
#include "Histogram.hpp"
#include "SVM.hpp"
#include "Vocabulary.hpp"

using namespace std;
using namespace utils;
using namespace boost::filesystem;

void readme();

int main(int argc, char** argv) {

  // Assumes argv[2] is the directory of images;
  if (argc != 3) {
    readme();
    return -1;
  }

  string db_dir = argv[2];

// Iterate over all images in DB and obtain their feature vectors
  vector<string> db_images = utils::Utility::get_image_names_from_dir(db_dir);
  vector<cv::Mat> feature_descriptors = get_multiple_feature_vectors(db_images);
  cv::Mat concatenated_descriptors = ConcatenateDescriptors(feature_descriptors);


  string vocabulary_name = "vocabulary.yml";
  ConstructVocabulary(concatenated_descriptors, "vocabulary.yml", false);

  cv::Mat training_data;
  ComputeHistograms(db_images, training_data, vocabulary_name);

  cv::Ptr<cv::ml::SVM> svm = cv::ml::SVM::create();

  // Note: these params were found from performing parameter estimation on a smaller subset of the data.
  // More desirable results may be acquired from modifying the gamma and C values.
  svm->setType(cv::ml::SVM::C_SVC);
  svm->setKernel(cv::ml::SVM::RBF);
  svm->setGamma(0.50625);
  svm->setC(34389);

  string _dir = "data/histograms/";
  TrainSVM(_dir, 64, CV_32FC1, svm);
  cv::Mat vocabulary = ReadVocabularyFromDisk(vocabulary_name);

  string query_path = argv[1];
  cv::Mat query_hist = ComputeHistogram(query_path, vocabulary);

  string best_match = TestSVM(query_hist, svm);
  cout << best_match << endl;

  return 0;
}

void readme() {
  cout << "usage: ./reverse-image-search query_img.jpg data/" << endl;
}
