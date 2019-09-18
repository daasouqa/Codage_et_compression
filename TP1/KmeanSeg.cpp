//
// Created by Ines Benghezal on 17/09/2019.
//

#include <iostream>
#include <vector>


#include "ImageBase.cpp"

double distance(int red1, int green1, int blue1, int red2, int green2, int blue2) {
    return sqrt(pow(red1 - red2, 2) + pow(green1 - green2, 2) + pow(blue1 - blue2, 2));
}

int main(int argc, char* argv[]) {
    
    using namespace std;
    
    char cNomImgLue[250], cNomImgEcrite[250];
    int nH, nW, nTaille, nR, nG, nB, S;
    if (argc != 4) {
        printf("Usage: ImageIn.ppm ImageOut.ppm color1 color2\n");
        exit(1);
    }
    
    //Colors to BW: (0.3 * R) + (0.59 * G) + (0.11 * B)
    
    
    //Lecture de l'image
    
    sscanf(argv[1], "%s", cNomImgLue);
    sscanf(argv[2], "%s", cNomImgEcrite);
    
    OCTET *ImgIn, *ImgOut;
    
    lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;
    
    int nTaille3 = nTaille * 3;
    allocation_tableau(ImgIn, OCTET, nTaille3);
    lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
//    allocation_tableau(leftClusterImg, OCTET, nTaille3);
//    allocation_tableau(rightClusterImg, OCTET, nTaille3);
    allocation_tableau(ImgOut, OCTET, nTaille3);

    /// Création de la structure "couleur"
     struct color {
         int r;
         int g;
         int b;
     };
     
    /// Création des 2 clusters
    
    vector<color> leftCluster;
    vector<color> rightCluster;

    /// Sélection de 2 couleurs de l'image
    
    color leftCenter, rightCenter;
    
    leftCenter.r = ImgIn[0];
    leftCenter.g = ImgIn[1];
    leftCenter.b = ImgIn[2];
    
    rightCenter.r = ImgIn[nTaille3 - 3];
    rightCenter.g = ImgIn[nTaille3 - 2];
    rightCenter.b = ImgIn[nTaille3 - 1];
    
    /// Ajout des deux couleurs à leurs clusters
    
    leftCluster.push_back(leftCenter);
    rightCluster.push_back(rightCenter);
    
    /// Lancement de l'algorithme K-mean
    
    for (int k = 0; k < 5; ++k) {
        std::cout << k << std::endl;
        for (int i = 0; i < nTaille3; i += 3) {
            if (distance(ImgIn[i], ImgIn[i + 1], ImgIn[i + 2], leftCenter.r, leftCenter.g, leftCenter.b) <
                distance(ImgIn[i], ImgIn[i + 1], ImgIn[i + 2], rightCenter.r, rightCenter.g, rightCenter.b)) {
                /// Dans ce cas: La couleur appartient au cluster gauche
                
                /// Vérifier que la couleur n'est pas déjà dans e cluster
    
                bool exists = false;
                for (int j = 0; j < leftCluster.size(); ++j) {
                    if (leftCluster[j].r == ImgIn[i] && leftCluster[j].g == ImgIn[i + 1] && leftCluster[j].b) {
                        exists = true;
                    }
                }
                
                /// Ajout de la couleur au cluster
                if (!exists) {
                    color newColor;
                    newColor.r = ImgIn[i];
                    newColor.g = ImgIn[i + 1];
                    newColor.b = ImgIn[i + 2];
                    leftCluster.push_back(newColor);
                }
                
                /// Retirer la couleur de l'autre cluster si elle y est déjà
    
                bool isElsewhere = false;
                int where = 0;
                for (int l = 0; l < rightCluster.size(); ++l) {
                    if (rightCluster[l].r == ImgIn[i] && rightCluster[l].g == ImgIn[i + 1] &&
                        rightCluster[l].b == ImgIn[i + 2]) {
                        where = l;
                        isElsewhere = true;
                    }
                }
    
                if (isElsewhere) {
                    rightCluster.erase(rightCluster.begin() + where);
                }
            } else {
                /// La couleur appartient au cluster droit
                
                /// Vérifier qu'elle n'y est pas déjà
    
                bool exists = false;
                for (int j = 0; j < rightCluster.size(); ++j) {
                    if (rightCluster[j].r == ImgIn[i] && rightCluster[j].g == ImgIn[i + 1] && rightCluster[j].b) {
                        exists = true;
                    }
                }
                
                /// Ajout de la couleur au cluster
                if (!exists) {
                    color newColor;
                    newColor.r = ImgIn[i];
                    newColor.g = ImgIn[i + 1];
                    newColor.b = ImgIn[i + 2];
                    
                }
                
                /// Retirer la couleur de l'autre cluster si elle y est
    
                bool isElsewhere = false;
                int where = 0;
                
                for (int l = 0; l < leftCluster.size(); ++l) {
                    if (leftCluster[l].r == ImgIn[i] && leftCluster[l].g == ImgIn[i + 1] &&
                        leftCluster[l].b == ImgIn[i + 2]) {
                        isElsewhere = true;
                        where = l;
                    }
                }
    
                if (isElsewhere) {
                    leftCluster.erase(leftCluster.begin() + where);
                }
            }
    
            leftCenter = leftCluster[leftCluster.size() / 2];
            rightCenter = rightCluster[rightCluster.size() / 2];
        }
    }
    
    /// Création de l'image finale
    
    for (int m = 0; m < nTaille3; m += 3) {
        
        if (distance(ImgIn[m], ImgIn[m + 1], ImgIn[m + 2], leftCenter.r, leftCenter.g, leftCenter.b) <
            distance(ImgIn[m], ImgIn[m + 1], ImgIn[m + 2], rightCenter.r, rightCenter.g, rightCenter.b)) {
            ImgOut[m] = leftCenter.r;
            ImgOut[m + 1] = leftCenter.g;
            ImgOut[m + 2] = leftCenter.b;
        } else {
            ImgOut[m] = rightCenter.r;
            ImgOut[m + 1] = rightCenter.g;
            ImgOut[m + 2] = rightCenter.b;
        }
    }
    
    char fileName[250] = "lena2.ppm";
    ecrire_image_pgm(fileName, ImgOut, nH, nW);
    free(ImgIn);
}