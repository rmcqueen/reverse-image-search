/*
 * Modified version of PerceptualHash.h
 * Source location: https://github.com/pippy360/transformationInvariantImageSearch/blob/master/fullEndToEndDemo/src/PerceptualHash.h
 */
#ifndef REVERSE_IMAGE_SEARCH_PERCEPTUALHASH_H
#define REVERSE_IMAGE_SEARCH_PERCEPTUALHASH_H

#include <string>
#include <vector>
#include <memory>
#include <iomanip>
#include <opencv2/opencv.hpp>

#include "KeyPoint.h"
#include "img_hash_opencv_module/phash.hpp"

using namespace std;


class PerceptualHash {
  private:
      vector<bool> hash_;
      vector<KeyPoint> shape_;

      /**
       * Converts the hash to a hex string
       * @param hash
       * @return string
       */
      static std::string ConvertHashToString(vector<bool> hash) {
          std::string ret = "";
          int h = 0;
          for (unsigned int i = 0; i < hash.size(); i++) {
              if (hash[i]) {
                  h += pow(2, (i % 8));
              }

              if (i % 8 == 7) {
                  std::stringstream buffer;
                  buffer << std::hex << std::setfill('0') << std::setw(2) << h;
                  ret += buffer.str();
                  h = 0;
              }
          }
          return ret;
      }

      /**
       * Convert a hexadecimal string to the appropriate hash value
       * @param inputString
       * @return vector<bool> hash for the given string
       */
      static std::vector<bool> HexStringToHash(std::string inputString) {
          std::vector<bool> hash;
          int size = int(inputString.size() / 2);
          for (int i = 0; i < size; i++) {
              std::string str2 = inputString.substr(i * 2, 2);
              if (str2.empty()) {
                  continue;
              }

              unsigned int value = 0;
              std::stringstream SS(str2);
              SS >> std::hex >> value;
              for (int j = 0; j < 8; j++) {
                  // Perform an AND operation to ensure they're the same value
                  bool check = ((value >> j) & 1) != 0;
                  hash.push_back(check);
              }
          }
          return hash;
      }

      /**
       * Helper function for computeHash. Constructs a bool array for a given Image matrix
       * @param inHash
       * @return
       */
      static std::vector<bool> MatrixHashToBoolArray(cv::Mat const inHash) {
          const unsigned char* data = inHash.data;
          std::vector<bool> v;
          for (int i = 0; i < 8; i++) {
              unsigned char c = data[i];
              for (int j = 0; j < 8; j++) {
                  int shift = (8 - j) - 1;
                  bool val = ((c >> shift) & 1) != 0;
                  v.push_back(val);
              }
          }
          return v;
      }

      /**
       * Computes the phash value of a given Image matrix
       * @param input
       * @return
       */
      static vector<bool> ComputeHash(cv::Mat const input) {
          cv::Mat inHash;
          auto algorithm = cv::img_hash::PHash();
          algorithm.compute(input, inHash);
          return MatrixHashToBoolArray(inHash);
      }


    // TODO: Implement function to get the hash distance between two Image objects, and their respective has values
};
#endif //REVERSE_IMAGE_SEARCH_PERCEPTUALHASH_H
