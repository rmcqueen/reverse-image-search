/*
 * Modified version of Keypoint.h
 * Source location: https://github.com/pippy360/transformationInvariantImageSearch/blob/master/fullEndToEndDemo/src/Keypoint.h
*/

#include <iostream>
#include <sstream>

using namespace std;

class KeyPoint {
public:
    double x, y;
    KeyPoint(double _x, double _y) {
        x = _x;
        y = _y;
    };

    /**
     * Convert a KeyPoint to a string
     * @return string
     */
    string toString() {
        std::ostringstream stringStream;
        stringStream << "KeyPoint[ " << x << ", " << y << "]";
        return stringStream.str();
    }
};

template <>
struct hash<KeyPoint> {
    std::size_t operator()(const KeyPoint &k) const {
        return ((hash<double>()(k.x) ^ (hash<double>()(k.y) << 1)) >> 1);
    }
};