FROM gcc:latest

# Install cmake, google C++ test, libcurl4 
RUN apt-get -y update
RUN apt-get -y install gdb cmake googletest libcurl4

# Install Boost (SimpleAmqpClient dependency)
WORKDIR /var/tmp
RUN wget https://dl.bintray.com/boostorg/release/1.73.0/source/boost_1_73_0.tar.bz2 && \
    tar --bzip2 -xf boost_1_73_0.tar.bz2

WORKDIR /var/tmp/boost_1_73_0
RUN ./bootstrap.sh && \
    ./b2 install || true

# Install rabbitmq-c (SimpleAmqpClient dependency)
WORKDIR /var/tmp
RUN git clone https://github.com/alanxz/rabbitmq-c

WORKDIR /var/tmp/rabbitmq-c
RUN mkdir build

WORKDIR /var/tmp/rabbitmq-c/build   
RUN cmake .. && cmake --build . --target install

# Install SimpleAmqpClient
WORKDIR /var/tmp
RUN git clone https://github.com/alanxz/SimpleAmqpClient

WORKDIR /var/tmp/SimpleAmqpClient
RUN mkdir build
WORKDIR /var/tmp/SimpleAmqpClient/build
RUN cmake .. && make install

# update linker library paths.
RUN ldconfig

# build googletest from source
WORKDIR /usr/src/googletest/ 
RUN cmake . && make install

WORKDIR /root