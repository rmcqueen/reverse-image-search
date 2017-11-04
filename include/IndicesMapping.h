/**
 * IndicesMapping.h
 * Brief: Incorporates a mapping for each image stored within the image database.
 *
 * The purpose is to include relevant information for each image, such as the name of the image, and the position of
 * a particular image within the master feature vector array storing all images, as well as within the FLANN index.
 *
 */
#pragma once
#ifndef REVERSE_IMAGE_SEARCH_INDICESMAPPING_H
#define REVERSE_IMAGE_SEARCH_INDICESMAPPING_H

#include <string>

using namespace std;

class IndicesMapping {
 public:
  /**
   * A constructor for IndicesMapping
   * @param file_name std::string the name of the image file
   * @param index_start int indicates the starting row of the image's feature vector stored within the FLANN index
   * @param index_end int indicates the ending row of the image's feature vector stored within the FLANN index
   */
  IndicesMapping(
      string file_name,
      int index_start,
      int index_end
  );

  IndicesMapping(
      string file_name,
      int index_start,
      int index_end,
      int similarity
  );


  struct SimilartySort{
    inline bool operator() (const IndicesMapping &a, const IndicesMapping &b) {
      return (a.get_similarity() > b.get_similarity());
    }
  };

  /**
   *
   * @return std::string the name of the image file
   */
  string &get_file_name() {
    return file_name_;
  }

  /**
   *
   * @param file_name string the name of the image file
   */
  void set_file_name(string &file_name) {
    file_name_ = file_name;
  }

  /**
   *
   * @return int starting index of the image's feature vector within the FLANN index
   */
  int &get_index_start() {
    return index_start_;
  }

  /**
   *
   * @param index_start int the starting index of the image's feature vector within the FLANN index
   */
  void set_index_start(int &index_start) {
    index_start_ = index_start;
  }

  /**
   *
   * @return int ending index of the image's feature vector within the FLANN index
   */
  int &get_index_end() {
    return index_end_;
  }

  /**
   *
   * @param index_end int the ending index of the image's feature vector within the FLANN index
   */
  void set_index_end(int &index_end) {
    index_end_ = index_end;
  }

  /**
   *
   * @return similarity int the count similarity of a database image compared to the querying image
   */
  int get_similarity() const {
    return similarity_;
  }

  /**
   *
   * @param similarity int the count similarity between a query image and a database image
   */
  void set_similarity(int &similarity) {
    similarity_ = similarity;
  }

 private:
  string file_name_;
  int index_start_;
  int index_end_;
  int similarity_;
};

inline IndicesMapping::IndicesMapping(
    string file_name,
    int index_start,
    int index_end) : file_name_(file_name),
                     index_start_(index_start),
                     index_end_(index_end) {}

inline IndicesMapping::IndicesMapping(
    string file_name,
    int index_start,
    int index_end,
    int similarity) : file_name_(file_name),
                      index_start_(index_start),
                      index_end_(index_end),
                      similarity_(similarity) {}

#endif //REVERSE_IMAGE_SEARCH_INDICESMAPPING_H
