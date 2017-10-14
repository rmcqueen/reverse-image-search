/*
 * Modified version of Keypoint.h
 * Source location: https://github.com/pippy360/transformationInvariantImageSearch/blob/master/fullEndToEndDemo/src/Keypoint.h
*/
#pragma once
#ifndef REVERSE_IMAGE_SEARCH_KEYPOINT_H
#define REVERSE_IMAGE_SEARCH_KEYPOINT_H

#include <iostream>
#include <sstream>

using namespace std;

class KeyPoint {
  public:
    double x_, y_;
    KeyPoint(double x, double y) {
        x_ = x;
        y_ = y;
    };

    /**
     * Convert a KeyPoint to a string
     * @return string
     */
    string ToString() {
        std::ostringstream string_stream_;
      string_stream_<< "KeyPoint[ " << x_ << ", " << y_ << "]";
        return string_stream_.str();
    }
};

namespace hash {
  template <>
  struct hash<KeyPoint> {
      std::size_t operator()(const KeyPoint &k) const {
        using std::hash;
          return ((hash<double>()(k.x_) ^ (hash<double>()(k.y_) << 1)) >> 1);
      }
  };
}
#endif