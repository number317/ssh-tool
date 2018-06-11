cc = gcc
target = ssh-tool
source = ssh-tool.c

$(target): $(source)
	$(cc) -o $(target) -Wall -std=gnu99 $(source) -lncurses

clean:
	rm $(target)
