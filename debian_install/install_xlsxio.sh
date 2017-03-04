#!/bin/bash

echo "USE /opt/sinri"
mkdir -p /opt/sinri
cd /opt/sinri

echo "INSTALL components..."
apt-get install cmake
apt-get install libzip-dev
apt-get install libexpat1-dev

echo "SOURCE INSTALL OF libzip-1.2.0"
cd /opt/sinri
wget http://www.leqeewechat.com/vainity/package/libzip-1.2.0.tar.gz
tar -zxvf libzip-1.2.0.tar.gz
cd libzip-1.2.0
./configure
make
make install

echo "SOURCE INSTALL OF expat-2.2.0"
cd /opt/sinri
wget http://www.leqeewechat.com/vainity/package/expat-2.2.0.tar.bz2
tar -jxvf expat-2.2.0.tar.bz2
cd expat-2.2.0
./configure
make
make install

echo "SOURCE INSTALL OF xlsxio"
git clone https://github.com/sinri/xlsxio.git
cd xlsxio
cmake .
make
make install

echo "OVER"