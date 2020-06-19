FROM debian:latest

COPY ./ /root
RUN rm -rf /root/debug /root/release /root/tests/bin

COPY ./bootstrap.sh /usr/local/bin/bootstrap.sh
RUN bootstrap.sh

CMD [ "/root/release/workerapp" ]