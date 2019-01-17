FROM alpine
LABEL maintainer="cheon cheon0112358d@gmail.com"

ADD ssh-tool.c /opt/ssh-tool/
RUN apk update && \
    apk add --no-cache sshpass gcc ncurses-dev libc-dev libconfig openssh && \
    cd /opt/ssh-tool && \
    gcc -o ssh-tool ssh-tool.c -lncurses && \
    ln -sf $PWD/ssh-tool /usr/bin/ssh-tool && \
    mkdir -p /root/.config/ssh-tool && \
    mkdir -p /root/.ssh/ && \
    echo "StrictHostKeyChecking no" > /root/.ssh/config && \
    apk del gcc 

CMD ["/bin/sh"]
