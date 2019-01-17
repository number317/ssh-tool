cc = gcc
target = ssh-tool
source = ssh-tool.c host.h host.c config.h config.c draw.h draw.c

$(target): $(source)
	$(cc) -o $(target) -Wall -std=c11 $(source) -lncurses -lconfig

debug:
	$(cc) -g -o $(target) -Wall -std=c11 $(source) -lncurses -lconfig

clean:
	rm $(target)
