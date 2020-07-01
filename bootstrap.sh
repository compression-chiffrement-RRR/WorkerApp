#!/bin/sh
set -e

# Install cmake, google C++ test, libcurl4 
apt-get -y update
apt-get -y install gcc g++ build-essential git gdb cmake googletest libcurl4 libcurl4-openssl-dev nlohmann-json3-dev libssl-dev

# RUN update-alternatives --install /usr/bin/gfortran gfortran /usr/local/bin/gfortran 999
apt-get -y install --reinstall libboost-all-dev libboost-chrono1.67.0 libboost-chrono1.67-dev

# Install rabbitmq-c (SimpleAmqpClient dependency)
cd /var/tmp
git clone https://github.com/alanxz/rabbitmq-c

cd /var/tmp/rabbitmq-c
mkdir build

cd /var/tmp/rabbitmq-c/build   
cmake -ENABLE_SSL_SUPPORT=OFF .. && cmake --build . --target install

# Install SimpleAmqpClient
cd /var/tmp
git clone https://github.com/alanxz/SimpleAmqpClient

cd /var/tmp/SimpleAmqpClient
find . -type f -exec sed -i 's/^#define *BROKER_HEARTBEAT *0/#define BROKER_HEARTBEAT 10/g' {} +
cat src/ChannelImpl.cpp
mkdir build
cd /var/tmp/SimpleAmqpClient/build
cmake .. && make install

# update linker library paths.
ldconfig

# build googletest from source
cd /usr/src/googletest/ 
cmake . && make install

cd /root

make test && make release && chmod 500 /root/release/workerapp

apt remove -y gcc g++ build-essential git gdb cmake googletest libcurl4-openssl-dev nlohmann-json3-dev libssl-dev libboost-chrono1.67-dev libboost-all-dev
apt autoremove -y
rm -rf /var/tmp