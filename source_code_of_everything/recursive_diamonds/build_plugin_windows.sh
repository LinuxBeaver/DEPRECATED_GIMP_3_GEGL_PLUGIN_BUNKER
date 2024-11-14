#!/bin/bash

mkdir WindowsBinaries

TOP=$(pwd)  

chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name 'diamondscircles.dll') $TOP/WindowsBinaries

cd .. 

cd port_kseg && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name 'segment-kmeans-port.dll') $TOP/WindowsBinaries

