/*
====================== PROJECT TEAM =======================================
  ID          |        Name                  | Filter no. | Section
---------------------------------------------------------------------------
  20240515    | Mohamed Abdullah Abdul-Hafez | 1 - 4 - 7  | All-B
  20240649    | Hisham Mohamed Fathy         | 2 - 5      | 23
  20240692    | Youssef Sayed Ali            | 3 - 6      | 23
============================================================================
*/

#include <iostream>
#include <string>
#include <vector>
#include "Image_Class.h"

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
                avg /= image.channels;
                unsigned char val = (avg > 128) ? 255 : 0;
                for (int k = 0; k < image.channels; ++k) {
                    image(i, j, k) = val;
                }
            }
        }
    }

    //Filter #3
    void invertColors() {
        for (int i = 0; i < image.width; i++){
            for (int j = 0; j < image.height; j++) {
                for (int c = 0; c < image.channels; c++) {
                    image(i, j, c) = 255 - image(i, j, c);
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
                    unsigned char temp = image(i, j, k);
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

    // Filter #10
    void detectEdges() {
        Image gray = image;
        for (int i = 0; i < gray.width; ++i) {
            for (int j = 0; j < gray.height; ++j) {
                unsigned int avg = 0;
                for (int k = 0; k < gray.channels; ++k)
                    avg += gray(i, j, k);
                avg /= gray.channels;
                for (int k = 0; k < gray.channels; ++k)
                    gray(i, j, k) = avg;
            }
        }
        Image edges(gray.width, gray.height);
        for (int i = 1; i < gray.width - 1; ++i) {
            for (int j = 1; j < gray.height - 1; ++j) {
                int gx = 0, gy = 0;

                // تدرج أفقي (difference in x)
                gx = abs(gray(i+1, j, 0) - gray(i-1, j, 0));

                // تدرج رأسي (difference in y)
                gy = abs(gray(i, j+1, 0) - gray(i, j-1, 0));

                int edgeVal = min(255, gx + gy); // إجمالي الفرق
                for (int c = 0; c < gray.channels; ++c)
                    edges(i, j, c) = edgeVal;
            }
        }

        image = edges;
    }

    void saveImage(const string &outName) {
        image.saveImage(outName);
    }

private:
    Image image;
};


int main() {
    string inputFile;
    cout << "Enter photo name: ";
    cin >> inputFile;

    ImageProcessor p(inputFile);

    vector<string> menu{
        "1. Convert to grayscale",
        "2. Convert to black & white",
        "3. Invert colors",
        "4. Merge images",
        "5. Flip vertically",
        "6. Flip horizontally",
        "7. Rotate image",
        "8.Change Brightness",
        "9. Detect Edges",
        "12. Exit"
    };

    int choice;
    while (true) {
        cout << "\n===== MENU =====\n";
        for (auto &option : menu) cout << option << "\n";
        cout << "Choose a filter number: ";
        cin >> choice;

        if (choice == 12) { // Exit
            cout << "Exiting...\n";
            break;
        }

        switch (choice) {
            case 1: p.convertToGrayscale(); break;
            case 2: p.convertToBlackWhite(); break;
            case 3: p.invertColors(); break;
            case 4: {
                string secImage;
                cout << "Enter the name of the second image: ";
                cin >> secImage;
                p.mergeImages(secImage);
                break;
            }
            case 5: p.flipVertical(); break;
            case 6: p.flipHorizontal(); break;
            case 7: {
                int angle;
                cout << "Enter rotation angle (90, 180, 270): ";
                cin >> angle;
                p.rotate(angle);
                break;
            }
            case 8: {
                int x;
                cout << "1.darker\n2.lighter";
                cin >> x;
                (x == 1) ? p.ChangeBrightness(-127) : p.ChangeBrightness(127);
            }
            case 9: p.detectEdges(); break;
            default:
                cout << "Invalid choice\n";
                continue;
        }

        string outFile;
        cout << "Enter output file name: ";
        cin >> outFile;
        p.saveImage(outFile);

        cout << "\n--- Done! ---\n";
    }

    return 0;
}
