# Makefile

CXXFLAGS = -std=c++11
CXX = g++
TARGETS = board.cpp coast.cpp landprovince.cpp \
	moveparser.cpp player.cpp seaprovince.cpp
OBJS = board.o coast.o landprovince.o \
	moveparser.o player.o seaprovince.o

all: main.o $(OBJS)
	$(CXX) -o main main.o $(OBJS)

board.o: board.cpp board.hpp
	$(CXX) $(CXXFLAGS) -o board.cpp 

coast.o: coast.cpp coast.hpp
	$(CXX) $(CXXFLAGS) coast.cpp

landprovince.o: landprovince.cpp landprovince.hpp \
	province.cpp province.hpp
	$(CXX) $(CXXFLAGS) province.cpp landprovince.cpp

moveparser.o: moveparser.cpp moveparser.hpp
	$(CXX) $(CXXFLAGS) moveparser.cpp

player.o: player.cpp player.hpp
	$(CXX) $(CXXFLAGS) player.cpp

seaprovince.o: seaprovince.cpp seaprovince.hpp \
	province.cpp province.hpp
	$(CXX) $(CXXFLAGS) province.cpp seaprovince.cpp

main.o: main.cpp
	$(CXX) $(CXXFLAGS) main.cpp

clean:
	rm *.o main
