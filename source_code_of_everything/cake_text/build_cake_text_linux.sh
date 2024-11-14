#!/bin/bash

mkdir LinuxBinaries

TOP=$(pwd)  

chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*cake.so') $TOP/LinuxBinaries

TOP=$(pwd)  

cd ..

cd extrusion && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*extrusion299.so') $TOP/LinuxBinaries

cd .. 

cd innerglow && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*inner-glow299.so') $TOP/LinuxBinaries

cd .. 

cd threshold_alpha_2 && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*thresholdalpha2.so') $TOP/LinuxBinaries

cd .. 

cd starfield && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*starfield.so') $TOP/LinuxBinaries

cd .. 

cd bevel_bump && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*bevel299.so') $TOP/LinuxBinaries


