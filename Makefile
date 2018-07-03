cc = gcc
target = ssh-tool
source = ssh-tool.c

$(target): $(source)
	$(cc) -o $(target) -Wall -std=c11 $(source) -lncurses

debug:
	$(cc) -g -o $(target) -Wall -std=c11 $(source) -lncurses

clean:
	rm $(target)
