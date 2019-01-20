FROM alpine
LABEL maintainer="cheon cheon0112358d@gmail.com"

ADD *.c Makefile /opt/ssh-tool/
ADD config /opt/ssh-tool/config
ADD draw /opt/ssh-tool/draw
ADD host /opt/ssh-tool/host
RUN apk update && \
    apk add --no-cache sshpass gcc make ncurses-dev libc-dev libconfig-dev openssh && \
    cd /opt/ssh-tool && \
    make && \
    ln -sf $PWD/ssh-tool /usr/bin/ssh-tool && \
    mkdir -p /root/.config/ssh-tool && \
    mkdir -p /root/.ssh/ && \
    echo "StrictHostKeyChecking no" > /root/.ssh/config && \
    apk del gcc make

CMD ["/bin/sh"]
