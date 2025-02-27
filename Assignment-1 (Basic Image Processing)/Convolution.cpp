#include "ImageMatrix.h"
#include "Convolution.h"
#include "ImageLoader.h"

// Default constructor 
Convolution::Convolution() {
    kernelHeight = 2;
    kernelWidth = 2;
    stride = 1;
    padding = false;
    customKernel = new double * [kernelHeight];
    for (int i = 0; i < kernelHeight;i++){
        customKernel[i] = new double[kernelWidth];
        for (int j = 0; j < kernelWidth; j++){
            customKernel[i][j] = 1;
        }
    }
}

// Parametrized constructor for custom kernel and other parameters
Convolution::Convolution(double** customKerneL, int kh, int kw, int stride_val, bool pad){

    kernelHeight = kh;
    kernelWidth = kw;
    stride = stride_val;
    padding = pad;
    customKernel = new double * [kernelHeight];
    for (int i = 0; i < kernelHeight; ++i) {
        customKernel[i] = new double [kernelWidth];
        for (int j = 0; j < kernelWidth; ++j) {
            customKernel[i][j] = customKerneL[i][j];
        }
    }
}

// Destructor
Convolution::~Convolution() {
        for (int i = 0; i < kernelHeight; ++i) {
            delete[] customKernel[i];
        }
        delete[] customKernel;
}

// Copy constructor
Convolution::Convolution(const Convolution &other){
        kernelHeight = other.kernelHeight;
        kernelWidth = other.kernelWidth;
        stride = other.stride;
        padding = other.padding;

        customKernel = new double *[kernelHeight];
        for (int i = 0; i < kernelHeight; ++i) {
            customKernel[i] = new double [kernelWidth];
            for (int j = 0; j < kernelWidth; ++j) {
             customKernel[i][j] = other.customKernel[i][j];
            }
    }
}

// Copy assignment operator
Convolution& Convolution::operator=(const Convolution &other) {
    if (this == &other){
        return *this;
    }
    for (int i = 0; i < kernelHeight; ++i) {
        delete[] customKernel[i];
    }
    delete[] customKernel;

    kernelHeight = other.kernelHeight;
    kernelWidth = other.kernelWidth;
    stride = other.stride;
    padding = other.padding;

    customKernel = new double * [kernelHeight];
    for (int i = 0; i < kernelHeight; i++){
        customKernel[i] = new double[kernelWidth];
        for (int j = 0; j < kernelWidth; j++){
            customKernel[i][j] =other.customKernel[i][j];
        }
    }
    return *this;
}


// Convolve Function: Responsible for convolving the input image with a kernel and return the convolved image.
ImageMatrix Convolution::convolve(const ImageMatrix& input_image) const {
    int outputHeight,outputWidth;
    if (padding){
        outputHeight = ((input_image.get_height()-kernelHeight+2) / stride)+1;
        outputWidth = ((input_image.get_width()-kernelWidth+2) / stride)+1;
        ImageMatrix result(outputHeight,outputWidth);
        ImageMatrix output_image(input_image.get_height()+2,input_image.get_width()+2);
        for (int i = 1; i < input_image.get_height()+1 ; ++i) {
            for (int j = 1; j < input_image.get_width()+1 ; ++j) {
                output_image.set_data(i,j,input_image.get_data(i-1,j-1));
            }
        }
        int index1 = -1;
        int index2;
        for (int i = 0; i < output_image.get_height(); i +=stride){
            restart:
            index1++;
            index2 = 0;
            for (int j = 0; j < output_image.get_width(); j += stride) {
                double total = 0;
                for (int k = 0; k < kernelHeight; ++k) {
                    for (int l = 0; l < kernelWidth; ++l) {
                        if (j+k < output_image.get_height()){
                            total +=(output_image.get_data(k+i,l+j) * customKernel[k][l]);
                        }
                        else{
                            i+=stride;
                            goto restart;
                        }
                        if (i+k >= output_image.get_width()){
                            goto end;
                        }
                    }
                }
                result.set_data(index1 ,index2,total);
                index2++;
            }
        }
        end:
        return result;
    } else{
        outputHeight = ((input_image.get_height()-kernelHeight) / stride ) + 1;
        outputWidth = ((input_image.get_width()-kernelWidth) / stride) + 1;
        ImageMatrix result(outputHeight,outputWidth);
        ImageMatrix output_image(input_image.get_height(),input_image.get_width());
        for (int i = 0; i < input_image.get_height() ; ++i) {
            for (int j = 0; j < input_image.get_width() ; ++j) {
                output_image.set_data(i,j,input_image.get_data(i,j));
            }
        }
        int index1 = -1;
        int index2;
        for (int i = 0; i < output_image.get_height(); i+=stride){
            restart2:
            index1++;
            index2 = 0;
            for (int j = 0; j < output_image.get_width(); j+=stride) {
                double total = 0;
                for (int k = 0; k < kernelHeight; ++k) {
                    for (int l = 0; l < kernelWidth; ++l) {
                        if (j+k < output_image.get_height()){
                            total += (output_image.get_data(k+i,l+j) * customKernel[k][l]);
                        }
                        else{
                            i+=stride;
                            goto restart2;
                        }
                        if (i+k >= output_image.get_width()){
                            goto end2;
                        }
                    }
                }
                result.set_data(index1,index2,total);
                index2++;
            }
        }
        end2:
        return result;
    }
}
