#include <vector>
#include <opencv2/opencv.hpp>
#include <utils.h>
#include <IndicesMapping.h>

#include "PerceptualHash.h"
#include "Surf.h"


using namespace std;

void readme();

int main(int argc, char** argv) {

  // Assumes argv[2] is the directory of images;
  if (argc != 3) {
    readme();
    return -1;
  }

  cv::Mat query_image = cv::imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);

  if (!query_image.data) {
    printf("Error reading query image\n");
    return -1;
  }
  // Get feature vector of the query image
  vector<cv::KeyPoint> key_points = get_key_points(query_image);
  cv::Mat query_descriptors = get_single_feature_vector(query_image, key_points);

  // Iterate over all images in DB and obtain their feature vectors
  string db_dir = argv[2];
  vector<string> db_images = get_image_names_from_dir(db_dir);
  vector<IndicesMapping> indices_map;
  vector<cv::Mat> database_descriptors = get_multiple_feature_vectors(db_images, indices_map);
  cv::Mat concatenated_descriptors = ConcatenateDescriptors(database_descriptors);

  // Create FLANN Index
  int K = 2;
  cv::Mat indices;
  cv::Mat dists;

  cv::flann::Index flann_index(concatenated_descriptors, cv::flann::KDTreeIndexParams(8));
  flann_index.knnSearch(query_descriptors, indices, dists, K, cv::flann::SearchParams(128));

  for (int i = 0; i < query_descriptors.rows; i++) {
    if (dists.at<float>(i, 0) <= (0.6 * dists.at<float>(i, 1))) {
      for (IndicesMapping &x : indices_map) {
        if (x.get_index_start() <= indices.at<int>(i, 1) && x.get_index_end() >= indices.at<int>(i, 1)) {
          int new_sim = x.get_similarity() + 1;
          x.set_similarity(new_sim);
          break;
          }
        }
      }
    }

  vector<IndicesMapping> mapping_copy = indices_map;
  sort(mapping_copy.begin(), mapping_copy.end(), IndicesMapping::SimilartySort());

  vector<string> matches;
  for (int i = 0; i < 10; i++) {
    printf("Match File: %s", mapping_copy.at(i).get_file_name().c_str());
    printf(" Similarity: %d\n", mapping_copy.at(i).get_similarity());
    }
//
//  if (!matches.empty()) {
//    cv::imshow("Best Match", cv::imread(matches.at(matches.size()-1)));
//    cv::waitKey(0);
//  }
  return 0;
}

void readme() {
}
