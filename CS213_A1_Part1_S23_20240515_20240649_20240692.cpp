/*
====================== PROJECT TEAM =======================================
  ID          |        Name                  | Filter no.     | Section
---------------------------------------------------------------------------
  20240515    | Mohamed Abdullah Abdul-Hafez | 1 - 4 - 7 - 10 | All-B
  20240649    | Hisham Mohamed Fathy         | 2 - 5 - 8 - 11 | 35
  20240692    | Youssef Sayed Ali            | 3 - 6 - 9 - 12 | 25
============================================================================
*/

#include <iostream>
#include <string>
#include <vector>
#include "Image_Class.h"
#include <cstdlib>

using namespace std;

class ImageProcessor {
public:
    ImageProcessor(const string &filename) : image(filename) {}

    //Filter #1
    void convertToGrayscale() {
        for (int i = 0; i < image.width; ++i) {
            for (int j = 0; j < image.height; ++j) {
                unsigned int avg = 0;
                for (int k = 0; k < 3; ++k) avg += image(i, j, k);
                avg /= 3;
                image(i, j, 0) = avg;
                image(i, j, 1) = avg;
                image(i, j, 2) = avg;
            }
        }
    }

    //Filter #2
    void convertToBlackWhite() {
        for (int i = 0; i < image.width; ++i) {
            for (int j = 0; j < image.height; ++j) {
                unsigned int avg = 0;
                for (int k = 0; k < image.channels; ++k) avg += image(i, j, k);
                avg /= 3;
                unsigned int val = (avg > 128) ? 255 : 0;
                for (int k = 0; k < image.channels; ++k) image(i, j, k) = val;
            }
        }
    }

    //Filter #3
    void invertColors() {
        for (int i = 0; i < image.width; i++){
            for (int j = 0; j < image.height; j++) {
                for (int k = 0; k < image.channels; k++) {
                    image(i, j, k) = 255 - image(i, j, k);
                }
            }
        }
    }

    //Filter #4
    void mergeImages(const string &filename2) {
        Image img2(filename2);
        int newWidth = min(image.width, img2.width);
        int newHeight = min(image.height, img2.height);
        Image newImage(newWidth, newHeight);
        bool bl = true;
        for (int y = 0; y < newHeight; y++) {
            for (int x = 0; x < newWidth; x++) {
                for (int c = 0; c < img2.channels; ++c) {
                    if (bl) {
                        newImage.setPixel(x, y, c, img2.getPixel(x, y, c));
                        bl = false;
                    } else {
                        newImage.setPixel(x, y, c, image.getPixel(x, y, c));
                        bl = true;
                    }
                }
            }
        }
        image = newImage;
    }

    //Filter #5
    void flipVertical() {
        for (int j = 0; j < image.height / 2; ++j) {
            for (int i = 0; i < image.width; ++i) {
                for (int k = 0; k < image.channels; ++k) {
                    int oppositeY = image.height - 1 - j;
                    unsigned int temp = image(i, j, k);
                    image(i, j, k) = image(i, oppositeY, k);
                    image(i, oppositeY, k) = temp;
                }
            }
        }
    }
    void flipHorizontal() {
        for (int j = 0; j < image.height; ++j) {
            for (int i = 0; i < image.width / 2; ++i) {
                for (int k = 0; k < image.channels; ++k) {
                    int oppositeX = image.width - 1 - i;
                    unsigned char temp = image(i, j, k);
                    image(i, j, k) = image(oppositeX, j, k);
                    image(oppositeX, j, k) = temp;
                }
            }
        }
    }

    //Filter #6
    void rotate(int angle) {
        Image out;
        if (angle == 90) {
            out = Image(image.height, image.width);
            for (int i = 0; i < image.width; i++) {
                for (int j = 0; j < image.height; j++) {
                    for (int c = 0; c < image.channels; c++) {
                        out(j, image.width - 1 - i, c) = image(i, j, c);
                    }
                }
            }
        } else if (angle == 180) {
            out = Image(image.width, image.height);
            for (int i = 0; i < image.width; i++) {
                for (int j = 0; j < image.height; j++) {
                    for (int c = 0; c < image.channels; c++) {
                        out(image.width - 1 - i, image.height - 1 - j, c) = image(i, j, c);
                    }
                }
            }
        } else if (angle == 270) {
            out = Image(image.height, image.width);
            for (int i = 0; i < image.width; i++) {
                for (int j = 0; j < image.height; j++) {
                    for (int c = 0; c < image.channels; c++) {
                        out(image.height - 1 - j, i, c) = image(i, j, c);
                    }
                }
            }
        }
        image = out;
    }
    // Filter #7
    void ChangeBrightness(int n) {
        for (int i = 0; i < image.width; ++i) {
            for (int j = 0; j < image.height; ++j) {
                for (int k = 0; k < 3; ++k) {
                    int x = image(i,j,k) + n;
                    if (x > 255)x = 255;
                    if (x < 0)x = 0;
                    image(i,j,k) = x;
                }
            }
        }
    }
    // Filter #8
    void cropImage() {
        int x, y, width, height;
        cout << "Enter crop start X: "; cin >> x;
        cout << "Enter crop start Y: "; cin >> y;
        cout << "Enter crop width: "; cin >> width;
        cout << "Enter crop height: "; cin >> height;

        if (x + width > image.width || y + height > image.height) {
            cout << "Error: crop area exceeds image bounds!" << endl;
            return;
        }

        Image cropped(width, height);
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                for (int k = 0; k < image.channels; k++) {
                    cropped(i, j, k) = image(i + x, j + y, k);
                }
            }
        }
        image = cropped;
    }
    // Filter #9
    void addFrame(int thickness, int r = 0, int g = 0, int b = 0) {
        for (int i = 0; i < image.width; i++)
            for (int j = 0; j < image.height; j++) {
                bool border = (i < thickness || j < thickness || i >= image.width - thickness || j >= image.height - thickness);
                if (border) {
                    image(i, j, 0) = r;
                    image(i, j, 1) = g;
                    image(i, j, 2) = b;
                }
            }
    }

    // Filter #10
    void detectEdges() {
        convertToGrayscale();
        Image edges(image.width, image.height);
        for (int i = 1; i < image.width - 1; ++i) {
            for (int j = 1; j < image.height - 1; ++j) {
                int gx = 0, gy = 0;
                gx = abs(image(i+1, j, 0) - image(i-1, j, 0));
                gy = abs(image(i, j+1, 0) - image(i, j-1, 0));
                int edgeVal = min(255, gx + gy);
                for (int k = 0; k < image.channels; ++k)
                    edges(i, j, k) = 255 - edgeVal;
            }
        }
        image = edges;
    }
    // Filter #11
    void resizeImage() {
        int newWidth, newHeight;
        cout << "Enter new width and height: ";
        cin >> newWidth >> newHeight;

        Image resized(newWidth, newHeight);
        for (int i = 0; i < newWidth; i++) {
            for (int j = 0; j < newHeight; j++) {
                int oldX = i * image.width / newWidth;
                int oldY = j * image.height / newHeight;
                for (int k = 0; k < image.channels; k++)
                    resized(i, j, k) = image(oldX, oldY, k);
            }
        }
        image = resized;
    }
    // Filter #12
    void blurImages(int blockSize) {
        for (int y = 0; y < image.height; y += blockSize) {
            for (int x = 0; x < image.width; x += blockSize) {
                int rSum = 0, gSum = 0, bSum = 0;
                int count = 0;

                for (int j = y; j < y + blockSize && j < image.height; j++) {
                    for (int i = x; i < x + blockSize && i < image.width; i++) {
                        rSum += image(i, j, 0);
                        gSum += image(i, j, 1);
                        bSum += image(i, j, 2);
                        count++;
                    }
                }

                int rAvg = rSum / count;
                int gAvg = gSum / count;
                int bAvg = bSum / count;

                for (int j = y; j < y + blockSize && j < image.height; j++) {
                    for (int i = x; i < x + blockSize && i < image.width; i++) {
                        image(i, j, 0) = rAvg;
                        image(i, j, 1) = gAvg;
                        image(i, j, 2) = bAvg;
                    }
                }
            }
        }
    }
    void saveImage(const string &outName) {
        image.saveImage(outName);
    }

private:
    Image image;
};

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int main() {
    string inputFile;
    cout << "Enter photo name: ";
    cin >> inputFile;

    vector<string> menu{
        "1. Convert to grayscale",
        "2. Convert to black & white",
        "3. Invert colors",
        "4. Merge images",
        "5. Flip",
        "6. Rotate image",
        "7. Change Brightness",
        "8. Crop image",
        "9. Add frame",
        "10. Detect Edges",
        "11. Resize image",
        "12. Blur image",
        "13. Exit"
    };

    int choice;
    while (true) {
        clearScreen();  

        cout << "===== MENU =====\n";
        for (auto &option : menu)
            cout << option << "\n";
        cout << "Choose a filter number: ";
        cin >> choice;

        if (choice == 13) {
            cout << "Exiting...\n";
            break;
        }

        ImageProcessor imageProcessor(inputFile);

        switch (choice) {
            case 1: imageProcessor.convertToGrayscale(); break;
            case 2: imageProcessor.convertToBlackWhite(); break;
            case 3: imageProcessor.invertColors(); break;
            case 4: {
                string secImage;
                cout << "Enter the name of the second image: ";
                cin >> secImage;
                imageProcessor.mergeImages(secImage);
                break;
            }
            case 5: {
                cout << "1. Flip vertically\n2. Flip horizontally\nEnter the type: ";
                int x; cin >> x;
                (x == 1) ? imageProcessor.flipVertical() : imageProcessor.flipHorizontal();
                break;
            }
            case 6: {
                int angle;
                cout << "Enter rotation angle (90, 180, 270): ";
                cin >> angle;
                imageProcessor.rotate(angle);
                break;
            }
            case 7: {
                int x;
                cout << "1. Darker\n2. Lighter\nEnter your choice: ";
                cin >> x;
                (x == 1) ? imageProcessor.ChangeBrightness(-127) : imageProcessor.ChangeBrightness(127);
                break;
            }
            case 8: imageProcessor.cropImage(); break;
            case 9: {
                cout << "Enter the thickness: ";
                int x; cin >> x;
                cout << "Enter the color (R G B): ";
                int r, g, b; cin >> r >> g >> b;
                imageProcessor.addFrame(x, r, g, b);
                break;
            }
            case 10: imageProcessor.detectEdges(); break;
            case 11: imageProcessor.resizeImage(); break;
            case 12: {
                cout << "Enter blur block size: ";
                int x; cin >> x;
                imageProcessor.blurImages(x);
                break;
            }
            default:
                cout << "Invalid choice\n";
                continue;
        }

        string outFile;
        cout << "Enter output file name: ";
        cin >> outFile;
        imageProcessor.saveImage(outFile);

        cout << "\n--- Done! ---\n";
        cout << "Press Enter to return to the menu...";
        cin.ignore();
        cin.get();
    }

    return 0;
}
