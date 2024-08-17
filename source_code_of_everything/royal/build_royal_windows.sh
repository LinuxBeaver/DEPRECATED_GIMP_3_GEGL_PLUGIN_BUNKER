#!/bin/bash

mkdir WindowsBinaries

TOP=$(pwd)  

chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*royal.dll') $TOP/WindowsBinaries

TOP=$(pwd)  

cd ..

cd extrusion && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*extrusion.dll') $TOP/WindowsBinaries

cd .. 

cd edge_smooth && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*smoothedge.dll') $TOP/WindowsBinaries

cd .. 

cd threshold_alpha_2 && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*thresholdalpha2.dll') $TOP/WindowsBinaries

cd .. 

cd glass_over_text && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*glassovertext.dll') $TOP/WindowsBinaries

cd .. 

cd metallic && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*metal.dll') $TOP/WindowsBinaries


