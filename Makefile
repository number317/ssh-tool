cc = gcc
target = ssh-tool
source = ssh-tool.c ssh-tool.h host/* config/* draw/* utility/*

$(target): $(source)
	$(cc) -o $(target) -Wall -std=c11 $(source) -lncursesw -lconfig

debug:
	$(cc) -g -o $(target) -Wall -std=c11 $(source) -lncursesw -lconfig

clean:
	rm $(target)
