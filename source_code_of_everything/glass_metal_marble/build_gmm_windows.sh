#!/bin/bash

mkdir WindowsBinaries

TOP=$(pwd)  

chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*goldmetallicmeme.dll') $TOP/WindowsBinaries

TOP=$(pwd)  

cd ..

cd bevel_bump && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*bevel.dll') $TOP/WindowsBinaries

cd .. 

cd custom_bevel && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*cbevel.dll') $TOP/WindowsBinaries

