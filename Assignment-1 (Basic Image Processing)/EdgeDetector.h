    #ifndef EDGE_DETECTOR_H
    #define EDGE_DETECTOR_H

#include "EdgeDetector.h"
#include <cmath>
#include <iostream>
#include "ImageMatrix.h"
#include "Convolution.h"
#include "ImageSharpening.h"
#include "DecodeMessage.h"
#include "EncodeMessage.h"
#include "ImageLoader.h"
#include "ImageProcessor.h"

    class EdgeDetector {
        public:
            EdgeDetector(); 
            ~EdgeDetector(); 
            
            std::vector<std::pair<int, int>> detectEdges(const ImageMatrix& input_image);

        private:
            double **Gx;
            double **Gy;
            // add your private member variables and functions 
    };

    #endif // EDGE_DETECTOR_H


