//
// Created by Inès Benghezal on 07/10/19.
//
//
// Copyright (c) 2019 Inès Benghezal. All rights reserved.
//

#include <iostream>
#include <vector>


#include "../Sources/ImageBase.cpp"

using namespace std;

struct color{
    int r;
    int g;
    int b;
};

struct colorYCrCb {
    double y;
    double cr;
    double cb;
};

colorYCrCb toYCrCb(color c){
    
    colorYCrCb result;
    result.y = 0.299f * c.r + 0.587f * c.g + 0.114f * c.b;
    result.cr = -0.1687f * c.r - 0.3313 * c.g + 0.5 * c.b + 128;
    result.cb = 0.5 * c.r - 0.4187 * c.g - 0.0813 * c.b + 128;
    
    return result;
}

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
    
    sscanf (argv[1],"%s",cNomImgLue) ;
    sscanf (argv[2],"%s",cNomImgEcrite);
    
    ImageBase ImgIn;
    ImgIn.load(cNomImgLue);
    
    ImageBase ImgOut(ImgIn.getWidth(), ImgIn.getHeight(), ImgIn.getColor());
    ImageBase cr(ImgIn.getWidth(), ImgIn.getHeight(), ImgIn.getColor());
    ImageBase y(ImgIn.getWidth(), ImgIn.getHeight(), ImgIn.getColor());
    ImageBase cb(ImgIn.getWidth(), ImgIn.getHeight(), ImgIn.getColor());
    

    
    vector<colorYCrCb> yCrCb;
    
    for (int i = 0; i < ImgIn.getHeight(); ++i) {
        for (int j = 0; j < ImgIn.getWidth(); ++j) {
            color old;
            old.r = ImgIn[i * 3][j * 3];
            old.g = ImgIn[i * 3][j * 3 + 1];
            old.b = ImgIn[i * 3][j * 3 + 2];
            
            colorYCrCb newColor = toYCrCb(old);
    
            ImgIn[i * 3][j * 3] = newColor.cr;
            ImgIn[i * 3][j * 3 + 1] = newColor.y;
            ImgIn[i * 3][j * 3 + 2] = newColor.cb;
            
        }
    }
    
    //1er passage
    
    for (int i = 0; i < ImgIn.getHeight(); i += 2) {
        for (int j = 0; j < ImgIn.getWidth(); j += 2) {
            double ACr = ImgIn[i * 3][j * 3];
            double BCr = ImgIn[i * 3][(j+1) * 3];
            double CCr= ImgIn[(i + 1) * 3][j * 3];
            double DCr = ImgIn[(i + 1) * 3][(j + 1) * 3];
            
            double Ay = ImgIn[i * 3][j * 3 + 1];
            double By = ImgIn[i * 3][(j+1) * 3 + 1];
            double Cy= ImgIn[(i + 1) * 3][j * 3 + 1];
            double Dy = ImgIn[(i + 1) * 3][(j + 1) * 3 + 1];
    
            double ACb = ImgIn[i * 3][j * 3 + 2];
            double BCb = ImgIn[i * 3][(j+1) * 3 + 2];
            double CCb= ImgIn[(i + 1) * 3][j * 3 + 2];
            double DCb = ImgIn[(i + 1) * 3][(j + 1) * 3 + 2];
            
            
    
            cr[(i / 2) * 3][(j / 2) * 3] = (ACr + BCr + CCr + DCr) / 4;
            cr[(i / 2) * 3][((j / 2) + ImgIn.getWidth() / 2) * 3] = (ACr + BCr - CCr - DCr) / 2;
            cr[((i / 2) + ImgIn.getHeight() / 2) * 3][(j / 2) * 3] = (ACr - BCr + CCr - DCr) / 2;
            cr[((i / 2) + ImgIn.getHeight() / 2) * 3][((j / 2) + ImgIn.getWidth() / 2) * 3] = ACr - BCr - CCr + DCr;
    
            y[(i / 2) * 3][(j / 2) * 3 + 1] = (Ay + By + Cy + Dy) / 4;
            y[(i / 2) * 3][((j / 2) + ImgIn.getWidth() / 2) * 3 + 1] = (Ay + By - Cy - Dy) / 2;
            y[((i / 2) + ImgIn.getHeight() / 2) * 3][(j / 2) * 3 + 1] = (Ay - By + Cy - Dy) / 2;
            y[((i / 2) + ImgIn.getHeight() / 2) * 3][((j / 2) + ImgIn.getWidth() / 2) * 3 + 1] = Ay - By - Cy + Dy;
    
            cb[(i / 2) * 3][(j / 2) * 3 + 2] = (ACb + BCb + CCb + DCb) / 4;
            cb[(i / 2) * 3][((j / 2) + ImgIn.getWidth() / 2) * 3 + 2] = (ACb + BCb - CCb - DCb) / 2;
            cb[((i / 2) + ImgIn.getHeight() / 2) * 3][(j / 2) * 3 + 2] = (ACb - BCb + CCb - DCb) / 2;
            cb[((i / 2) + ImgIn.getHeight() / 2) * 3][((j / 2) + ImgIn.getWidth() / 2) * 3 + 2] = ACb - BCb - CCb + DCb;
    
    
            ImgOut[(i / 2) * 3][(j / 2) * 3 + 0] = (ACr + BCr + CCr + DCr) / 4;
            ImgOut[(i / 2) * 3][(j / 2) * 3 + 1] = (Ay + By + Cy + Dy) / 4;
            ImgOut[(i / 2) * 3][(j / 2) * 3 + 2] = (ACb + BCb + CCb + DCb) / 4;
            
            ImgOut[(i / 2) * 3][((j / 2) + ImgIn.getWidth() / 2) * 3 + 0] = (ACr + BCr - CCr - DCr) / 2;
            ImgOut[(i / 2) * 3][((j / 2) + ImgIn.getWidth() / 2) * 3 + 1] = (Ay + By - Cy - Dy) / 2;
            ImgOut[(i / 2) * 3][((j / 2) + ImgIn.getWidth() / 2) * 3 + 2] = (ACb + BCb - CCb - DCb) / 2;
            
            ImgOut[((i / 2) + ImgIn.getHeight() / 2) * 3][(j / 2) * 3 + 0] = (ACr - BCr + CCr - DCr) / 2;
            ImgOut[((i / 2) + ImgIn.getHeight() / 2) * 3][(j / 2) * 3 + 1] = (Ay - By + Cy - Dy) / 2;
            ImgOut[((i / 2) + ImgIn.getHeight() / 2) * 3][(j / 2) * 3 + 2] = (ACb - BCb + CCb - DCb) / 2;
    
            ImgOut[((i / 2) + ImgIn.getHeight() / 2) * 3][((j / 2) + ImgIn.getWidth() / 2) * 3 + 0] = ACr - BCr - CCr + DCr;
            ImgOut[((i / 2) + ImgIn.getHeight() / 2) * 3][((j / 2) + ImgIn.getWidth() / 2) * 3 + 1] = Ay - By - Cy + Dy;
            ImgOut[((i / 2) + ImgIn.getHeight() / 2) * 3][((j / 2) + ImgIn.getWidth() / 2) * 3 + 2] = ACb - BCb - CCb + DCb;
            
        }
    }
    
    ImageBase ImgOut2(ImgOut.getWidth(), ImgOut.getHeight(), ImgOut.getColor());
    for (int i = 0; i < ImgOut.getHeight(); ++i) {
        for (int j = 0; j < ImgOut.getWidth(); ++j) {
            ImgOut2[i * 3][j * 3 + 0] = ImgOut[i * 3][j * 3 + 0];
            ImgOut2[i * 3][j * 3 + 1] = ImgOut[i * 3][j * 3 + 1];
            ImgOut2[i * 3][j * 3 + 2] = ImgOut[i * 3][j * 3 + 2];
        }
    }
    
    //2eme passage
    for (int i = 0; i < ImgOut.getHeight() / 2; i += 2) {
        for (int j = 0; j < ImgOut.getWidth() / 2; j += 2) {
            double ACr = ImgOut[i * 3][j * 3];
            double BCr = ImgOut[i * 3][(j + 1) * 3];
            double CCr= ImgOut[(i + 1) * 3][j * 3];
            double DCr = ImgOut[(i + 1) * 3][(j + 1) * 3];

            double Ay = ImgOut[i * 3][j * 3 + 1];
            double By = ImgOut[i * 3][(j+1) * 3 + 1];
            double Cy= ImgOut[(i + 1) * 3][j * 3 + 1];
            double Dy = ImgOut[(i + 1) * 3][(j + 1) * 3 + 1];

            double ACb = ImgOut[i * 3][j * 3 + 2];
            double BCb = ImgOut[i * 3][(j+1) * 3 + 2];
            double CCb= ImgOut[(i + 1) * 3][j * 3 + 2];
            double DCb = ImgOut[(i + 1) * 3][(j + 1) * 3 + 2];


            ImgOut2[(i / 2) * 3][(j / 2) * 3 + 0] = (ACr + BCr + CCr + DCr) / 4;
            ImgOut2[(i / 2) * 3][(j / 2) * 3 + 1] = (Ay + By + Cy + Dy) / 4;
            ImgOut2[(i / 2) * 3][(j / 2) * 3 + 2] = (ACb + BCb + CCb + DCb) / 4;

            ImgOut2[(i / 2) * 3][((j / 2) + ImgOut.getWidth() / 4) * 3 + 0] = (ACr + BCr - CCr - DCr) / 2;
            ImgOut2[(i / 2) * 3][((j / 2) + ImgOut.getWidth() / 4) * 3 + 1] = (Ay + By - Cy - Dy) / 2;
            ImgOut2[(i / 2) * 3][((j / 2) + ImgOut.getWidth() / 4) * 3 + 2] = (ACb + BCb - CCb - DCb) / 2;

            ImgOut2[((i / 2) + ImgOut.getHeight() / 4) * 3][(j / 2) * 3 + 0] = (ACr - BCr + CCr - DCr) / 2;
            ImgOut2[((i / 2) + ImgOut.getHeight() / 4) * 3][(j / 2) * 3 + 1] = (Ay - By + Cy - Dy) / 2;
            ImgOut2[((i / 2) + ImgOut.getHeight() / 4) * 3][(j / 2) * 3 + 2] = (ACb - BCb + CCb - DCb) / 2;

            ImgOut2[((i / 2) + ImgOut.getHeight() / 4) * 3][((j / 2) + ImgOut.getWidth() / 4) * 3 + 0] = ACr - BCr - CCr + DCr;
            ImgOut2[((i / 2) + ImgOut.getHeight() / 4) * 3][((j / 2) + ImgOut.getWidth() / 4) * 3 + 1] = Ay - By - Cy + Dy;
            ImgOut2[((i / 2) + ImgOut.getHeight() / 4) * 3][((j / 2) + ImgOut.getWidth() / 4) * 3 + 2] = ACb - BCb - CCb + DCb;

        }
    }
    
    ImageBase ImgOut3(ImgOut2.getWidth(), ImgOut2.getHeight(), ImgOut2.getColor());
    for (int i = 0; i < ImgOut.getHeight(); ++i) {
        for (int j = 0; j < ImgOut.getWidth(); ++j) {
            ImgOut3[i * 3][j * 3 + 0] = ImgOut2[i * 3][j * 3 + 0];
            ImgOut3[i * 3][j * 3 + 1] = ImgOut2[i * 3][j * 3 + 1];
            ImgOut3[i * 3][j * 3 + 2] = ImgOut2[i * 3][j * 3 + 2];
        }
    }
    
    //3eme passe
    for (int i = 0; i < ImgOut2.getHeight() / 4; i += 2) {
        for (int j = 0; j < ImgOut2.getWidth() / 4; j += 2) {
            double ACr = ImgOut2[i * 3][j * 3];
            double BCr = ImgOut2[i * 3][(j + 1) * 3];
            double CCr= ImgOut2[(i + 1) * 3][j * 3];
            double DCr = ImgOut2[(i + 1) * 3][(j + 1) * 3];
            
            double Ay = ImgOut2[i * 3][j * 3 + 1];
            double By = ImgOut2[i * 3][(j+1) * 3 + 1];
            double Cy= ImgOut2[(i + 1) * 3][j * 3 + 1];
            double Dy = ImgOut2[(i + 1) * 3][(j + 1) * 3 + 1];
            
            double ACb = ImgOut2[i * 3][j * 3 + 2];
            double BCb = ImgOut2[i * 3][(j+1) * 3 + 2];
            double CCb= ImgOut2[(i + 1) * 3][j * 3 + 2];
            double DCb = ImgOut2[(i + 1) * 3][(j + 1) * 3 + 2];
            
            
            ImgOut3[(i / 2) * 3][(j / 2) * 3 + 0] = (ACr + BCr + CCr + DCr) / 4;
            ImgOut3[(i / 2) * 3][(j / 2) * 3 + 1] = (Ay + By + Cy + Dy) / 4;
            ImgOut3[(i / 2) * 3][(j / 2) * 3 + 2] = (ACb + BCb + CCb + DCb) / 4;
            
            ImgOut3[(i / 2) * 3][((j / 2) + ImgOut.getWidth() / 8) * 3 + 0] = (ACr + BCr - CCr - DCr) / 2;
            ImgOut3[(i / 2) * 3][((j / 2) + ImgOut.getWidth() / 8) * 3 + 1] = (Ay + By - Cy - Dy) / 2;
            ImgOut3[(i / 2) * 3][((j / 2) + ImgOut.getWidth() / 8) * 3 + 2] = (ACb + BCb - CCb - DCb) / 2;
            
            ImgOut3[((i / 2) + ImgOut.getHeight() / 8) * 3][(j / 2) * 3 + 0] = (ACr - BCr + CCr - DCr) / 2;
            ImgOut3[((i / 2) + ImgOut.getHeight() / 8) * 3][(j / 2) * 3 + 1] = (Ay - By + Cy - Dy) / 2;
            ImgOut3[((i / 2) + ImgOut.getHeight() / 8) * 3][(j / 2) * 3 + 2] = (ACb - BCb + CCb - DCb) / 2;
            
            ImgOut3[((i / 2) + ImgOut.getHeight() / 8) * 3][((j / 2) + ImgOut.getWidth() / 8) * 3 + 0] = ACr - BCr - CCr + DCr;
            ImgOut3[((i / 2) + ImgOut.getHeight() / 8) * 3][((j / 2) + ImgOut.getWidth() / 8) * 3 + 1] = Ay - By - Cy + Dy;
            ImgOut3[((i / 2) + ImgOut.getHeight() / 8) * 3][((j / 2) + ImgOut.getWidth() / 8) * 3 + 2] = ACb - BCb - CCb + DCb;
            
        }
    }
    
//    cr.save(const_cast<char *>("cr.ppm"));
//    y.save(const_cast<char *>("y.ppm"));
//    cb.save(const_cast<char *>("cb.ppm"));
    ImgOut3.save(cNomImgEcrite);
}

