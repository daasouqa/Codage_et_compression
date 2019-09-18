cmake_minimum_required(VERSION 3.13)
project(TP1)

set(CMAKE_CXX_STANDARD 14)

add_executable(TP1 main.cpp ReductionCouleur.cpp KmeanSeg.cpp SegmentationKmean.cpp 2MeanSegmentation.cpp 256MeanSegmentation.cpp)