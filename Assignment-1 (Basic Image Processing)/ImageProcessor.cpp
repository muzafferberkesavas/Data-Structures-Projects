#include "ImageProcessor.h"

ImageProcessor::ImageProcessor() {

}

ImageProcessor::~ImageProcessor() {

}


std::string ImageProcessor::decodeHiddenMessage(const ImageMatrix &img) {
    ImageSharpening sharpener;
    ImageMatrix sharpenedImg = sharpener.sharpen(img, 2);
    EdgeDetector edgeDetector;
    std::vector<std::pair<int, int>> edgePixels = edgeDetector.detectEdges(sharpenedImg);
    DecodeMessage decoder;
    std::string decodedMessage = decoder.decodeFromImage(sharpenedImg,edgePixels);
    return decodedMessage;
}

ImageMatrix ImageProcessor::encodeHiddenMessage(const ImageMatrix &img, const std::string &message) {
    ImageMatrix encodedImg(img);
    ImageSharpening sharpener;
    ImageMatrix sharpenedImg = sharpener.sharpen(encodedImg,2);
    EdgeDetector edgeDetector;
    std::vector<std::pair<int, int>> edgePixels = edgeDetector.detectEdges(sharpenedImg);
    EncodeMessage encoder;
    encodedImg = encoder.encodeMessageToImage(img,message,edgePixels);
    return encodedImg;
}
