cc = gcc
target = main
source = main.c node.h host.h node.c host.c

$(target): $(source)
	$(cc) -o $(target) -Wall -std=c11 $(source) -lncurses

debug:
	$(cc) -g -o $(target) -Wall -std=c11 $(source) -lncurses

clean:
	rm $(target)
