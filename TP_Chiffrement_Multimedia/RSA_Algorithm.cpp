//
// Created by Inès Benghezal on 21/10/19.
//
//
// Copyright (c) 2019 Inès Benghezal. All rights reserved.
//

#include <iostream>
#include <vector>


#include "ImageBase.cpp"

using namespace std;

double entropy(char filename[]) {
    ImageBase imIn;
    imIn.load(filename);
    
    double res = 0.0;
    double histo[256] = {0};
    
    for (int i=0; i<imIn.getHeight(); i++) {
        for (int j=0; j<imIn.getWidth(); j++) {
            histo[imIn[i][j]]++;
        }
    }
    
    int size = imIn.getHeight() * imIn.getWidth();
    for (int i=0; i<256; i++) {
        if (histo[i] != 0) {
            res += (histo[i] / size) * log2(histo[i] / size);
        }
    }
    
    return -res;
}

bool isPrime(int n){
    if (n <= 1)
        return false;
    
    for (int i = 2; i < n; i++)
        if (n % i == 0)
            return false;
    
    return true;
}

int PGCD(int a, int b){
    while(b!=0){
        int c=a%b;
        a=b;
        b=c;
    }
    return a;
}

bool primeBetweenThem(int a, int b){
    return PGCD(a, b) == 1;
}

std::vector<int> cipheringExponent(int phi){
    std::vector<int> result;
    for (int i = 0; i < phi; ++i) {
        if (primeBetweenThem(i, phi)) {
            std::cout << "Exponent = " << i << std::endl;
            result.push_back(i);
        }
    }
    
    return result;
}

int modularExponentiation(int pixel, int e, int n) {
    int result = pixel;
    for (int i = 1; i < e; ++i) {
        result = (result * pixel) % n;
    }
    
    return result;
}

int modularInverse(int e, int phi){
    int b0 = phi, t, q;
    int x0 = 0, x1 = 1;
    if (phi == 1) return 1;
    while (e > 1) {
        q = e / phi;
        t = phi, phi = e % phi, e = t;
        t = x0, x0 = x1 - q * x0, x1 = t;
    }
    if (x1 < 0) x1 += b0;
    return x1;
    
}

void RSA_Ciphering(ImageBase ImgIn, int p, int q, int e, char fileName[250]) {
    ImageBase result(ImgIn.getWidth(), ImgIn.getHeight(), ImgIn.getColor());
    for (int i = 0; i < ImgIn.getHeight(); ++i) {
        for (int j = 0; j < ImgIn.getWidth(); ++j) {
            result[i][j] = modularExponentiation(ImgIn[i][j], e, p * q);
        }
    }
    
    result.save(fileName);
}


void RSA_Deciphering(ImageBase ImgIn, int p, int q, int e, char fileName[250]) {
    int phi = (p - 1) * (q - 1);
    int d = modularInverse(e, phi);
    
    std::cout << "d = " << d << std::endl;
    ImageBase result(ImgIn.getWidth(), ImgIn.getHeight(), ImgIn.getColor());
    
    for (int i = 0; i < ImgIn.getHeight(); ++i) {
        for (int j = 0; j < ImgIn.getWidth(); ++j) {
            result[i][j] = modularExponentiation(ImgIn[i][j], d, p * q);
        }
    }
    
    result.save(fileName);
}

int main(int argc, char* argv[]) {
    
    
    char cNomImgLue[250], cnomImgChiffree[250];
    int nH, nW, nTaille;
    if (argc != 3) {
        printf("Usage: ImageIn.pgm ImgOut.pgm \n");
        exit(1);
    }
    
    sscanf(argv[1], "%s", cNomImgLue);
    sscanf(argv[2], "%s", cnomImgChiffree);
    
    ImageBase ImgIn;
    ImgIn.load(cNomImgLue);
    
    int p = 11, q = 23, e = 219;
    
//    cipheringExponent((p - 1) * (q - 1));
    RSA_Ciphering(ImgIn, p, q, e, cnomImgChiffree);
//    RSA_Deciphering(ImgIn, p, q, e, cnomImgChiffree);
    

}
