FROM alpine
LABEL maintainer="cheon cheon0112358d@gmail.com"

ADD *.c *.h Makefile /opt/ssh-tool/
RUN apk update && \
    apk add --no-cache sshpass gcc make ncurses-dev libc-dev libconfig-dev openssh && \
    cd /opt/ssh-tool && \
    gcc -o ssh-tool ssh-tool.c -lncurses && \
    ln -sf $PWD/ssh-tool /usr/bin/ssh-tool && \
    mkdir -p /root/.config/ssh-tool && \
    mkdir -p /root/.ssh/ && \
    echo "StrictHostKeyChecking no" > /root/.ssh/config && \
    apk del gcc make

CMD ["/bin/sh"]
