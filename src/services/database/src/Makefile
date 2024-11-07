
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


#all:	$(ODIR)/%.o

all:	create_dirs $(ODIR)/$(OBJ)

create_dirs:
	mkdir -p $(ODIR) 

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ $(IDIR)/*~ core
