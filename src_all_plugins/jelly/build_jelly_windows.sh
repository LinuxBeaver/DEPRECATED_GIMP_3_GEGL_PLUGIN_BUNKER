#!/bin/bash

mkdir WindowsBinaries

TOP=$(pwd)  

chmod +x build_linux.sh

./build_linux.sh


cp $(find . -name '*goosub.dll') $TOP/WindowsBinaries

cd .. 

cd custom_bevel && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name '*cbevel.dll') $TOP/WindowsBinaries

