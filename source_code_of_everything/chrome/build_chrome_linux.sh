#!/bin/bash

mkdir LinuxBinaries

TOP=$(pwd)  

chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*geglchrome.so') $TOP/LinuxBinaries

TOP=$(pwd)  

cd ..

cd bevel && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*bevel.so') $TOP/LinuxBinaries

cd .. 

cd edge_smooth && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*smoothedge.so') $TOP/LinuxBinaries

cd .. 

cd threshold_alpha_2 && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*thresholdalpha2.so') $TOP/LinuxBinaries





