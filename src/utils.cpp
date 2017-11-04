/**
 * utils.cpp
 * Provides some general utility functionality such as dealing with file system navigation, and obtaining file names
 */
#include <vector>
#include <string>
#include <boost/filesystem.hpp>

#include "utils.h"

using namespace boost::filesystem;

/**
 * Given an absolute path to an image, i.e, ~/Documents/Pictures/cat.png extract the file name of the image
 * @param image_full_path std::string the absolute path to an image
 * @return std::string the file name of an image
 */
string get_file_name_from_full_path(string &image_full_path) {
  // No sub-folder containing the image, return just the image name as it is in the current directory
  if (image_full_path.find_last_of("/\\") == string::npos) {
    return image_full_path;
  }

  return image_full_path.substr(image_full_path.find_last_of("/\\") + 1);
}

/**
 * Loads a particular image based on the absolute path, and constructs an Image object
 * @param image_full_path std::string the absolute path to the image
 * @return Image a constructed Image object
 */
Image get_loaded_image(string &image_full_path) {
  cout << "Loading Image: " << image_full_path;
  cv::Mat img = cv::imread(image_full_path);
  cout << " Image Loaded" << endl;
  string file_name = get_file_name_from_full_path(image_full_path);
  return Image(file_name, img, image_full_path);
}

/**
 * Iterates over an entire specified directory, and returns a vector containing all of the file names within sub-directories
 * and non-sub directories
 * @param dir_name std::string the absolute path of the directory
 * @return vector<string> a vector containing all of the image names present within the specified directory
 */
vector<string> get_image_names_from_dir(string &dir_name) {
  vector<string> file_names;
  path p(dir_name);
  recursive_directory_iterator end;

  for (recursive_directory_iterator itr(p); itr != end; ++itr) {
    if (!is_directory(itr->path())) {
      file_names.push_back(itr->path().string());
    }
  }

  return file_names;
}