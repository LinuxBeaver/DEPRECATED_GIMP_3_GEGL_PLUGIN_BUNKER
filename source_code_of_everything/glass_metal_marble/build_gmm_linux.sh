#!/bin/bash

mkdir LinuxBinaries

TOP=$(pwd)  

chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*goldmetallicmeme.so') $TOP/LinuxBinaries

TOP=$(pwd)  

cd ..

cd bevel_bump && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*bevel.so') $TOP/LinuxBinaries

cd .. 

cd custom_bevel && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*cbevel.so') $TOP/LinuxBinaries

