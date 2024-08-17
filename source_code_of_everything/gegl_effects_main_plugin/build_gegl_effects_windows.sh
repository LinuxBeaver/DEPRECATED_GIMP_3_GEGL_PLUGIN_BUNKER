#!/bin/bash

mkdir WindowsBinaries

TOP=$(pwd)  

chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*gegleffects.dll') $TOP/WindowsBinaries


cd ..

cd bevel_bump && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name 'bevel.dll') $TOP/WindowsBinaries

cd .. 

cd innerglow && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name 'inner-glow.dll') $TOP/WindowsBinaries

cd .. 

cd glass_over_text && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name 'glassovertext.dll') $TOP/WindowsBinaries

cd .. 

cd custom_bevel && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name 'cbevel.dll') $TOP/WindowsBinaries

cd .. 

cd ssg && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*ssg.dll') $TOP/WindowsBinaries

cd .. 

cd bevel_bump_sharp && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*sharpbevelbump.dll') $TOP/WindowsBinaries
