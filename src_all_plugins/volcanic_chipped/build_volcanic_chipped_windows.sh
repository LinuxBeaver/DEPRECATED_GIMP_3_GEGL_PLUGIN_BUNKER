#!/bin/bash

mkdir WindowsBinaries

TOP=$(pwd)  

chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*chippedvolcano.dll') $TOP/WindowsBinaries

cd ..

cd threshold_alpha_2 && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*thresholdalpha2.dll') $TOP/WindowsBinaries

cd .. 

cd edge_smooth && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*smoothedge.dll') $TOP/WindowsBinaries

cd .. 

cd ocean_surface && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*ocean_surface.dll') $TOP/WindowsBinaries

cd .. 

cp $(find . -name '*glassovertext.dll') $TOP/WindowsBinaries

cd .. 

cd bevel && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*bevel.dll') $TOP/WindowsBinaries



