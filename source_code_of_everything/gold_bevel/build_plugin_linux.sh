#!/bin/bash

mkdir LinuxBinaries

TOP=$(pwd)  

chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name 'goldbevel2.so') $TOP/LinuxBinaries

cd ..

cd metallic && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name 'metal.so') $TOP/LinuxBinaries

cd ..

cd sharp_bevel && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name 'sharpbevel.so') $TOP/LinuxBinaries

cd ..

cd threshold_alpha2 && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name 'thresholdalpha2.so') $TOP/LinuxBinaries

cd ..

cd edge_smooth && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name 'smoothedge.so') $TOP/LinuxBinaries

