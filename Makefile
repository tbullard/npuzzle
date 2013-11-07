IDIR=include
CC=gcc
CFLAGS=-g -Wall -Werror -ansi -Wpedantic -I$(IDIR)

ODIR=bin

TARGET=ai

#LDIR =../lib

#LIBS=

#$(LIBS) --> target end

_DEPS = arraylist.h heap.h llist.h ugf.h hashm.h search.h npuzzle.h hasht.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = arraylist.o llist.o search.o hashm.o main.o npuzzle.o hasht.o heap.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))
OCTO = $(shell [ -d "$(ODIR)" ] || mkdir -p $(ODIR))

$(ODIR)/%.o: src/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OCTO) $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -r $(ODIR)
	rm $(TARGET)
