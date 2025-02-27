#include "ImageMatrix.h"
#include "ImageLoader.h"


// Default constructor
ImageMatrix::ImageMatrix(){
    data = new double * [3];
    for (int i = 0; i < 3;i++){
        data[i] = new double[3];
        for (int j = 0; j < 3; j++){
            data[i][j] = 0;
        }
    }
    height = 3;
    width = 3;
}


// Parameterized constructor for creating a blank image of given size
ImageMatrix::ImageMatrix(int imgHeight, int imgWidth) : height(imgHeight), width(imgWidth) {
    data = new double * [height];
    for (int i = 0;i < height; i++){
        data[i] = new double[width];
        for (int j = 0; j < width; j++){
            data[i][j] = 0;
        }
    }
}

// Parameterized constructor for loading image from file. PROVIDED FOR YOUR CONVENIENCE
ImageMatrix::ImageMatrix(const std::string &filepath) {
    // Create an ImageLoader object and load the image
    ImageLoader imageLoader(filepath);

    // Get the dimensions of the loaded image
    height = imageLoader.getHeight();
    width = imageLoader.getWidth();

    // Allocate memory for the matrix
    data = new double*[height];
    for (int i = 0; i < height; ++i) {
        data[i] = new double[width];
    }

    // Copy data from imageLoader to data
    double** imageData = imageLoader.getImageData();
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; j++) {
            data[i][j] = imageData[i][j];
        }
    }

}



// Destructor
ImageMatrix::~ImageMatrix() {
        for (int i = 0; i < height; i++){
            delete[] data[i];
        }
        delete[]data;
    }


// Parameterized constructor - direct initialization with 2D matrix
ImageMatrix::ImageMatrix(const double** inputMatrix, int imgHeight, int imgWidth){
    height = imgHeight;
    width = imgHeight;
    data = new double*[imgHeight];
    for (int i = 0; i < imgHeight; ++i) {
        data[i] = new double[imgWidth];
        for (int j = 0; j < imgWidth; ++j) {
            data[i][j] = inputMatrix[i][j];
        }
    }
}

// Copy constructor
ImageMatrix::ImageMatrix(const ImageMatrix &other) {
    height = other.height;
    width = other.width;
    data = new double * [height];
    for (int i = 0; i < height; i++){
        data[i] = new double [width];
        for (int j = 0; j < width; j++){
            data[i][j] = other.data[i][j];
        }
    }
    
}

// Copy assignment operator
ImageMatrix& ImageMatrix::operator=(const ImageMatrix &other) {
    if (this == &other){
        return *this;
    }
    for (int i = 0; i < height; ++i) {
        delete[] data[i];
    }
    delete[] data;

    height = other.height;
    width = other.width;

    data = new double * [height];
    for (int i = 0; i < height; i++){
        data[i] = new double[width];
        for (int j = 0; j < width; j++){
            data[i][j] =other.data[i][j];
        }
    }
    return *this;
}



// Overloaded operators

// Overloaded operator + to add two matrices
ImageMatrix ImageMatrix::operator+(const ImageMatrix &other) const {
    ImageMatrix result(height,width);
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            result.data[i][j] = data[i][j] + other.data[i][j];
        }
    }
    return result;
}

// Overloaded operator - to subtract two matrices
ImageMatrix ImageMatrix::operator-(const ImageMatrix &other) const {
    ImageMatrix result(height,width);
    for (int i = 0; i < height;i++){
        for (int j = 0; j < width; j++){
            result.data[i][j] = data[i][j] - other.data[i][j];
        }
    }
    return result;
}

// Overloaded operator * to multiply a matrix with a scalar
ImageMatrix ImageMatrix::operator*(const double &scalar) const {
    ImageMatrix result(height,width);
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; ++j) {
            result.data[i][j] = data[i][j] * scalar;
        }
    }
    return result;
}


// Getter function to access the data in the matrix
double** ImageMatrix::get_data() const {
    return data;
}

// Getter function to access the data at the index (i, j)
double ImageMatrix::get_data(int i, int j) const {
    if (i >= 0 && i < height && j >= 0 && j < width){
        return data[i][j];
    } else{
        return 0;
    }
}

int ImageMatrix::get_height() const {
    return height;
}
int ImageMatrix::get_width() const {
    return width;
}
void ImageMatrix::set_data(int i, int j, double n){
    data[i][j] = n;
}
