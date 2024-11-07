#IDIR=../hdr
IDIR=.
ODIR=../obj
BDIR=../bin

CC=g++
# use this when connecting to Postgresql (library lpq)
#CFLAGS=-std=c++11 -I. -I$(IDIR) -I/usr/include/postgresql -lpq -lm 
CFLAGS=-std=c++20 -I. -I$(IDIR) -I/usr/include/postgresql -lpq -lm -lhiredis

# use this when no connection to Postgresql (library lpq) is needed
#CFLAGS=-std=c++11 -I. -I$(IDIR) -lm 

DEPS = $(wildcard $(IDIR)/*.h)
#OBJ = $(wildcard $(ODIR)/*.o)
SRC = $(wildcard *.cpp)
OBJ = $(patsubst %.cpp, $(ODIR)/%.o, $(SRC))
db_OBJ=$(wildcard ../../../../services/database/obj/*.o)
redis_OBJ=$(wildcard ../../../../services/redis/obj/*.o)
server_OBJ=$(wildcard ../../../../server/obj/*.o)

all:	create_dirs db redis server $(BDIR)/main

create_dirs:
	mkdir -p $(ODIR) $(BDIR)

db:
	$(MAKE) -C ../../../../services/database/src/

redis:
	$(MAKE) -C ../../../../services/redis/src/

server:
	$(MAKE) -C ../../../../server/src/

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BDIR)/main: $(OBJ) $(DEPS)
	$(CC) -o $@ $(OBJ) $(db_OBJ) $(redis_OBJ) $(server_OBJ) $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ $(IDIR)/*~ core 