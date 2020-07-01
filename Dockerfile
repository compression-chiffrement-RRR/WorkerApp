FROM debian:latest as builder

ARG BROKER_HEARTBEAT_OVERRIDE=10

RUN apt-get -y update
RUN apt-get -y install \
    g++ \
    make \ 
    cmake \ 
    git \ 
    googletest \ 
    nlohmann-json3-dev \ 
    libblkid-dev \ 
    e2fslibs-dev \ 
    libboost-all-dev \ 
    libaudit-dev \
    wget

WORKDIR /var/tmp
RUN wget https://www.openssl.org/source/openssl-1.1.1g.tar.gz && tar -xf openssl-1.1.1g.tar.gz
WORKDIR /var/tmp/openssl-1.1.1g
RUN chmod 500 * && ./Configure linux-x86_64 && make && make install

WORKDIR /var/tmp
RUN git clone https://github.com/alanxz/rabbitmq-c
WORKDIR /var/tmp/rabbitmq-c
RUN mkdir -p build
WORKDIR /var/tmp/rabbitmq-c/build   
RUN cmake -DBUILD_SHARED_LIBS=OFF -DCMAKE_INSTALL_PREFIX=/usr/local .. && cmake --build . --target install
WORKDIR /var/tmp
RUN git clone https://github.com/alanxz/SimpleAmqpClient

WORKDIR /var/tmp/SimpleAmqpClient
RUN find . -type f -exec sed -i "s/^#define *BROKER_HEARTBEAT *0/#define BROKER_HEARTBEAT ${BROKER_HEARTBEAT_OVERRIDE}/g" {} +
RUN mkdir -p build
WORKDIR /var/tmp/SimpleAmqpClient/build
RUN cmake -DBUILD_SHARED_LIBS=OFF -DCMAKE_INSTALL_PREFIX=/usr/local .. && cmake --build . --target install

WORKDIR  /usr/src/googletest/ 
RUN cmake . && make install

WORKDIR /var/tmp
RUN git clone https://github.com/curl/curl
WORKDIR /var/tmp/curl
RUN ./buildconf && ./configure --help &&  ./configure \
    --enable-static-nss \
    --disable-shared \ 
    --enable-static \ 
    --disable-ldap \ 
    --disable-sspi \ 
    --without-librtmp \ 
    --disable-ftp \ 
    --disable-dict \ 
    --disable-telnet \ 
    --disable-tftp \ 
    --disable-rtsp \ 
    --disable-pop3 \ 
    --disable-imap \ 
    --disable-smtp \
    --disable-gopher \ 
    --disable-smb \ 
    --without-libidn && make && make install

RUN ldconfig

COPY ./ /app
WORKDIR /app
RUN make clean
RUN make test  
RUN make release

FROM debian:buster-slim

RUN apt-get update && apt-get install -y ca-certificates

COPY --from=builder /app/release/workerapp /app/workerapp
RUN chmod 500 /app/workerapp

WORKDIR /app
CMD [ "./workerapp" ]