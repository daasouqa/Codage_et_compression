//
// Created by Ines Benghezal on 21/02/2019.
//

#include <cstdio>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include "ImageBase.cpp"

using namespace std;

int main(int argc, char **argv) {
    char cNomImgLue[250], cNomImgEcrite[250];
    int seuil;
    if (argc != 4) {
        printf("Usage: ImageIn.pgm ImageSeuilée.pgm seuil\n");
        return 1;
    }
    sscanf(argv[1], "%s", cNomImgLue);
    sscanf(argv[2], "%s", cNomImgEcrite);
    sscanf(argv[3], "%d", &seuil);
    
    ImageBase imageBase;
    imageBase.load(cNomImgLue);
    ImageBase out(imageBase.getWidth(), imageBase.getHeight(), imageBase.getColor());
    
    for (int i = 0; i < imageBase.getHeight(); ++i) {
        for (int j = 0; j < imageBase.getWidth(); ++j) {
            if (imageBase[i][j] < seuil) {
                out[i][j] = 0;
            } else {
                out[i][j] = 255;
            }
        }
    }
    
    out.save(cNomImgEcrite);
}