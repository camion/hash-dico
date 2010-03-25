C=gcc
CFLAGS=-Wall -ansi

EXEC=Hash-dico
SRC=$(wildcard src/*.c)
OBJ=$(SRC:.c=.o)

all: $(EXEC)

$(EXEC): $(OBJ)
	@$(C) -o $@ $^
	@echo "  > $@ : Compilation done."

.PHONY: clean recompile

recompile: clean all

clean:
	@rm -f $(OBJ) $(EXEC)
	@echo "  > $(OBJ) $(exec) cleaned."

%.o: %.c
	@$(C) -c $(CFLAGS) $< -o $*.o
