// DecodeMessage.cpp
#include "DecodeMessage.h"
#include "ImageMatrix.h"
#include "ImageLoader.h"
#include "ImageProcessor.h"

// Default constructor
DecodeMessage::DecodeMessage() {
    // Nothing specific to initialize here
}

// Destructor
DecodeMessage::~DecodeMessage() {
    // Nothing specific to clean up
}


std::string DecodeMessage::decodeFromImage(const ImageMatrix& image, const std::vector<std::pair<int, int>>& edgePixels) {
    std::string binaryString;
    std::string asciiString;
    int sizeofVector = edgePixels.size();
    if (sizeofVector % 7 != 0){
        int zerosNeeded = 7 - (sizeofVector % 7);
        for (int i = 0; i < zerosNeeded; ++i) {
            binaryString +='0';
        }
    }
    for (const auto& pixel : edgePixels){
        int pixelValue = image.get_data(pixel.first,pixel.second);
        int lsb = static_cast<int>(std::fmod(pixelValue, 2.0));
        binaryString += std::to_string(lsb);
    }
    for (int i = 0; i < binaryString.length(); i+=7) {
        std::string sevenbit = binaryString.substr(i,7);
        int asciiValue = std::stoi(sevenbit, nullptr,2);
        if (asciiValue <= 32) {
            asciiValue += 33;
        }
        if (asciiValue >= 127){
            asciiValue = 126;
        }
        asciiString += static_cast<char>(asciiValue);
    }
    return asciiString;
}

