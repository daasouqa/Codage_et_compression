//
// Created by Ines Benghezal on 16/09/2019.
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
    if (argc != 4)
    {
        printf("Usage: ImageIn.ppm ImageOut.ppm color1 color2\n");
        exit (1) ;
    }
    
    //Colors to BW: (0.3 * R) + (0.59 * G) + (0.11 * B)
    
    
    //Lecture de l'image
    
    sscanf (argv[1],"%s",cNomImgLue) ;
    sscanf (argv[2],"%s",cNomImgEcrite);
    
    OCTET *ImgIn, *ImgOut, *leftClusterImg, *rightClusterImg;
    
    lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;
    
    int nTaille3 = nTaille * 3;
    allocation_tableau(ImgIn, OCTET, nTaille3);
    lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
    allocation_tableau(leftClusterImg, OCTET, nTaille3);
    allocation_tableau(rightClusterImg, OCTET, nTaille3);
    allocation_tableau(ImgOut, OCTET, nTaille3);
    
    
    //Sélection des deux couleurs
    
    int couleur1[3];
    couleur1[0] = ImgIn[0];
    couleur1[1] = ImgIn[1];
    couleur1[2] = ImgIn[2];
    
    int couleur2[3];
    couleur2[0] = ImgIn[nTaille3 - 2];
    couleur2[1] = ImgIn[nTaille3 - 1];
    couleur2[2] = ImgIn[nTaille3 - 0];
    
    // Création d'un tableau de 256 couleurs
    
    int decalage = 0;
    int colors[256 * 3];
    for (int j = 0; j < 256 * 3; j += 3) {
        colors[j] = ImgIn[j + decalage];
        colors[j + 1] = ImgIn[j + decalage + 1];
        colors[j + 2] = ImgIn[j + decalage + 2];
        decalage += 3;
    }
    
    /// Avec 256 couleurs

    for (int i = 0; i < nTaille3; i += 3) {
        int indiceMin = 0;
        int distMin = 5000;
        for (int j = 0; j < 256*3; j+=3) {
            if (distance(ImgIn[i], ImgIn[i + 1], ImgIn[i + 2], colors[j], colors[j + 1], colors[j + 2]) < distMin) {
                distMin = distance(ImgIn[i], ImgIn[i + 1], ImgIn[i + 2], colors[j], colors[j + 1], colors[j + 2]);
                indiceMin = j;
            }
        }
        ImgOut[i] = colors[indiceMin];
        ImgOut[i + 1] = colors[indiceMin + 1];
        ImgOut[i + 2] = colors[indiceMin + 2];
    }
    
    
//    /// Avec 2 couleurs
//    for (int i = 0; i < nTaille3; i += 3) {
//        if (distance(ImgIn[i], ImgIn[i + 1], ImgIn[i + 2], couleur1[0], couleur1[1], couleur1[2]) <
//            distance(ImgIn[i], ImgIn[i + 1], ImgIn[i + 2], couleur2[0], couleur2[1], couleur2[2])) {
//            ImgOut[i] = couleur1[0];
//            ImgOut[i + 1] = couleur1[1];
//            ImgOut[i + 2] = couleur1[2];
//        } else{
//            ImgOut[i] = couleur2[0];
//            ImgOut[i + 1] = couleur2[1];
//            ImgOut[i + 2] = couleur2[2];
//        }
//
//    }


//    ecrire_image_ppm(left, leftClusterImg,  1, leftCluster.size()/3);
//    ecrire_image_ppm(right, rightClusterImg,  1, rightCluster.size()/3);
    

    

    char fileName[250] = "2Colors.ppm";
    ecrire_image_ppm(fileName, ImgOut, nH, nW);
    free(ImgIn);
    return 1;
}










