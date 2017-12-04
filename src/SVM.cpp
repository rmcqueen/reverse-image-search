/**
 * SVM.cpp
 *
 * This class implements the features of the Support Vector Machine classifier as outlined within the OpenCV library.
 * It includes some utility functions such as reading/writing SVM training data to disk, and obtaining the classes
 */
#include <opencv2/core.hpp>
#include <opencv2/ml/ml.hpp>
#include <boost/filesystem.hpp>
#include "opencv2/imgcodecs.hpp"
#include <opencv2/imgproc.hpp>

#include "Histogram.hpp"
#include "SVM.hpp"
#include "Surf.hpp"
#include "utils.hpp"

using namespace std;
using namespace boost::filesystem;

/**
 * Reads the existing SVM training data from disk. Rows are histograms which correspond to a particular image.
 * @param file_path
 * @return
 */
cv::Mat ReadSVMTrainingDataFromDisk(string &file_path) {
  cv::Mat training_data;
  cv::FileStorage fs(file_path, cv::FileStorage::READ);
  fs["svm_training"] >> training_data;
  fs.release();
}

/**
 * Writes the computed Bag of Visual Words histograms to disk in order to use for training the SVM
 * @param file_path std::string the relative file path to write the training data to. Expects a .yml file
 * @param training_data cv::Mat the concatenated histograms to write to disk
 */
void WriteSVMTrainingDataToDisk(string &file_path, cv::Mat &training_data) {
  if (!exists("data/classifer")) {
    create_directory("data/classifier");
  }

  if (boost::filesystem::exists(file_path)) {
    cout << "Training data already exists" << endl;
    return;
  }

  cv::FileStorage svm_training(file_path, cv::FileStorage::WRITE);
  svm_training << "svm_training" << training_data;
  svm_training.release();
}

/**
 * Trains the SVM with Bag of Visual Words histograms
 * @param class_file_path std::string the path to the directory containing the image histograms (i.e, data/histograms/)
 * @param response_cols int the number of columns that the training data have (for SURF this would be 64 as 64 feature
 * descriptors are extracted)
 * @param response_type int the type of the matrix to construct. See the OpenCV documentation for further explanation on
 * what the int values correspond to
 * @param out_svm cv::Ptr<cv::ml::SVM> the trained SVM to use for a prediction
 */
void TrainSVM(const string &class_file_path, int response_cols, int response_type, cv::Ptr<cv::ml::SVM> &out_svm) {
  vector<string> classes = utils::Utility::get_classes(class_file_path);

  cv::Mat samples(0, response_cols, response_type);
  cv::Mat labels(0, 1, CV_32SC1);
  for (int i = 0; i < classes.size(); i++) {
    cv::Mat training_data = ReadClassHistogramsFromDisk(class_file_path, classes[i]);
    samples.push_back(training_data);

    cv::Mat class_label = cv::Mat(training_data.rows, 1, CV_32SC1, i);
    labels.push_back(class_label);
  }
  if (exists("predictor.yml")) {
    cout << "Trained SVM already found, loading..." << endl;
    out_svm = out_svm->load("predictor.yml");
    return;

  } else {
    cout << "Trained SVM not found, training..." << endl;
    cv::Mat samples_32f;
    samples.convertTo(samples_32f, response_type);
    out_svm->train(samples_32f, cv::ml::ROW_SAMPLE, labels);
    out_svm->save("predictor.yml");
  }
}

/**
 * Makes an image prediction based on a trained SVM and from this, computes the highest similarity image based on a
 * cross-correlation histogram comparison between histograms in the predicted class.
 * @param test_img cv::Mat the image being searched for
 * @param svm cv::Ptr<cv::ml::SVM> a trained SVM that will be used for predicting the class of the query image
 * @return std::string returns an absolute string to the best matching image within the data set used
 */
string TestSVM(cv::Mat &test_img, cv::Ptr<cv::ml::SVM> &svm) {
  // Ensure the query image is not empty, and that the SVM is trained
  assert(!test_img.empty());
  assert(svm->isTrained());

  // Make the prediction
  float res = svm->predict(test_img);

  // TODO: Refactor this cross-correlation computation for the most similar image into utils.cpp
  vector<string> classes = utils::Utility::get_classes("data/histograms/");
  cv::Mat class_hists = ReadClassHistogramsFromDisk("data/histograms/", classes[(int)res]);
  cout << res << endl;
  double max = -9999;
  int max_index = 0;
  for (int i = 0; i < class_hists.rows; i++) {
    cv::Mat cur_db_hist = class_hists.row(i);
    double cur = cv::compareHist(test_img, cur_db_hist, CV_COMP_CORREL);
    if (cur > max) {
      max = cur;
      max_index = i;
    }
  }

  string base_img_path = "data/images";
  path predicted_img_path;
  path p(base_img_path);
  recursive_directory_iterator end;

  // Since when the class labels are extracted the digits before the label are removed, we must find the full label
  // in the images directory, and append this onto 'data/images' to get the full path back.
  for (recursive_directory_iterator itr(p); itr != end; ++itr) {
    if (is_directory(itr->path()) && itr->path().string().find(classes[int(res)]) != std::string::npos) {
         predicted_img_path = itr->path();
      }
  }


  int i = 0;
  string match_file_path;

  // Iterate over the images within the class until the image which is in the same index as the best match is found
  for (recursive_directory_iterator itr(predicted_img_path); itr != end; ++itr) {
    if (i == max_index) {
      match_file_path = itr->path().string();
      break;
    }
    i++;
  }

  return match_file_path;
}