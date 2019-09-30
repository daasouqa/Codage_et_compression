//
// Created by Inès Benghezal on 30/09/19.
//
// Copyright (c) 2019 Inès Benghezal. All rights reserved.
//

#include <iostream>
#include <vector>


#include "../Sources/ImageBase.cpp"

using namespace std;

struct color{
    OCTET r;
    OCTET g;
    OCTET b;
};

double distanceColor(int red1, int green1, int blue1, int red2, int green2, int blue2) {
    return sqrt(pow(red1 - red2, 2) + pow(green1 - green2, 2) + pow(blue1 - blue2, 2));
}

double PSNR(OCTET* a, OCTET* b,int taille3){
    double EQM = 0;
    for (int i = 0; i < taille3; i += 3) {
        EQM += pow(distanceColor(a[i], a[i + 1], a[i + 2], b[i], b[i + 1], b[i + 2]),2);
    }
    EQM = EQM / (taille3 / 3);
    return (10 * log10(pow(255, 2) / EQM));
}


int main(int argc, char* argv[]) {
    
    
    char cNomImgLue[250], cNomImgEcrite[250];
    int nH, nW, nTaille;
    if (argc != 3) {
        printf("Usage: ImageIn.ppm ImageOut.ppm\n");
        exit(1);
    }
    
    
    
    /// Lecture de l'image
    
    sscanf(argv[1], "%s", cNomImgLue);
    sscanf(argv[2], "%s", cNomImgEcrite);
    
    OCTET *ImgIn, *ImgOut;
    
    nTaille = nH * nW;
    
    int nTaille3 = nTaille * 3;
    allocation_tableau(ImgIn, OCTET, nTaille3);
    lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
    allocation_tableau(ImgOut, OCTET, nTaille3);
    
    OCTET *RED, *GREEN, *BLUE;
    
//    allocation_tableau(RED, OCTET, nTaille3/4);
    allocation_tableau(GREEN, OCTET, nTaille3);
//    allocation_tableau(BLUE, OCTET, nTaille3/4);
    
    for (int i = 0; i < nTaille3; i += 3) {
        GREEN[i] = 0;
        GREEN[i + 1] = ImgIn[i + 1];
        GREEN[i + 2] = 0;
    }
    
    
    
//    cout << "PSNR = " << PSNR(ImgIn, ImgOut, nTaille3);
    
    ecrire_image_ppm(cNomImgEcrite, GREEN, nH, nW);
//    ecrire_image_ppm(cNomImgEcrite, ImgOut, nH, nW);
//    free(ImgIn);
}