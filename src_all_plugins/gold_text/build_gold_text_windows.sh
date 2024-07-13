#!/bin/bash

mkdir WindowsBinaries

TOP=$(pwd)  

chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*goldbevel.dll') $TOP/WindowsBinaries

TOP=$(pwd)  

cd ..

cd edge_extract && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*edgeextract.dll') $TOP/WindowsBinaries

cd .. 

cd edge_smooth && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*smoothedge.dll') $TOP/WindowsBinaries


cd .. 

cd advance_emboss && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*makeatexture.dll') $TOP/WindowsBinaries


