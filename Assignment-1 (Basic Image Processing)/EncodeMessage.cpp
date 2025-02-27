#include "EncodeMessage.h"
#include <vector>
#include <string>
#include <cmath>
#include <bitset>


// Default Constructor
EncodeMessage::EncodeMessage() {

}

// Destructor
EncodeMessage::~EncodeMessage() {
    
}

// Function to encode a message into an image matrix
ImageMatrix EncodeMessage::encodeMessageToImage(const ImageMatrix &img, const std::string &message, const std::vector<std::pair<int, int>>& positions) {
    int asciiValue;
    ImageMatrix encodedImage(img);
    std::string shiftedMessage;
    std::string tranformedMessage = message;
    std::string binaryMessage;
    int shiftAmount  = message.length()/2;
    for (int i = 0; i < message.length() && positions.size(); i++) {
        if (i >= 2) {
            int flag = 0;
            for (int j = 2; j <= sqrt(i); ++j) {
                if (i % j == 0) {
                    flag = 1;
                }
            }
            if (flag == 0) {
                int fibonacci = calculateFibonacci(i);
                asciiValue = static_cast<int>(tranformedMessage[i]);
                asciiValue += fibonacci;
                if (asciiValue <= 32) {
                    asciiValue += 33;
                }
                if (asciiValue >= 127) {
                    asciiValue = 126;
                }
                tranformedMessage[i] = static_cast<char>(asciiValue);
            }
        }
    }
    shiftAmount = shiftAmount % tranformedMessage.length();
    shiftedMessage = tranformedMessage.substr(tranformedMessage.length() - shiftAmount) + tranformedMessage.substr(0,tranformedMessage.length() - shiftAmount);
    for (char c : shiftedMessage) {
        std::string ReverseBinaryMessage;
        std::bitset<7> binaryChar(c);
        ReverseBinaryMessage = binaryChar.to_string();
        int n = ReverseBinaryMessage.length();
        for (int i = 0; i < n / 2; i++) {
            std::swap(ReverseBinaryMessage[i], ReverseBinaryMessage[n - i - 1]);
        }
        binaryMessage += ReverseBinaryMessage;
    }
    binaryMessage = binaryMessage.substr(0,positions.size());
    for(int i = 0; i < binaryMessage.length(); i++) {
        std::string binaryString;
        int value = encodedImage.get_data(positions[i].first, positions[i].second);
        std::bitset<8> binaryNumber(value);
        binaryString = binaryNumber.to_string();
        binaryString = binaryString.substr(0, binaryString.length() - 1) + binaryMessage[i];
        std::bitset<8> binaryStr(binaryString);
        int decimalValue = static_cast<int>(binaryStr.to_ulong());
        encodedImage.set_data(positions[i].first,positions[i].second,decimalValue);
    }
    return encodedImage;
}

int EncodeMessage:: calculateFibonacci(int n){
    if (n == 0){
        return 0;
    }else if (n == 1){
        return 1;
    } else{
        int nextFibonacci = calculateFibonacci(n-1) + calculateFibonacci(n -2);
        return nextFibonacci;
    }
}

