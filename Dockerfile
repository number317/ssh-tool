FROM alpine
LABEL maintainer="cheon cheon0112358d@gmail.com"

ADD *.c *.h Makefile hosts.cfg /opt/ssh-tool/
ADD config /opt/ssh-tool/config
ADD draw /opt/ssh-tool/draw
ADD host /opt/ssh-tool/host
ADD utility /opt/ssh-tool/utility
RUN apk update && \
    apk add --no-cache sshpass gcc make ncurses-dev libc-dev libconfig-dev openssh vim && \
    cd /opt/ssh-tool && \
    make && \
    ln -sf $PWD/ssh-tool /usr/bin/ssh-tool && \
    mkdir -p /root/.config/ssh-tool && \
    mv hosts.cfg /root/.config/ssh-tool && \
    mkdir -p /root/.ssh/ && \
    echo "StrictHostKeyChecking no" > /root/.ssh/config && \
    apk del gcc make
ENV EDITOR vim

CMD ["/bin/sh"]
