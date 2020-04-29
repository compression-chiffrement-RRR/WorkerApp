FROM gcc:latest

RUN apt-get -y update
RUN apt-get -y install gdb cmake googletest

WORKDIR /usr/src/googletest/ 

RUN cmake . && make install

WORKDIR /root