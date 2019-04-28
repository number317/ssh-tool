CC = gcc
CFLAGS = -lncursesw -lconfig -std=c11
TARGET = ssh-tool
CGOV = -g -O0 --coverage

VPATH = config:draw:host:utility

OBJ = utility.o host.o config.o draw.o ssh-tool.o
GCNO_OBJ = $(patsubst %.o, %.gcno, $(OBJ))

$(TARGET): $(OBJ)
	$(CC) $^ $(CFLAGS) -o $@

debug: $(GCNO_OBJ)
	$(CC) $(OBJ) $(CFLAGS) $(CGOV) -o $(TARGET)

$(GCNO_OBJ):%.gcno:%.c
	$(CC) $^ $(CGOV) -c

.PHONY: clean
clean:
	rm -f *.o *.gc* $(TARGET)
