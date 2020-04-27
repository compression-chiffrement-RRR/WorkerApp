FROM gcc:latest

RUN apt-get -y update
RUN apt-get -y install gdb

WORKDIR /root