//
// Created by Ines Benghezal on 29/09/2019.
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
    
    lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;
    
    int nTaille3 = nTaille * 3;
    allocation_tableau(ImgIn, OCTET, nTaille3);
    lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
    allocation_tableau(ImgOut, OCTET, nTaille3);
    
    
    for (int i = 0; i < nTaille3; i += 6) {
        ImgOut[i] = ImgIn[i];
        ImgOut[i + 1] = ImgIn[i + 1];
        ImgOut[i + 2] = ImgIn[i + 2];
        ImgOut[i + 3] = ImgIn[i];
        ImgOut[i + 4] = ImgIn[i + 4];
        ImgOut[i + 5] = ImgIn[i + 2];
    }
    
    cout << "PSNR = " << PSNR(ImgIn, ImgOut, nTaille3);
    
    ecrire_image_ppm(cNomImgEcrite, ImgOut, nH, nW);
    free(ImgIn);
}