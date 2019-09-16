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
    
    OCTET *ImgIn, *leftClusterImg,*rightClusterImg;
    
    lire_nb_lignes_colonnes_image_ppm(cNomImgLue, &nH, &nW);
    nTaille = nH * nW;
    
    int nTaille3 = nTaille * 3;
    allocation_tableau(ImgIn, OCTET, nTaille3);
    lire_image_ppm(cNomImgLue, ImgIn, nH * nW);
    allocation_tableau(leftClusterImg, OCTET, nTaille3);
    allocation_tableau(rightClusterImg, OCTET, nTaille3);
    
    //////////////////////////////////////
    
    
    // Création des 2 clusters
    
    int leftCenter[3];
    int rightCenter[3];
    
    // Sélection des 2 couleurs
    
    leftCenter[0] = 0;
    leftCenter[1] = 0;
    leftCenter[2] = 0;
    
    rightCenter[0] = 255;
    rightCenter[1] = 255;
    rightCenter[2] = 255;
    
    
    vector<int> leftCluster;
    vector<int> rightCluster;
    
    leftCluster.push_back(leftCenter[0]);
    leftCluster.push_back(leftCenter[1]);
    leftCluster.push_back(leftCenter[2]);
    
    rightCluster.push_back(rightCenter[0]);
    rightCluster.push_back(rightCenter[1]);
    rightCluster.push_back(rightCenter[2]);
    
    for (int i=0; i < nTaille3; i+=3)
    {
        nR = ImgIn[i];
        nG = ImgIn[i+1];
        nB = ImgIn[i+2];
        
        if (distance(nR,nG,nB,leftCenter[0],leftCenter[1],leftCenter[2]) < distance(nR,nG,nB,rightCenter[0],rightCenter[1],rightCenter[2])){
            for (int j = 0; j < leftCluster.size(); j+=3) {
                if (!(leftCluster[j] == nR && leftCluster[j + 1] == nG && leftCluster[j + 2] == nB)) {
                    leftCluster.push_back(nR);
                    leftCluster.push_back(nG);
                    leftCluster.push_back(nB);
                }
            }
        } else {
            for (int j = 0; j < rightCluster.size(); j+=3) {
                if (!(rightCluster[j] == nR && rightCluster[j + 1] && rightCluster[j + 2])) {
                    rightCluster.push_back(nR);
                    rightCluster.push_back(nG);
                    rightCluster.push_back(nB);
                }
            }
        }
        
    }
    
    cout << "Left cluster: " << leftCluster.size() / 3<< endl;
    cout << "Right cluster: " << rightCluster.size() / 3 << endl;
    
    for (int j = 0; j < leftCluster.size(); ++j) {
        leftClusterImg[j] = leftCluster[j];
    }
    for (int k = 0; k < rightCluster.size(); ++k) {
        rightClusterImg[k] = rightCluster[k];
    }
    
    char left[250] = "leftCluster.ppm";
    char right[250] = "rightCluster.ppm";
    
    ecrire_image_ppm(left, leftClusterImg,  1, leftCluster.size()/3);
    ecrire_image_ppm(right, rightClusterImg,  1, rightCluster.size()/3);
    free(ImgIn);
    return 1;
}


