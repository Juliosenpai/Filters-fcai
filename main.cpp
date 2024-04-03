#include <iostream>
#include "Image_Class.h"
#include <string>

using namespace std;

Image save_photo(Image &image)
{
    string save, save_type;

    cout << "What is the name you want to save the image with? ";
    cin >> save;

    while (true)
    {
        cout << "What is the type you want to save the image with? ";
        cin >> save_type;
        if (save_type == "png" || save_type == "jpg" || save_type == "jpeg" || save_type == "bmp")
        {
            break; // Exit the loop if the type is valid
        }
        else
        {
            cout << "Invalid image type. Please enter one of: png, jpg, jpeg, bmp" << endl;
        }
    }

    save = save + "." + save_type;
    image.saveImage(save);
}
Image merge(Image &firstImage)
{
    cout << "__Welcome to merge Filter__" << endl;

    string name2;
    string outputName;

    string image_name;
    string type;
    Image secondImage;
    cout << "Please enter the name of the second image you want to merge: ";
    cin >> image_name;
    cout << "Please enter the extension for image: ";
    cin >> type;
    name2 = image_name + "." + type;
    // inputting the second image you want to merge
    while (true)
    {
        try
        {
            secondImage.loadNewImage(name2);
            break;
        }
        catch (invalid_argument)
        {
            cout << "__Please enter a valid photo__\n"
                 << endl;
            cout << "Please enter name for image: ";
            cin >> image_name;
            cout << "Please enter type for image: ";
            cin >> type;
            name2 = image_name + "." + type;
        }
    }

    int minWidth = min(firstImage.width, secondImage.width);
    int minHeight = min(firstImage.height, secondImage.height);
    // setting the greater image to the smaller image size while maintaining dimensions
    if (secondImage.width != firstImage.width || secondImage.height != firstImage.height)
    {
        Image resizedSecondImage(firstImage.width, firstImage.height);
        for (int i = 0; i < firstImage.width; i++)
        {
            for (int j = 0; j < firstImage.height; j++)
            {
                for (int k = 0; k < 3; k++)
                {
                    resizedSecondImage(i, j, k) = secondImage(i * secondImage.width / firstImage.width, j * secondImage.height / firstImage.height, k);
                }
            }
        }
        secondImage = resizedSecondImage;
    }
    // creating a new image with the smaller dimensions
    Image smallerImage(firstImage.width, firstImage.height);

    for (int i = 0; i < smallerImage.width; i++)
    {
        for (int j = 0; j < smallerImage.height; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                // getting the average between two images
                smallerImage(i, j, k) = (secondImage(i, j, k) + firstImage(i, j, k)) / 2;
            }
        }
    }
    // save image
    save_photo(smallerImage);
    return smallerImage;
}
Image darken(Image &firstImage)
{
    cout << "Welcome to the darkening filter" << endl;
    // looping on image pixels and channels
    for (int i = 0; i < firstImage.width; i++)
    {
        for (int j = 0; j < firstImage.height; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                // divide pixel value by 1.5 and choosing 0 if the pixel value is less than tat
                int newValue = firstImage(i, j, k) / 1.5;
                firstImage(i, j, k) = max(newValue, 0);
            }
        }
    }
    // saving images
    save_photo(firstImage);
    return firstImage;
};

Image greyScale(Image &firstImage)

{
    Image grayscaleImage(firstImage.width, firstImage.height);
    cout << "Welcome to the greyScale filter" << endl;
    // looping on image pixels and channels
    for (int i = 0; i < firstImage.width; i++)
    {
        for (int j = 0; j < firstImage.height; j++)
        {
            // Calculate luminance using this specific formula
            unsigned char luminance = static_cast<unsigned char>(
                0.2126 * firstImage(i, j, 0) + 0.7152 * firstImage(i, j, 1) + 0.0722 * firstImage(i, j, 2));

            // Set the grayscale pixel to the luminance value
            grayscaleImage(i, j, 0) = grayscaleImage(i, j, 1) = grayscaleImage(i, j, 2) = luminance;
        }
    }
    save_photo(grayscaleImage);
    return grayscaleImage;
};
Image lighten(Image &firstImage)
{

    cout << "__Welcome to lighten Filter__" << endl;
    // looping on image pixels and channels
    for (int i = 0; i < firstImage.width; i++)
    {
        for (int j = 0; j < firstImage.height; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                // multiply pixel value by 1.5 and choosing 255 if pixel is more than that
                int newValue = firstImage(i, j, k) * 1.5;
                firstImage(i, j, k) = min(newValue, 255);
            }
        };
    }
    // saving image
    save_photo(firstImage);
    return firstImage;
}
Image sobelBlackToWhite(Image &inputImage)
{
    for (int i = 0; i < inputImage.width; i++)
    {
        for (int j = 0; j < inputImage.height; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                inputImage(i, j, k) = (inputImage(i, j, k) == 0) ? 255 : 0;
            }
        };
    }
    // saving image
    save_photo(inputImage);
    return inputImage;
}
void sobelEdgeDetection(Image &inputImage)
{
    Image outputImage(inputImage.width, inputImage.height);
    int Xoperator[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Yoperator[3][3] = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};
    unsigned int maxPixelValue = 0;
    for (int i = 0; i < inputImage.width; i++)
    {
        for (int j = 0; j < inputImage.height; j++)
        {
            for (int k = 0; k < inputImage.channels; k++)
            {
                maxPixelValue = max(maxPixelValue, static_cast<unsigned int>(inputImage(i, j, k)));
            }
        }
    }

    // Calculate the dynamic threshold as a percentage of the maximum pixel value

    for (int i = 1; i < inputImage.width - 1; i++)
    {
        for (int j = 1; j < inputImage.height - 1; j++)
        {
            int xVal = 0;
            int yVal = 0;

            for (int k = -1; k <= 1; k++)
            {
                for (int l = -1; l <= 1; l++)
                {
                    int intensity = inputImage(i + k, j + l, 0);

                    xVal += Xoperator[k + 1][l + 1] * inputImage(i + k, j + l, 0);
                    yVal += Yoperator[k + 1][l + 1] * inputImage(i + k, j + l, 0);
                }
            }
            int threshold = sqrt((pow(abs(xVal), 2)) + (pow(abs(yVal), 2)));

            int angle = atan2(yVal, xVal) * (180.0 / M_PI);
            angle += 180;

            int pixel1, pixel2;
            if ((angle >= 0 && angle < 22.5) || (angle >= 157.5 && angle <= 180))
            {
                pixel1 = inputImage(i, j - 1, 0);
                pixel2 = inputImage(i, j + 1, 0);
            }
            else if (angle >= 22.5 && angle < 67.5)
            {
                pixel1 = inputImage(i - 1, j + 1, 0);
                pixel2 = inputImage(i + 1, j - 1, 0);
            }
            else if (angle >= 67.5 && angle < 112.5)
            {
                pixel1 = inputImage(i - 1, j, 0);
                pixel2 = inputImage(i + 1, j, 0);
            }
            else if (angle >= 112.5 && angle < 157.5)
            {
                pixel1 = inputImage(i - 1, j - 1, 0);
                pixel2 = inputImage(i + 1, j + 1, 0);
            }

            // Perform Non-Maximum Suppression
            if (threshold > pixel1 && threshold > pixel2)
            {
                outputImage(i, j, 0) = outputImage(i, j, 1) = outputImage(i, j, 2) = threshold;
            }
            else
            {
                outputImage(i, j, 0) = outputImage(i, j, 1) = outputImage(i, j, 2) = 0;
            }
        }
    }
    sobelBlackToWhite(outputImage);
}

void edgeDetect(Image &inputImage)
{
    Image grayscaleImage(inputImage.width, inputImage.height);
    cout << "Welcome to the edge detection filter" << endl;
    // looping on image pixels and channels
    for (int i = 0; i < inputImage.width; i++)
    {
        for (int j = 0; j < inputImage.height; j++)
        {
            // Calculate luminance using this specific formula
            unsigned char luminance = static_cast<unsigned char>(
                0.299 * inputImage(i, j, 0) + 0.587 * inputImage(i, j, 1) + 0.114 * inputImage(i, j, 2));

            // Set the grayscale pixel to the luminance value
            grayscaleImage(i, j, 0) = grayscaleImage(i, j, 1) = grayscaleImage(i, j, 2) = luminance;
        }
    }
    sobelEdgeDetection(inputImage);
}
Image crop(Image &image)
{
    int x, z, a, b;
    cout << "enter the dimensions of the new photo (W then H)" << endl;
    cin >> x >> z;
    cout << "enter the pixel you want to start from (W then H)" << endl;
    cin >> a >> b;
    Image image2(x, z);

    // Copying lower half of image to image2
    for (int i = 0; i < image2.width; ++i)
    {
        for (int j = 0; j < image2.height; ++j)
        {
            for (int k = 0; k < image.channels; ++k)
            {
                // Accessing pixels within bounds of image
                unsigned int temp = image.getPixel(i, j, k);
                image2.setPixel(i, j, k, image.getPixel(a + i, b + j, k));
            }
        }
    }

    // Save the cropped image
    save_photo(image2);
    return image2;
}

Image BW(Image &image)
{
    // Convert the loaded image to grayscale
    for (int i = 0; i < image.width; ++i)
    {
        for (int j = 0; j < image.height; ++j)
        {
            unsigned int intensity = 0.3 * image.getPixel(i, j, 0) +
                                     0.59 * image.getPixel(i, j, 1) +
                                     0.11 * image.getPixel(i, j, 2);

            // Set all channels to the calculated intensity to convert to grayscale
            for (int k = 0; k < image.channels; ++k)
            {
                image.setPixel(i, j, k, intensity);
            }
        }
    }

    // Define a threshold value for the black and white conversion
    unsigned int threshold = 128;

    // Convert the grayscale image to black and white
    for (int i = 0; i < image.width; ++i)
    {
        for (int j = 0; j < image.height; ++j)
        {
            unsigned int total = 0;
            for (int k = 0; k < image.channels; ++k)
            {
                total += image.getPixel(i, j, k);
            }

            // Calculate the average intensity by dividing the total by the number of channels
            unsigned int average = total / image.channels;

            // Determine if the pixel should be white or black based on the threshold
            if (average >= threshold)
            {
                // Set the pixel to white
                for (int k = 0; k < image.channels; ++k)
                {
                    image.setPixel(i, j, k, 255); // Set all channels to 255 (white)
                }
            }
            else
            {
                // Set the pixel to black
                for (int k = 0; k < image.channels; ++k)
                {
                    image.setPixel(i, j, k, 0); // Set all channels to 0 (black)
                }
            }
        }
    }
    save_photo(image);
    return image;
}
Image Invert_Image(Image &image)
{
    for (int i = 0; i < image.width; ++i)
    {
        for (int j = 0; j < image.height; ++j)
        {
            for (int k = 0; k < image.channels; ++k)
            {
                // Calculate negative value for each channel
                int neg_value = 255 - image(i, j, k);
                image(i, j, k) = neg_value;
            }
        }
    }
    save_photo(image);
    return image;
}

Image flip_90(Image &image)
{
    Image flippedImage(image.height, image.width);
    for (int i = 0; i < image.width; ++i)
    {
        for (int j = 0; j < image.height; ++j)
        {
            for (int k = 0; k < image.channels; ++k)
            {
                flippedImage(j, image.width - i - 1, k) = image(i, j, k);
            }
        }
    }
    save_photo(flippedImage);
    return flippedImage;
}

// Function to flip the image by 180 degrees
Image flip_180(Image &image)
{
    Image flippedImage(image.width, image.height); // Create a new image to store the flipped result

    // Flip the image 180 degrees
    for (int i = 0; i < image.width; ++i)
    {
        for (int j = 0; j < image.height; ++j)
        {
            for (int k = 0; k < image.channels; ++k)
            {
                // Assign the pixel value from the transposed position to the flipped image
                flippedImage(image.width - i - 1, image.height - j - 1, k) = image(i, j, k);
            }
        }
    }
    save_photo(flippedImage);
    return flippedImage;
}

// Function to flip the image by 270 degrees
Image flip_270(Image &image)
{
    Image flippedImage(image.height, image.width); // Create a new image to store the flipped result

    // Flip the image 270 degrees
    for (int i = 0; i < image.width; ++i)
    {
        for (int j = 0; j < image.height; ++j)
        {
            for (int k = 0; k < image.channels; ++k)
            {
                // Assign the pixel value from the transposed position to the flipped image
                flippedImage(image.height - j - 1, i, k) = image(i, j, k);
            }
        }
    }
    save_photo(flippedImage);
    return flippedImage;
}

void applyFilter(Image &image, int filterChoice)
{
    int answer;
    switch (filterChoice)
    {
    case 1:
        // Grayscale Conversion
        greyScale(image);
        break;
    case 2:
        BW(image);
        break;
    case 3:
        // Invert Image
        Invert_Image(image);
        break;
    case 4:
        // Merge images
        merge(image);
        // Implement image merging
        break;
    case 5:
        // crop Images
        crop(image);
        // Implement image cropping
        break;
    case 6:
        cout << "How many degrees do you want to rotate the image ?\n1-90\n2-180\n3-270\n";
        cin >> answer;
        while (!(answer > 0 && answer < 4))
        {
            cout << "Please enter a valid input: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin >> answer;
        }
        if (answer == 1)
        {
            flip_90(image);
        }
        else if (answer == 2)
        {
            flip_180(image);
        }
        else
        {
            flip_270(image);
        }
        break;
    case 7:
        // darken or lighten image
        int darkOrLight;
        cout << "Choose filter\n1-Darken\n2-lighten\n: ";
        cin >> darkOrLight;
        while (!(darkOrLight > 0 && darkOrLight < 3))
        {
            cout << "Please enter a valid input: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin >> darkOrLight;
        }
        if (darkOrLight == 1)
        {
            darken(image);
        }
        else if (darkOrLight == 2)
        {
            lighten(image);
        }
        break;
    case 8:
        edgeDetect(image);
        break;
    default:
        cout << "Invalid filter choice. Please try again." << endl;
    }
}
void menu()
{
    string name;
    int choice, no_filter;
    Image image;

    cout << "What's your name? ";
    getline(cin, name);
    cout << "Hello Dr, " << name << endl;

    while (true)
    {
        cout << "What do you want to do?" << endl;
        cout << "1. Load Image" << endl;
        cout << "2. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        while (!(choice > 0 && choice < 3))
        {
            cout << "Please enter a valid input: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin >> choice;
        }

        if (choice == 1)
        {
            string first_name, type, image_name;
            cout << "Please enter name for image: ";
            cin >> first_name;
            cout << "Please enter type for image: ";
            cin >> type;
            image_name = first_name + "." + type;
            while (true)
            {
                try
                {
                    image.loadNewImage(image_name);
                    break;
                }
                catch (invalid_argument)
                {
                    cout << "__Please enter a valid photo__\n"
                         << endl;
                    cout << "Please enter name for image: ";
                    cin >> first_name;
                    cout << "Please enter type for image: ";
                    cin >> type;
                    image_name = first_name + "." + type;
                }
            }

            while (true)
            {
                cout << "1. Apply Filter" << endl;
                cout << "2. Save Image" << endl;
                cout << "3. Return to Main Menu" << endl;
                cout << "Enter your choice: ";
                cin >> choice;

                while (!(choice > 0 && choice < 4))
                {
                    cout << "Please enter a valid input: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cin >> choice;
                }

                if (choice == 1)
                {
                    cout << "You want me to filter" << endl;
                    cout << "1-Grayscale Conversion\n2-Black and White \n3-Invert Image\n4-merge Images\n5-crop Images\n6-Rotate Image\n7-darken or lighten\n8-detect edges\n: ";
                    cin >> no_filter;
                    while (!(no_filter > 0 && no_filter < 9))
                    {
                        cout << "Please enter a valid input: ";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cin >> no_filter;
                    }
                    applyFilter(image, no_filter);
                }
                else if (choice == 2)
                {
                    save_photo(image);
                }
                else
                {
                    // Return to the main menu
                    break;
                }
            }
        }
        else
        {
            // Exit the loop
            break;
        }
    }
}
int main()
{
    menu();
    return 0;
}