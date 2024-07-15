#!/bin/bash

mkdir LinuxBinaries

TOP=$(pwd)  

chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*outlinedeluxe.so') $TOP/LinuxBinaries

TOP=$(pwd)  

cd ..

cd ssg && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*ssg.so') $TOP/LinuxBinaries

cd .. 

cd custom_bevel && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*cbevel.so') $TOP/LinuxBinaries

