#!/bin/bash

mkdir WindowsBinaries

TOP=$(pwd)  

chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*skeletontext.dll') $TOP/WindowsBinaries

TOP=$(pwd)  

cd ..

cd threshold_alpha_2 && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*thresholdalpha2.dll') $TOP/WindowsBinaries


