#!/bin/bash

mkdir WidndowsBinaries

TOP=$(pwd)  

chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*gegleffects299.dll') $TOP/WindowsBinaries

TOP=$(pwd)  

cd ..

cd bevel && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*bevel.dll') $TOP/WindowsBinaries

cd .. 

cd innerglow && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*inner-glow299.dll') $TOP/WindowsBinaries

cd .. 

cd glass_over_text && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*glassovertext.dll') $TOP/WindowsBinaries

cd .. 

cd zzstrokebevel && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*zzstrokebevel299.dll') $TOP/WindowsBinaries
