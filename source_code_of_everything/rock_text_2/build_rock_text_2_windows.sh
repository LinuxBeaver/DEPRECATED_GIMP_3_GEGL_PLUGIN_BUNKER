#!/bin/bash

mkdir WindowsBinaries

TOP=$(pwd)  

chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*rocktext2.dll') $TOP/WindowsBinaries

TOP=$(pwd)  

cd ..

cd long_shadow_pixel_data && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*longshadowpd.dll') $TOP/WindowsBinaries

cd .. 

cd custom_bevel && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*cbevel.dll') $TOP/WindowsBinaries

cd .. 

cd edge_smooth && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*smoothedge.dll') $TOP/WindowsBinaries

