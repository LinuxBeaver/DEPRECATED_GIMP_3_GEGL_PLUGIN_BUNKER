#!/bin/bash

mkdir LinuxBinaries

TOP=$(pwd)  

chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*gshapes.so') $TOP/LinuxBinaries

TOP=$(pwd)  

cd ..

cd ssg && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*ssg.so') $TOP/LinuxBinaries

cd .. 

cd invert_transparency && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*invert-alpha.so') $TOP/LinuxBinaries


