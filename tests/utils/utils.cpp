/**
 * utils.cpp
 *
 * Provides some general utility functionality such as dealing with file system navigation, and obtaining file names
 */
#include <vector>
#include <string>
#include <boost/filesystem.hpp>

#include "utils.hpp"

using namespace boost::filesystem;
using namespace std;

/**
 * Given an absolute path to an image, i.e, ~/Documents/Pictures/cat.png extract the file name of the image
 * @param image_full_path std::string the absolute path to an image
 * @return std::string the file name of an image
 */
string utils::Utility::get_file_name_from_full_path(string &image_full_path) {
  // No sub-folder containing the image, return just the image name as it is in the current directory
  if (image_full_path.find_last_of("/\\") == string::npos) {
    return image_full_path;
  }

  return image_full_path.substr(image_full_path.find_last_of("/\\") + 1);
}

/**
 * Given a relative path to the directory of an image, extract the class label associated with it.
 * Example data/images/bathtub/008_00237.jpg --> `bathtub`
 * @param image_full_path std::string the relative path to an image
 * @return std::string the class label associated with the given image
 */
string utils::Utility::get_image_label(string &image_full_path) {
  /* Find the first occurrence of / --> data/ and the last occurrence XXX.some_class_label/XXX.XXXX.jpg
   * and extract the some_class_label portion.
   * Note: this while break if there is a nested structure within each class (i.e, bathtub/victorian_tub/XXX.XXX.jpg)
   */
  int start_idx = image_full_path.find_first_of('.') + 1;
  int end_idx = image_full_path.find_last_of("/\\");

  // Ensure there was a match actually found
  assert(start_idx > -1);
  assert(end_idx > -1);

  return image_full_path.substr(start_idx, end_idx-start_idx);
}

/**
 * Iterates over an entire directory and obtains all of the image names present within it. Additionally works for sub
 * directories within the main directory.
 * @param db_dir std::string the relative path to the directory holding the data set of images
 * @return std::vector<std::string> a vector composed of all of the relative paths to the images
 */
std::vector<std::string> utils::Utility::get_image_names_from_dir(std::string db_dir) {
  recursive_directory_iterator end;
  path p(db_dir);
  vector<string> images;
  for (recursive_directory_iterator itr(p); itr != end; ++itr) {
    if (!is_directory(itr->path())) {
      images.push_back(itr->path().string());
    }
  }
  return images;
}

/**
 * Iterates over the directories within the given dir_path parameter, and constructs a vector storing the appropriate
 * name for each folder.
 * Example data/images/008.bathtub/ => {"bathtub"}
 * @param dir_path std::sting the relative path to the directory to iterate over
 * @return vector<std::string> the names of each class label as provided by the directory name
 * (i.e, data/images/008.bathtub/ => {"bathtub", ...})
 */
vector<string> utils::Utility::get_classes(const string &dir_path) {
  path p(dir_path);
  recursive_directory_iterator end;
  vector<string> classes;

  for (recursive_directory_iterator itr(p); itr != end; ++itr) {
    if (is_directory(itr->path())) {
      int i = itr->path().string().find_last_of("/\\");
      string class_ = itr->path().string().substr(i+1, itr->path().string().size()-i);
      classes.push_back(class_);
    }
  }
  assert(!classes.empty());

  return classes;
}
