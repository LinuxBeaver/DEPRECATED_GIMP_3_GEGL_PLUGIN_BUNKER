#!/bin/bash

mkdir LinuxBinaries

TOP=$(pwd)  

chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*goldbevel.so') $TOP/LinuxBinaries

TOP=$(pwd)  

cd ..

cd edge_extract && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*edgeextract.so') $TOP/LinuxBinaries

cd .. 

cd edge_smooth && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*smoothedge.so') $TOP/LinuxBinaries


cd .. 

cd advance_emboss && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*makeatexture.so') $TOP/LinuxBinaries


