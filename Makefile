OBJECTS = game-system/main.cpp game-system/car.cc game-system/map.cpp game-system/manager.cc game-system/place.cpp
build:
	clang++ $(OBJECTS) -o bin/a -g -std=c++11
