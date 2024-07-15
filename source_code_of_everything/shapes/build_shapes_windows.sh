#!/bin/bash

mkdir WindowsBinaries

TOP=$(pwd)  

chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*gshapes.dll') $TOP/WindowsBinaries

TOP=$(pwd)  

cd ..

cd ssg && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*ssg.dll') $TOP/WindowsBinaries

cd .. 

cd invert_transparency && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*invert-alpha.dll') $TOP/WindowsBinaries


