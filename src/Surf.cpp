#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;

void readMe();
string concatenatePath(string path1, string path2) ;

int main(int argc, char** argv) {
    if(argc != 3) {
        readMe();
        return -1;
    }

    Mat img = imread(argv[1]);
    string dst = concatenatePath(argv[2], "surf_img.jpg");


    if(!img.data) {
        std::cout << "--- Error reading images ---" << std::endl;
        return -1;
    }

    // 1. Detect Keypoints via SURF
    int minHessian = 400;
    Ptr<SURF> detector = SURF::create();
    detector->setHessianThreshold(minHessian);
    std::vector<KeyPoint> keypoints;
    Mat descriptors;
    detector->detectAndCompute(img, Mat(), keypoints, descriptors);

    // 2. Draw Keypoints on original image
    Mat imgKeyPoints;
    drawKeypoints(img, keypoints, imgKeyPoints, Scalar::all(-1), DrawMatchesFlags::DEFAULT);

    // 3. Write image to user specified location
    imwrite(dst, imgKeyPoints);
    return 0;
}


string concatenatePath(string path1, const string path2) {
    if(path1.back() == '/') {
        path1.append(path2);
    } else {
        path1.append("/");
        path1.append(path2);
    }
    return path1;
}

void readMe() {
    std::cout << "Usage: ./surf path/to/image/ path/to/write \n";
}

