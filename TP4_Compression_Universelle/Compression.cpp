//
// Created by Inès Benghezal on 04/11/19.
//
//
// Copyright (c) 2019 Inès Benghezal. All rights reserved.
//

#include <iostream>
#include <vector>


#include "../Sources/ImageBase.cpp"

using namespace std;

double distanceColor(int red1, int green1, int blue1, int red2, int green2, int blue2) {
    return sqrt(pow(red1 - red2, 2) + pow(green1 - green2, 2) + pow(blue1 - blue2, 2));
}

double PSNR(ImageBase a, ImageBase b) {
    
    if (a.getHeight() != b.getHeight() || a.getWidth() != b.getWidth() || a.getColor() != b.getColor()) {
        cerr << endl << "Les deux images n'ont pas les mêmes dimensions" << endl;
        return 0;
    } else{
    
        double EQM = 0;
        
        if (a.getColor()) {
            for (int i = 0; i < a.getHeight(); ++i) {
                for (int j = 0; j < a.getWidth(); ++j) {
                    EQM += pow(distanceColor(a[i * 3][j * 3], a[i * 3][j * 3 + 1], a[i * 3][j * 3 + 2], b[i * 3][j * 3],
                                             b[i * 3][j * 3 + 1], b[i * 3][j * 3 + 2]), 2);
                }
            }
    
            EQM = EQM / (a.getHeight() * a.getWidth());
        } else {
            for (int i = 0; i < a.getHeight(); ++i) {
                for (int j = 0; j < a.getWidth(); ++j) {
                    EQM += pow(a[i][j] - b[i][j], 2);
    
                }
            }
            EQM = (1.0/262144) * (EQM);
        }
        return (10 * log10(pow(255, 2) / EQM));
    }
}


void PSNR(char filename1[], char filename2[]) {
    ImageBase imIn1, imIn2;
    imIn1.load(filename1);
    imIn2.load(filename2);
    
    double eqm = 0.0;
    
    if (imIn1.getColor()) {
        int sommeR = 0, sommeG = 0, sommeB = 0;
        
        for (int i=0; i<imIn1.getHeight(); i++) {
            for (int j=0; j<imIn1.getWidth(); j++) {
                sommeR += pow(imIn1[i*3][j*3+0] - imIn2[i*3][j*3+0], 2);
                sommeG += pow(imIn1[i*3][j*3+1] - imIn2[i*3][j*3+1], 2);
                sommeB += pow(imIn1[i*3][j*3+2] - imIn2[i*3][j*3+2], 2);
            }
        }
        
        eqm = (1.0/262144) * (sommeR + sommeG + sommeB);
    }
    
    else {
        int somme = 0;
        
        for (int i=0; i<imIn1.getHeight(); i++) {
            for (int j=0; j<imIn1.getWidth(); j++) {
                somme += pow(imIn1[i][j] - imIn2[i][j], 2);
            }
        }
        eqm = (1.0/262144) * (somme);
    }
    
    double psnr = 10 * log10(262144.0 / eqm);
    
    std::cout << psnr << "\n";
}

int main(int argc, char* argv[]) {
    
    
    char cNomImgLue[250], cNomImgEcrite[250];
    
    if (argc != 3) {
        printf("Usage: ImageIn.pgm ImgOut.pgm \n");
        exit(1);
    }
    
    sscanf(argv[1], "%s", cNomImgLue);
    sscanf(argv[2], "%s", cNomImgEcrite);
    
    ImageBase ImgIn;
    ImgIn.load(cNomImgLue);
    
    
    ImageBase ImgOut(ImgIn.getWidth() / 2, ImgIn.getHeight() / 2, ImgIn.getColor());
    ImageBase ImgFinale(ImgIn.getWidth(), ImgIn.getHeight(), ImgIn.getColor());
    
    cout << "Image en couleur: " << ImgIn.getColor() << endl;
    
    if (ImgIn.getColor()) {
        for (int i = 0; i < ImgIn.getHeight(); i+=2) {
            for (int j = 0; j < ImgIn.getWidth(); j+=2) {
                int r = (ImgIn[i * 3][j * 3 + 0] + ImgIn[i * 3][(j + 1) * 3 + 0] + ImgIn[(i + 1) * 3][j * 3 + 0] + ImgIn[(i + 1) * 3][(j + 1) * 3 + 0]) / 4;
                int g = (ImgIn[i * 3][j * 3 + 1] + ImgIn[i * 3][(j + 1) * 3 + 1] + ImgIn[(i + 1) * 3][j * 3 + 1] + ImgIn[(i + 1) * 3][(j + 1) * 3 + 1]) / 4;
                int b = (ImgIn[i * 3][j * 3 + 2] + ImgIn[i * 3][(j + 1) * 3 + 2] + ImgIn[(i + 1) * 3][j * 3 + 2] + ImgIn[(i + 1) * 3][(j + 1) * 3 + 2]) / 4;
    
                ImgOut[(i/ 2) * 3 ][(j * 3 + 0) / 2] = r;
                ImgOut[(i/ 2) * 3 ][(j * 3 + 1) / 2] = g;
                ImgOut[(i/ 2) * 3 ][(j * 3 + 2) / 2] = b;
    
                ImgOut[(i/ 2) * 3 ][((j + 1) * 3 + 0) / 2] = r;
                ImgOut[(i/ 2) * 3 ][((j + 1) * 3 + 1) / 2] = g;
                ImgOut[(i/ 2) * 3 ][((j + 1) * 3 + 2) / 2] = b;
    
                ImgOut[((i + 1) / 2) * 3][(j / 2) * 3 + 0] = r;
                ImgOut[((i + 1) / 2) * 3][(j / 2) * 3 + 1] = g;
                ImgOut[((i + 1) / 2) * 3][(j / 2) * 3 + 2] = b;
                
                ImgOut[((i + 1)/ 2) * 3][((j + 1)/ 2) * 3 + 0 ] = r;
                ImgOut[((i + 1)/ 2) * 3][((j + 1)/ 2) * 3 + 1 ] = g;
                ImgOut[((i + 1)/ 2) * 3][((j + 1)/ 2) * 3 + 2 ] = b;
            }
    
            for (int i = 0; i < ImgFinale.getHeight(); i+=2) {
                for (int j = 0; j < ImgFinale.getWidth(); j+=2) {
    
                    int currentPxR = ImgOut[(i / 2) * 3][j / 2 + 0];
                    int currentPxG = ImgOut[(i / 2) * 3][j / 2 + 1];
                    int currentPxB = ImgOut[(i / 2) * 3][j / 2 + 2];

                    ImgFinale[i * 3][j * 3] = currentPxR;
                    ImgFinale[i * 3][j * 3] = currentPxG;
                    ImgFinale[i * 3][j * 3] = currentPxB;
                    
                    ImgFinale[i * 3][(j + 1) * 3] = currentPxR;
                    ImgFinale[i * 3][(j + 1) * 3] = currentPxG;
                    ImgFinale[i * 3][(j + 1) * 3] = currentPxB;
                    
                    ImgFinale[(i + 1) * 3][j * 3] = currentPxR;
                    ImgFinale[(i + 1) * 3][j * 3] = currentPxG;
                    ImgFinale[(i + 1) * 3][j * 3] = currentPxB;
                    
                    ImgFinale[(i + 1) * 3][(j + 1) * 3] = currentPxR;
                    ImgFinale[(i + 1) * 3][(j + 1) * 3] = currentPxG;
                    ImgFinale[(i + 1) * 3][(j + 1) * 3] = currentPxB;
                }
            }
        }
    
    } else {
        for (int i = 0; i < ImgIn.getHeight(); i+=2) {
            for (int j = 0; j < ImgIn.getWidth(); j+=2) {
    
                int pixel = static_cast<unsigned char>(
                        (ImgIn[i][j] + ImgIn[i][j + 1] + ImgIn[i + 1][j] + ImgIn[i + 1][j + 1]) / 4);
                
                ImgOut[i / 2][j / 2] = pixel;
            }
        }
        
        for (int i = 0; i < ImgFinale.getHeight(); i+=2) {
            for (int j = 0; j < ImgFinale.getWidth(); j+=2) {

                int currentPx = ImgOut[i / 2][j / 2];

                ImgFinale[i][j] = currentPx;
                ImgFinale[i][j + 1] = currentPx;
                ImgFinale[i + 1][j] = currentPx;
                ImgFinale[i + 1][j + 1] = currentPx;
            }
        }
    }
    
    ImgFinale.save(cNomImgEcrite);
    
    cout << "PSNR = " << PSNR(ImgIn, ImgFinale) << endl;
    
    
}
