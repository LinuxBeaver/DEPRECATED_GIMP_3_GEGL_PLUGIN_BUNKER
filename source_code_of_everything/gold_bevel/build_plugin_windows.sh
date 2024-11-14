#!/bin/bash

mkdir WindowsBinaries

TOP=$(pwd)  

chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name 'goldbevel2.dll') $TOP/WindowsBinaries

cd ..

cd metallic && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name 'metal.dll') $TOP/WindowsBinaries

cd ..

cd sharp_bevel && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name 'sharpbevel.dll') $TOP/WindowsBinaries

cd ..

cd threshold_alpha2 && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name 'thresholdalpha2.dll') $TOP/WindowsBinaries

cd ..

cd edge_smooth && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name 'smoothedge.dll') $TOP/WindowsBinaries

