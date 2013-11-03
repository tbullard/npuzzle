IDIR=include
CC=gcc
CFLAGS=-g -Wall -Werror -ansi -Wpedantic -I$(IDIR)

ODIR=bin

TARGET=ai

#LDIR =../lib

LIBS=-lm

#$(LIBS) --> target end

_DEPS = arraylist.h heap.h llist.h ugf.h hashm.h search.h npuzzle.h hasht.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = arraylist.o llist.o search.o hashm.o main.o npuzzle.o hasht.o heap.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: src/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o
	rm $(TARGET)
