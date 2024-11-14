#!/bin/bash

mkdir LinuxBinaries

TOP=$(pwd)  

chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name 'diamondscircles.so') $TOP/LinuxBinaries

cd .. 

cd port_kseg && chmod +x build_linux.sh

./build_linux.sh

cp $(find . -name 'segment-kmeans-port.so') $TOP/LinuxBinaries

