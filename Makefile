cc = gcc
target = ssh-tool
source = ssh-tool.c host/* config/* draw/*

$(target): $(source)
	$(cc) -o $(target) -Wall -std=c11 $(source) -lncurses -lconfig

debug:
	$(cc) -g -o $(target) -Wall -std=c11 $(source) -lncurses -lconfig

clean:
	rm $(target)
