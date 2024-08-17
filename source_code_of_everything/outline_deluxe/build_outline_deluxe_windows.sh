#!/bin/bash

mkdir WindowsBinaries

TOP=$(pwd)  

chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*outlinedeluxe.dll') $TOP/WindowsBinaries

TOP=$(pwd)  

cd ..

cd ssg && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*ssg.dll') $TOP/WindowsBinaries

cd .. 

cd custom_bevel && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*cbevel.dll') $TOP/WindowsBinaries

