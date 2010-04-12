CC=gcc
CFLAGS=-Wall -ansi

EXEC=Index
SRC=$(wildcard src/*.c)
OBJ=$(SRC:.c=.o)

all: $(EXEC)

$(EXEC): $(OBJ)
	@$(CC) -o $@ $^
	@echo "  > $@ : Compilation done."

.PHONY: clean recompile

recompile: clean all

clean:
	@rm -f $(OBJ) $(EXEC)
	@echo "  > $(OBJ) $(exec) cleaned."

%.o: %.c
	@$(CC) -c $(CFLAGS) $< -o $*.o
