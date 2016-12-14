# Makefile

CXXFLAGS = -std=c++11
CXX = g++
TARGETS = board.cpp coast.cpp game.cpp\
	landprovince.cpp resolver.cpp moveparser.cpp \
	player.cpp province.cpp seaprovince.cpp
OBJS = board.o coast.o game.o  \
	landprovince.o resolver.o moveparser.o \
	player.o province.o seaprovince.o

all: main.o $(OBJS)
	$(CXX) -o main main.o $(OBJS)

resolver: coast.o province.o landprovince.o seaprovince.o \
	player.o resolver.o
	$(CXX) -o resolver province.o landprovince.o seaprovince.o \
	coast.o player.o resolver.o

board.o: board.cpp board.hpp
	$(CXX) -c $(CXXFLAGS) board.cpp 

coast.o: coast.cpp coast.hpp
	$(CXX) -c $(CXXFLAGS) coast.cpp

landprovince.o: landprovince.cpp landprovince.hpp \
	province.cpp province.hpp
	$(CXX) -c $(CXXFLAGS) province.cpp landprovince.cpp

moveparser.o: moveparser.cpp moveparser.hpp
	$(CXX) -c $(CXXFLAGS) moveparser.cpp

player.o: player.cpp player.hpp
	$(CXX) -c $(CXXFLAGS) player.cpp

province.o: province.cpp province.hpp
	$(CXX) -c $(CXXFLAGS) province.cpp

resolver.o: resolver.cpp resolver.hpp
	$(CXX) -c $(CXXFLAGS) resolver.cpp

seaprovince.o: seaprovince.cpp seaprovince.hpp \
	province.cpp province.hpp
	$(CXX) -c $(CXXFLAGS) province.cpp seaprovince.cpp

main.o: main.cpp
	$(CXX) -c $(CXXFLAGS) main.cpp

clean:
	rm *.o main
