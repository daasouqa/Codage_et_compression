//
// Created by Inès Benghezal on 30/09/19.
//

#include "../Sources/ImageBase.h"
#include "../Sources/ImageBase.cpp"

int main(int argc, char* argv[]){
    char cNomImgLue[250], cNomImgEcrite[250];
    char redC[250] = "red.ppm";
    char greenC[250] = "green.ppm";
    char blueC[250] = "blue.ppm";
    if (argc != 3) {
        printf("Usage: ImageIn.ppm ImageOut.ppm\n");
        return 1;
    }
    sscanf(argv[1], "%s", cNomImgLue);
    sscanf(argv[2], "%s", cNomImgEcrite);
    
    ImageBase imageBase;
    imageBase.load(cNomImgLue);
    ImageBase red(imageBase.getWidth(), imageBase.getHeight(), imageBase.getColor());
    ImageBase green(imageBase.getWidth(), imageBase.getHeight(), imageBase.getColor());
    ImageBase blue(imageBase.getWidth(), imageBase.getHeight(), imageBase.getColor());
    
    for (int i = 0; i < imageBase.getWidth(); ++i) {
        for (int j = 0; j < imageBase.getHeight(); ++j) {
            green[i * 3][j * 3] = 0;
            green[i * 3][j * 3 + 1] = imageBase[i * 3][j * 3 + 1];
            green[i * 3][j * 3 + 2] = 0;
        }
    }
    
    for (int i = 0; i < imageBase.getWidth(); i += 1) {
        for (int j = 0; j < imageBase.getHeight(); j += 1) {
            red[i * 3][j * 3] = imageBase[i * 3][j * 3];
            red[i * 3][j * 3 + 1] = 0;
            red[i * 3][j * 3 + 2] = 0;
        }
    }
    
    for (int i = 0; i < imageBase.getWidth(); ++i) {
        for (int j = 0; j < imageBase.getHeight(); ++j) {
            blue[i * 3][j * 3] = 0;
            blue[i * 3][j * 3 + 1] = 0;
            blue[i * 3][j * 3 + 2] = imageBase[i * 3][j * 3 + 2];;
        }
    }
    
    red.save(redC);
    green.save(greenC);
    blue.save(blueC);
    
}