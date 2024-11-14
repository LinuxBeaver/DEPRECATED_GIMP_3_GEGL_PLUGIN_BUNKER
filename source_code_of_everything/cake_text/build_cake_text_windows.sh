#!/bin/bash

mkdir WindowsBinaries

TOP=$(pwd)  

chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*cake.dll') $TOP/WindowsBinaries

TOP=$(pwd)  

cd ..

cd extrusion && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*extrusion299.dll') $TOP/WindowsBinaries

cd .. 

cd innerglow && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*inner-glow299.dll') $TOP/WindowsBinaries

cd .. 

cd threshold_alpha_2 && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*thresholdalpha2.dll') $TOP/WindowsBinaries

cd .. 

cd starfield && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*starfield.dll') $TOP/WindowsBinaries

cd .. 

cd bevel_bump && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*bevel299.dll') $TOP/WindowsBinaries


