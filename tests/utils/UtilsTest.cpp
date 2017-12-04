#include <gtest/gtest.h>
#include <opencv2/opencv.hpp>
#include <boost/filesystem.hpp>
#include <stdio.h>
#include <vector>

#include "utils.hpp"

TEST(FileNameFromFullPath, UtilsTest) {
  std::string full_path = "../../data/001.ak47/001_0001.jpg";
  ASSERT_EQ(utils::Utility::get_file_name_from_full_path(full_path), "001_0001.jpg");
}

TEST(ImageNamesFromDirectory, UtilsTest) {
  std::string  directory_path = "../../data/257.clutter";
  std::vector<std::string> image_names = utils::Utility::get_image_names_from_dir(directory_path);
  ASSERT_EQ(image_names.at(0), "257_0001.jpg");
  int last_idx = image_names.size()-1;
  cout << image_names.at(last_idx) << endl;
  ASSERT_EQ(image_names.at(last_idx), "257_0827.jpg");
}

TEST(WriteVocabularyToDisk, UtilsTest) {
  cv::Mat mat(cv::Mat::zeros(10, 10, 5));
  std::string vocab_name = "test_mat.yml";
  utils::Utility::WriteVocabularyToDisk(mat, vocab_name);
  ASSERT_TRUE(boost::filesystem::exists("test_mat.yml"));
}

TEST(ReadVocabularyFromDisk, UtilsTest) {
  std::string file_path = "dictionary_test.yml";
  cv::Mat vocab = utils::Utility::ReadVocabularyFromDisk(file_path);
  int vocab_rows = vocab.rows;
  ASSERT_EQ(vocab_rows, 2500);
  ASSERT_EQ(vocab.cols, 64);
}

TEST(DictionaryExists, UtilsTest) {
  std::string file_path = "../../data/001.ak47/";
  ASSERT_TRUE(boost::filesystem::exists(file_path));
}