cc = gcc
target = ssh-tool
source = ssh-tool.c host/* config/* draw/*

$(target): $(source)
	$(cc) -o $(target) -Wall -std=c11 $(source) -lncursesw -lconfig

debug:
	$(cc) -g -o $(target) -Wall -std=c11 $(source) -lncursesw -lconfig

clean:
	rm $(target)
