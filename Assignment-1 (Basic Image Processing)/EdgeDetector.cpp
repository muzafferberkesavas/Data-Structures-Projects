// EdgeDetector.cpp
#include "EdgeDetector.h"
#include <cmath>
#include "ImageMatrix.h"
#include "Convolution.h"
#include "ImageLoader.h"

// Default constructor
EdgeDetector::EdgeDetector() {
    this->Gx = new double * [3];
    this->Gx[0] = new double [3] {-1,0,1};
    this->Gx[1] = new double [3] {-2,0,2};
    this->Gx[2] = new double [3] {-1,0,1};
    this->Gy = new double * [3];
    this->Gy[0] = new double [3] {-1,-2,-1};
    this->Gy[1] = new double [3] {0,0,0};
    this->Gy[2] = new double [3] {1,2,1};
}

// Destructor
EdgeDetector::~EdgeDetector() {
        for (int i = 0; i < 3; ++i) {
            delete[] Gx[i];
            delete[] Gy[i];
        }
        delete[] Gx;
        delete[] Gy;
}

// Detect Edges using the given algorithm
std::vector<std::pair<int, int>> EdgeDetector::detectEdges(const ImageMatrix& input_image) {

    ImageMatrix output_image(input_image.get_height(),input_image.get_width());
    Convolution convolvex(Gx,3,3,1, true);
    Convolution convolvey(Gy,3,3,1, true);
    ImageMatrix Ix = convolvex.convolve(input_image);
    ImageMatrix Iy = convolvey.convolve(input_image);
    double magnitude1 = 0;
    double magnitude2;
    for (int i = 0; i < Ix.get_height(); ++i) {
        for (int j = 0; j < Ix.get_width(); ++j) {
            magnitude2 = sqrt(pow(Ix.get_data(i,j),2)+ pow(Iy.get_data(i,j),2));
            magnitude1 += magnitude2;
            output_image.set_data(i,j,magnitude2);
        }
    }
    double threshold;
    threshold = magnitude1 / (Ix.get_height()*Ix.get_width());
    std::vector<std::pair<int,int>> EdgePixels;
    for (int i = 0; i < output_image.get_height(); ++i) {
        for (int j = 0; j < output_image.get_width(); ++j) {
            if(output_image.get_data(i,j) > threshold){
                EdgePixels.emplace_back(i,j);
            }
        }
    }
    return EdgePixels;
}

