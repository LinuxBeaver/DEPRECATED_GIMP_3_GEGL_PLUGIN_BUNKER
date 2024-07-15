#!/bin/bash

mkdir LinuxBinaries

TOP=$(pwd)  

chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*rocktext2.so') $TOP/LinuxBinaries

TOP=$(pwd)  

cd ..

cd long_shadow_pixel_data && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*longshadowpd.so') $TOP/LinuxBinaries

cd .. 

cd custom_bevel && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*cbevel.so') $TOP/LinuxBinaries

cd .. 

cd edge_smooth && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*smoothedge.so') $TOP/LinuxBinaries

