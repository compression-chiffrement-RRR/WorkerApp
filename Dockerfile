FROM gcc:latest

ARG BOOST_VERSION=boost_1_73_0

# Install cmake and google C++ test
RUN apt-get -y update
RUN apt-get -y install gdb cmake googletest

# Install Boost (SimpleAmqpClient dependency)
WORKDIR /var/tmp
RUN wget https://dl.bintray.com/boostorg/release/1.73.0/source/${BOOST_VERSION}.tar.bz2 && \
    tar --bzip2 -xf ${BOOST_VERSION}.tar.bz2

WORKDIR ${BOOST_VERSION}
RUN ./bootstrap.sh && \
    ./b2 install || true

# Install rabbitmq-c (SimpleAmqpClient dependency)
WORKDIR /var/tmp
RUN git clone https://github.com/alanxz/rabbitmq-c

WORKDIR /var/tmp/rabbitmq-c
RUN mkdir build
WORKDIR /var/tmp/rabbitmq-c/build
RUN pwd && ls -la && cmake .. && cmake --build . --target install

# Install SimpleAmqpClient
WORKDIR /var/tmp
RUN git clone https://github.com/alanxz/SimpleAmqpClient

WORKDIR /var/tmp/SimpleAmqpClient
RUN mkdir build
WORKDIR /var/tmp/SimpleAmqpClient/build
RUN cmake .. && make install

RUN ldconfig

# build googletest from source
WORKDIR /usr/src/googletest/ 
RUN cmake . && make install

WORKDIR /root