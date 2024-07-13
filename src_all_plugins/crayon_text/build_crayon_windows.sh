#!/bin/bash

mkdir WindowsBinaries

TOP=$(pwd)  

chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*crayontext.dll') $TOP/WindowsBinaries

cd .. 

cd bevel && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*bevel.dll') $TOP/WindowsBinaries

