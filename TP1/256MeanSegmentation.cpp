//
// Created by Inès Benghezal on 18/09/19.
//

#include <iostream>
#include <vector>


#include "ImageBase.cpp"

using namespace std;
double distanceColor(int red1, int green1, int blue1, int red2, int green2, int blue2) {
    return sqrt(pow(red1 - red2, 2) + pow(green1 - green2, 2) + pow(blue1 - blue2, 2));
}

/// Création de la structure "couleur"
struct color {
    OCTET r;
    OCTET g;
    OCTET b;
};


color moy(vector<color> colors){
    int sumR = 0;
    int sumG = 0;
    int sumB = 0;
    for (int i = 0; i < colors.size(); ++i) {
        sumR += colors[i].r;
        sumG += colors[i].g;
        sumB += colors[i].b;
    }
    
    color result;
    result.r = sumR / colors.size();
    result.g = sumG / colors.size();
    result.b = sumB / colors.size();
    
    return result;
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
    
    
    /// Sélection des 256 premières couleurs et affectation aux clusters
    
    int decalage = 0;
    color clusters[256];
    for (int j = 0; j < 256; j += 3) {
        clusters[j].r = ImgIn[j + decalage];
        clusters[j].g = ImgIn[j + decalage + 1];
        clusters[j].b = ImgIn[j + decalage + 2];
        decalage += 3;
    }
    
    for (int k = 0; k < 2; ++k) {
    
        /// K-Mean algo
        int bestCluster = 0;
        double distMin = distanceColor(ImgIn[0], ImgIn[1], ImgIn[2], clusters[0].r, clusters[0].g, clusters[0].b);
        for (int i = 1; i < nTaille3; i += 3) {
            for (int j = 0; j < 256; ++j) {
                if (distanceColor(ImgIn[i], ImgIn[i + 1], ImgIn[i + 2], clusters[j].r, clusters[j].g, clusters[j].b) <
                    distMin) {
                    bestCluster = j;
                    distMin = distanceColor(ImgIn[i], ImgIn[i + 1], ImgIn[i + 2], clusters[j].r, clusters[j].g,
                                            clusters[j].b);
                }
            }
        
            color clusterCenter;
            clusterCenter.r = (clusters[bestCluster].r + ImgIn[i]) / 2;
            clusterCenter.g = (clusters[bestCluster].r + ImgIn[i + 1]) / 2;
            clusterCenter.b = (clusters[bestCluster].r + ImgIn[i + 2]) / 2;
            clusters[bestCluster] = clusterCenter;
        }
    }
    
    
    for (int l = 0; l < nTaille3; l += 3) {
    
    }
    
    
}