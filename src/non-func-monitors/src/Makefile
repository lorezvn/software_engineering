
IDIR=.
ODIR= ../obj
BDIR= ../bin

CC=g++
CFLAGS=-std=c++11 -I. -I$(IDIR) -I/usr/include/postgresql -lpq -lm
#CFLAGS=-I. -I/usr/include/postgresql -lpq -lm
#CFLAGS=-I. -lm

DEPS = $(wildcard $(IDIR)/*.h)
#OBJ = $(wildcard $(ODIR)/*.o)
SRC = $(wildcard *.cpp)
OBJ = $(patsubst %.cpp, $(ODIR)/%.o, $(SRC))
db_OBJ=$(wildcard ../../services/database/obj/*.o)

#all:	$(ODIR)/%.o

all:	create_dirs db $(BDIR)/main

create_dirs:
	mkdir -p $(ODIR) $(BDIR)

db:
	$(MAKE) -C ../../services/database/src/

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BDIR)/main: $(OBJ) $(DEPS)
	$(CC) -o $@ $(OBJ) $(db_OBJ) $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ $(IDIR)/*~ core
