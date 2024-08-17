#!/bin/bash

mkdir LinuxBinaries

TOP=$(pwd)  

chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*gegleffects.so') $TOP/LinuxBinaries


cd ..

cd bevel_bump && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name 'bevel.so') $TOP/LinuxBinaries

cd .. 

cd innerglow && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name 'inner-glow.so') $TOP/LinuxBinaries

cd .. 

cd glass_over_text && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name 'glassovertext.so') $TOP/LinuxBinaries

cd .. 

cd custom_bevel && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name 'cbevel.so') $TOP/LinuxBinaries

cd .. 

cd ssg && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*ssg.so') $TOP/LinuxBinaries

cd .. 

cd bevel_bump_sharp && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*sharpbevelbump.so') $TOP/LinuxBinaries
