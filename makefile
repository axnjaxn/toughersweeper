CFLAGS = `byteimage-config --cflags` -g -Wno-unused-result
LFLAGS = `byteimage-config --libs` `sdl-config --libs` `byteimage-config --libs`

all: toughersweeper

field.o: field.h field.cpp
	$(CXX) field.cpp -c $(CFLAGS)

main.o: field.h main.cpp
	$(CXX) main.cpp -c $(CFLAGS)

toughersweeper: field.o main.o
	$(CXX) field.o main.o -o $@ $(LFLAGS)

run: toughersweeper
	./toughersweeper

clean:
	rm -f *.o *~ toughersweeper
