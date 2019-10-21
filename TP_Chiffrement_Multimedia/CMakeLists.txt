cmake_minimum_required(VERSION 3.13)
project(TP3_Chiffrement_Multimedia)

set(CMAKE_CXX_STANDARD 14)

add_executable(TP3_Chiffrement_Multimedia XOR_Encryption.cpp RSA_Algorithm.cpp Histogram.cpp SeuillageBW.cpp)