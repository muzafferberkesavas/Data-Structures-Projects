#include "ImageMatrix.h"
#include "Convolution.h"
#include "ImageSharpening.h"
#include "ImageLoader.h"

// Default constructor
ImageSharpening::ImageSharpening() {
    kernel_height = 3;
    kernel_width = 3;
    blurring_kernel = new double * [kernel_height];
    for (int i = 0; i < kernel_height; ++i) {
        blurring_kernel[i] = new double [kernel_width];
        for (int j = 0; j < kernel_width; ++j) {
            blurring_kernel[i][j] = 1.0/9.0;
        }
    }

}

ImageSharpening::~ImageSharpening(){
        for (int i = 0; i < kernel_height; i++){
            delete[] blurring_kernel[i];
        }
        delete[]blurring_kernel;
}

ImageMatrix ImageSharpening::sharpen(const ImageMatrix& input_image, double k) {
    ImageMatrix inputimage(input_image);
    Convolution BlurringKernel(blurring_kernel,kernel_height,kernel_width,1, true);
    ImageMatrix BlurredImg(BlurringKernel.convolve(inputimage));
    ImageMatrix SharpImg(inputimage.operator-(BlurredImg).operator*(k).operator+(inputimage));
    for (int i = 0; i < SharpImg.get_height(); ++i) {
        for (int j = 0; j < SharpImg.get_width(); ++j) {
            if (SharpImg.get_data(i,j) < 0){
                SharpImg.set_data(i,j,0);
            }
            if (SharpImg.get_data(i,j) > 255){
                SharpImg.set_data(i,j,255);
            }
        }
    }
    return SharpImg;
}
