#!/bin/bash

mkdir LinuxBinaries

TOP=$(pwd)  

chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*crayontext.so') $TOP/LinuxBinaries

cd .. 

cd bevel && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*bevel.so') $TOP/LinuxBinaries

