//
// Created by Inès Benghezal on 16/09/19.
//


#include <iostream>
#include <vector>


#include "ImageBase.cpp"

double distance(int red1, int green1, int blue1, int red2, int green2, int blue2) {
    return sqrt(pow(red1 - red2, 2) + pow(green1 - green2, 2) + pow(blue1 - blue2, 2));
}

int main(int argc, char* argv[]) {
    
    using namespace std;
    
    struct color {
        int r;
        int g;
        int b;
    };
    
    char cNomImgLue[250], cNomImgEcrite[250];
    int nH, nW, nTaille, nR, nG, nB, S;
    if (argc != 3) {
        printf("Usage: ImageIn.ppm ImageOut.ppm\n");
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
    
    /// Sélection des 2 couleurs initiales
    
    color leftClusterCenter, rightClusterCenter;
    
    leftClusterCenter.r = ImgIn[0];
    leftClusterCenter.g = ImgIn[1];
    leftClusterCenter.b = ImgIn[1];
    
    rightClusterCenter.r= ImgIn[nTaille3 - 3];
    rightClusterCenter.g= ImgIn[nTaille3 - 2];
    rightClusterCenter.b= ImgIn[nTaille3 -1];
    
    // Initialisation des 2 clusters
    
    vector<color> leftCluster;
    vector<color> rightCluster;
    
    leftCluster.push_back(leftClusterCenter);
    rightCluster.push_back(rightClusterCenter);
    
    
    // Sauvegarde de la taille des clusters pour comparaison
    
    unsigned long sizeLeftCluster;
    unsigned long sizeRightCluster;
    
    
    // Segmentation
    
    //do {
    
    for (int k = 0; k < 5; ++k) {
        sizeLeftCluster = leftCluster.size();
        sizeRightCluster = rightCluster.size();
    
        for (int i = 0; i < nTaille3; i += 3) {
            if (distance(ImgIn[i], ImgIn[i + 1], ImgIn[i + 2], leftClusterCenter.r, leftClusterCenter.g,
                         leftClusterCenter.b) <
                distance(ImgIn[i], ImgIn[i + 1], ImgIn[i + 2], rightClusterCenter.r, rightClusterCenter.g,
                         rightClusterCenter.b)) {
            
                /// Vérifier que la couleur n'est pas déjà dans son cluster respectif
                bool exists = false;
                for (int j = 0; j < leftCluster.size(); ++j) {
                    if (ImgIn[i] == leftCluster[j].r && ImgIn[i + 1] == leftCluster[j].g &&
                        ImgIn[i + 2] == leftCluster[j].b) {
                    
                        exists = true;
                    }
                }
                if (!exists) {
                    color newColor;
                    newColor.r = ImgIn[i];
                    newColor.g = ImgIn[i + 1];
                    newColor.b = ImgIn[i + 2];
                
                    leftCluster.push_back(newColor);
                }
            
                /// Vérifier aussi qu'elle n'est pas dans l'autre cluster (si c'est le cas, on la retire de ce dernier
            
//                bool isAilleurs = false;
//                int whereAilleurs;
//                for (int j = 0; j < rightCluster.size(); ++j) {
//                    if (ImgIn[i] == rightCluster[j].r && ImgIn[i + 1] == rightCluster[j].g &&
//                        ImgIn[i + 2] == rightCluster[j].b) {
//                        isAilleurs = true;
//                        whereAilleurs = j;
//                    }
//                }
//                if (isAilleurs) {
//                    rightCluster.erase(rightCluster.begin() + whereAilleurs);
//                }
            
            } else {
            
                /// Vérifier que la couleur n'est pas déjà dans son cluster respectif pour eviter la redondance des couleurs
                bool exists = false;
                for (int j = 0; j < rightCluster.size(); ++j) {
                    if (ImgIn[i] == rightCluster[j].r && ImgIn[i + 1] == rightCluster[j].g &&
                        ImgIn[i + 2] == rightCluster[j].b) {
                    
                        exists = true;
                    }
                }
                if (!exists) {
                    color newColor;
                    newColor.r = ImgIn[i];
                    newColor.g = ImgIn[i + 1];
                    newColor.b = ImgIn[i + 2];
                
                    rightCluster.push_back(newColor);
                }
            
                /// Retirer cette couleur de l'autre cluster si elle s'y trouve
            
//                int whereAilleurs;
//                bool isAilleurs = false;
//                for (int j = 0; j < leftCluster.size(); ++j) {
//                    if (ImgIn[i] == leftCluster[j].r && ImgIn[i + 1] == leftCluster[j].g &&
//                        ImgIn[i + 2] == leftCluster[j].b) {
//
//                        isAilleurs = true;
//                        whereAilleurs = j;
//                    }
//                }
//                if (isAilleurs) {
//                    leftCluster.erase(leftCluster.begin() + whereAilleurs);
//                }
            }
        }
    
        /// Recalculer les barycentres
    
        leftClusterCenter.r = leftCluster[leftCluster.size() / 2].r;
        leftClusterCenter.g = leftCluster[leftCluster.size() / 2].g;
        leftClusterCenter.b = leftCluster[leftCluster.size() / 2].b;
    
        rightClusterCenter.r = rightCluster[rightCluster.size() / 2].r;
        rightClusterCenter.g = rightCluster[rightCluster.size() / 2].g;
        rightClusterCenter.b = rightCluster[rightCluster.size() / 2].b;
    
        std::cout << "Left cluster: " << leftCluster.size() / 3 << " colors" << endl;
        std::cout << "Left cluster barycentre = (" << leftClusterCenter.r << ", " << leftClusterCenter.g << ", "
                  << leftClusterCenter.b << ")" << endl;
        std::cout << "Right cluster: " << rightCluster.size() / 3 << " colors" << endl;
        std::cout << "Right cluster barycentre = (" << rightClusterCenter.r << ", " << rightClusterCenter.g << ", "
                  << rightClusterCenter.b << ")" << endl;
    }
        
    
        
    //} while (leftCluster.size() != sizeLeftCluster || rightCluster.size() != sizeRightCluster);
    
    for (int l = 0; l < nTaille3; l += 3) {
        if (distance(ImgIn[l], ImgIn[l + 1], ImgIn[l + 2], leftClusterCenter.r, leftClusterCenter.g,
                     leftClusterCenter.b) < distance(ImgIn[l], ImgIn[l + 1], ImgIn[l + 2], rightClusterCenter.r, rightClusterCenter.g,
                                                     rightClusterCenter.b)) {
            ImgOut[l] = leftClusterCenter.r;
            ImgOut[l + 1] = leftClusterCenter.g;
            ImgOut[l + 2] = leftClusterCenter.b;
        } else {
            ImgOut[l] = leftClusterCenter.r;
            ImgOut[l + 1] = leftClusterCenter.g;
            ImgOut[l + 2] = leftClusterCenter.b;
        }
    }
    
    
    char fileName[250] = "testKmean.ppm";
    ecrire_image_ppm(fileName, ImgOut, nH, nW);
    free(ImgIn);
    return 1;
    
    
}