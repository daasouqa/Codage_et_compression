//
// Created by Inès Benghezal on 14/10/19.
//
//
// Copyright (c) 2019 Inès Benghezal. All rights reserved.
//

#include <iostream>
#include <vector>


#include "./ImageBase.cpp"

using namespace std;

ImageBase PRNG(int n, int width, int height){
    ImageBase binarySequence(width, height, false);
    
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            double randomNumber = rand() % n;
            if (randomNumber < n / 2) {
                binarySequence[i][j] = 0;
            } else {
                binarySequence[i][j] = 255;
            }
        }
    }
    
    binarySequence.save("BinarySequence.pgm");
    
    return binarySequence;
}

ImageBase XOR(ImageBase ImgIn, ImageBase prgn, char* name){
    ImageBase result(ImgIn.getWidth(), ImgIn.getHeight(), false);
    
    for (int i = 0; i < ImgIn.getHeight(); ++i) {
        for (int j = 0; j < ImgIn.getWidth(); ++j) {
            result[i][j] = ImgIn[i][j] ^ prgn[i][j];
        }
    }
    
    result.save(name);
    
    return result;
}

double entropy(ImageBase ImgIn){
    double alpha[256] = {0};
    double h = 0;
    
    for(int i=0; i<ImgIn.getHeight(); i++){
        for(int j=0; j<ImgIn.getWidth(); j++){
            int index = ImgIn[i][j];
            alpha[index]++;
        }
    }
    
    double palpha[256];
    
    for(int i=0; i<256; i++){
        palpha[i] = alpha[i] / 256;
    }
    
    for(int i=0; i<256; i++){
        h+= palpha[i] * log2(palpha[i]);
    }
    
    return -h;
}

int main(int argc, char* argv[]) {
    
    
    char cNomImgLue[250], cNomImgEcrite[250];
    int nH, nW, nTaille;
    if (argc != 3) {
        printf("Usage: ImageIn.pgm ImageOut.pgm\n");
        exit(1);
    }
    
    sscanf(argv[1], "%s", cNomImgLue);
    sscanf(argv[2], "%s", cNomImgEcrite);
    
    ImageBase ImgIn;
    ImgIn.load(cNomImgLue);
    
    int n = 80;
    
    ImageBase prgnSeq = PRNG(n, ImgIn.getWidth(), ImgIn.getHeight());
    
//    ImageBase ImgOut = XOR(ImgIn, prgnSeq, "chiffrement.pgm");
    
    ImageBase reconstruction = XOR(XOR(ImgIn, prgnSeq, "chiffrement.pgm"), prgnSeq, "reconstruction.pgm");
    
    

}