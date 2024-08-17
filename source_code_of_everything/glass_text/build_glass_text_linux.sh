#!/bin/bash

mkdir LinuxBinaries

TOP=$(pwd)  

chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*glass_hardlight.so') $TOP/LinuxBinaries

TOP=$(pwd)  

cd ..

cd glossy_balloon && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*glossy-bevel-.so') $TOP/LinuxBinaries

cd .. 

cd threshold_alpha_2 && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*thresholdalpha2.so') $TOP/LinuxBinaries

cd .. 

cd metallic && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*metal.so') $TOP/LinuxBinaries

