//
// Created by Ines Benghezal on 15/02/2019.
//

#include "ImageBase.cpp"
#include <cstdio>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include "../Sources/ImageBase.h"

using namespace std;

int main(int argc, char **argv) {
    char cNomImgLue[250], cNomImgEcrite[250];
    
    if (argc != 2) {
        printf("Usage: ImageIn.pgm\n");
        return 1;
    }
    sscanf(argv[1], "%s", cNomImgLue);
    
    ImageBase imageBase;
    imageBase.load(cNomImgLue);
    ofstream myfile;
    myfile.open("histogramme.dat");
    
    for (int i = 0; i < 256; ++i) {
        int occ = 0;
        for (int j = 0; j < imageBase.getHeight(); ++j) {
            for (int k = 0; k < imageBase.getWidth(); ++k) {
                if (imageBase[j][k] == i)
                    occ++;
            }
        }
        myfile << i << " " << occ <<endl;
    }
    myfile.close();
}