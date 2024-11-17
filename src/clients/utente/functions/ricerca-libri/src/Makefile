IDIR=.
ODIR=../obj
BDIR=../bin

CC=g++
CFLAGS=-std=c++20 -I. -I$(IDIR) -I/usr/include/postgresql -lpq -lm -lhiredis

DEPS = $(wildcard $(IDIR)/*.h)
SRC = $(wildcard *.cpp)
OBJ = $(patsubst %.cpp, $(ODIR)/%.o, $(SRC))
db_OBJ=$(wildcard ../../../../../services/database/obj/*.o)
redis_OBJ=$(wildcard ../../../../../services/redis/obj/*.o)
classes_OBJ=$(wildcard ../../../../../classes/obj/*.o)
utilities_OBJ=$(wildcard ../../../../../utilities/obj/*.o)
server_OBJ=$(wildcard ../../../../../server/obj/*.o)

all:	create_dirs db redis classes utilities server $(BDIR)/main

create_dirs:
	mkdir -p $(ODIR) $(BDIR)

db:
	$(MAKE) -C ../../../../../services/database/src/

redis:
	$(MAKE) -C ../../../../../services/redis/src/

classes:
	$(MAKE) -C ../../../../../classes/src/

utilities:
	$(MAKE) -C ../../../../../utilities/src/

server:
	$(MAKE) -C ../../../../../server/src/

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BDIR)/main: $(OBJ) $(DEPS)
	$(CC) -o $@ $(OBJ) $(db_OBJ) $(redis_OBJ) $(classes_OBJ) $(utilities_OBJ) $(server_OBJ) $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ $(IDIR)/*~ core
