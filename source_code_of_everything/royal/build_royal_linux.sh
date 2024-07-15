#!/bin/bash

mkdir LinuxBinaries

TOP=$(pwd)  

chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*royal.so') $TOP/LinuxBinaries

TOP=$(pwd)  

cd ..

cd extrusion && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*extrusion.so') $TOP/LinuxBinaries

cd .. 

cd edge_smooth && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*smoothedge.so') $TOP/LinuxBinaries

cd .. 

cd threshold_alpha_2 && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*thresholdalpha2.so') $TOP/LinuxBinaries

cd .. 

cd glass_over_text && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*glassovertext.so') $TOP/LinuxBinaries

cd .. 

cd metallic && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*metal.so') $TOP/LinuxBinaries


