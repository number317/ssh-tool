cc = gcc
target = ssh-tool
source = ssh-tool.c

$(target): $(source)
	$(cc) -o $(target) -Wall -std=c11 $(source) -lncursesw

debug:
	$(cc) -g -o $(target) -Wall -std=c11 $(source) -lncursesw

clean:
	rm $(target)
