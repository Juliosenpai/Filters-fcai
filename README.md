# Filters-fcai
#include <iostream>
#include "Image_Class.h" // Assuming this header file contains the Image class definition
using namespace std;
void BW(Image &image) {
    // Convert the loaded image to grayscale
    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            unsigned int intensity = 0.3 * image.getPixel(i, j, 0) +
                                     0.59 * image.getPixel(i, j, 1) +
                                     0.11 * image.getPixel(i, j, 2);

            // Set all channels to the calculated intensity to convert to grayscale
            for (int k = 0; k < image.channels; ++k) {
                image.setPixel(i, j, k, intensity);
            }
        }
    }

    // Define a threshold value for the black and white conversion
    unsigned int threshold = 128;

    // Convert the grayscale image to black and white
    for (int i = 0; i < image.width; ++i) {
        for (int j = 0; j < image.height; ++j) {
            unsigned int total = 0;
            for (int k = 0; k < image.channels; ++k) {
                total += image.getPixel(i, j, k);
            }

            // Calculate the average intensity by dividing the total by the number of channels
            unsigned int average = total / image.channels;

            // Determine if the pixel should be white or black based on the threshold
            if (average >= threshold) {
                // Set the pixel to white
                for (int k = 0; k < image.channels; ++k) {
                    image.setPixel(i, j, k, 255); // Set all channels to 255 (white)
                }
            } else {
                // Set the pixel to black
                for (int k = 0; k < image.channels; ++k) {
                    image.setPixel(i, j, k, 0); // Set all channels to 0 (black)
                }
            }
        }
    }

    // Save the black and white image
    image.saveImage("bwmine3.png");
}

int main() {

    Image image;
    string image_name;
    cin>>image_name;
    image.loadNewImage(image_name);


    // Convert the image to black and white
    BW(image);

    return 0;
}
