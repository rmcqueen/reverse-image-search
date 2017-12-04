/**
 * Vocabulary.cpp
 * 
 * This class is used in order to construct the initial Bag of Visual Words vocabulary file in order to further extract
 * the relevant Bag of Visual Words histogram for each image within the data set being used. Includes various IO
 * operations in order to save the vocabulary to disk, and avoid a long computation time in the event the system needs 
 * to be ran again.
 */
#include <iostream>
#include <opencv2/core.hpp>
#include <boost/filesystem.hpp>

#include "Surf.hpp"

using namespace std;
using namespace boost::filesystem;

/**
 * Write the computed vocabulary matrix to disk to save a lot of time in the event the system needs to be ran again as
 * the vocabulary computation is very time consuming
 * @param matrix cv::Mat the matrix object of the vocabulary
 * @param file_name std::string the file name to save the vocabulary as
 */
void WriteVocabularyToDisk(cv::Mat &matrix, const string &file_name) {
  cv::FileStorage fs(file_name, cv::FileStorage::WRITE);
  fs << "vocabulary" << matrix;
  fs.release();
}

/**
 * Validate whether or not the dictionary to be writing/reading to exists
 * @param file_name std::string the name of the vocabulary file
 * @return bool true|false on whether or not the vocabulary exists
 */
bool VocabularyExists(const string &file_name) {
  return exists(file_name);
}

/**
 * Read the existing vocabulary from disk 
 * @param file_name std::string the name of the vocabulary.
 * @return cv::Mat the vocabulary stored on disk
 */
cv::Mat ReadVocabularyFromDisk(const string &file_name) {
  // Ensure the vocabulary actually exists
  assert(VocabularyExists(file_name));

  cout << "Reading vocabulary from disk..." << endl;
  cv::Mat matrix;
  cv::FileStorage fs(file_name, cv::FileStorage::READ);
  fs["vocabulary"] >> matrix;
  fs.release();
  return matrix;
}

/**
 * Constructs the vocabulary file to be used.
 * Note: the file_name must end in .yml
 * @param training_descriptors cv::Mat a matrix object containing concatenated feature descriptors from multiple
 * independent images
 * @param file_name std::string the file name to give to the vocabulary. Note: this does not construct a directory if it
 * does not already exist. If it is desired to create a new directory, this must be implemented in the code, or done
 * manually.
 * @param write_to_disk bool indicates whether or not to write the vocabulary to disk. Default is true
 */
cv::Mat ConstructVocabulary(cv::Mat &training_descriptors, const string &file_name, bool write_to_disk=true) {
  // Ensure we have some training descriptors in order to construct the vocabulary
  assert(training_descriptors.rows > 0);

  // See if a dictionary is already present with the specified file name
  if (VocabularyExists(file_name)) {
    cout << "Dictionary already exists, returning existing dictionary" << endl;
    return ReadVocabularyFromDisk(file_name);
  }

  // Set the type of the vocabulary matrix to hold 32 bit float values
  cv::Mat vocabulary(0, 1, CV_32FC1);

  /* 
   * Set the number of words to use in the dictionary to 2500, number of retries for word computations to 1
   * and the method to be the KMeans centroid centers
   * See: OpenCV documentation for further explanation of this chunk of code.
   */
  int dictionary_size = 2500;
  cv::TermCriteria term_criteria(CV_TERMCRIT_ITER, 100, 0.001);
  int retries = 1;
  int flags = cv::KMEANS_PP_CENTERS;

  // Construct the KMeans algorithm with the specified parameters
  cv::BOWKMeansTrainer bow_trainer(dictionary_size, term_criteria, retries, flags);
  // Include the training data to use for construction of the vocabulary
  bow_trainer.add(training_descriptors);
  
  vocabulary = bow_trainer.cluster();

  // If the dictionary is to be written to disk then write it. Default is to write to disk.
  if (write_to_disk) {
    WriteVocabularyToDisk(vocabulary, file_name);
  }

  cout << "Vocabulary constructed" << endl;
  return vocabulary;
}
