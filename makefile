CFLAGS = `byteimage-config --cflags` -g -Wno-unused-result
LFLAGS = `byteimage-config --libs` `sdl-config --libs` `byteimage-config --libs`

all: toughersweeper

packing.o: packing.h packing.cpp
	$(CXX) packing.cpp -c $(CFLAGS)

field.o: packing.h field.h field.cpp
	$(CXX) field.cpp -c $(CFLAGS)

main.o: field.h main.cpp
	$(CXX) main.cpp -c $(CFLAGS)

toughersweeper: packing.o field.o main.o
	$(CXX) packing.o field.o main.o -o $@ $(LFLAGS)

run: toughersweeper
	./toughersweeper

clean:
	rm -f *.o *~ toughersweeper
