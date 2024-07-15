#!/bin/bash

mkdir LinuxBinaries

TOP=$(pwd)  

chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*gegleffects299.so') $TOP/LinuxBinaries

TOP=$(pwd)  

cd ..

cd bevel && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*bevel.so') $TOP/LinuxBinaries

cd .. 

cd innerglow && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*inner-glow299.so') $TOP/LinuxBinaries

cd .. 

cd glass_over_text && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*glassovertext.so') $TOP/LinuxBinaries

cd .. 

cd zzstrokebevel && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*zzstrokebevel299.so') $TOP/LinuxBinaries
