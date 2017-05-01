#pragma once

#include "map.h"
#include "place.h"
#include "car.h"
#include <random>
#include <ctime>
using namespace std;

class Random
{
public:
	Random();
	int randPoint();
	bool hitted(int prob);
	int randProb();
private:
	default_random_engine generator;
	uniform_int_distribution<int> pointDis;
	uniform_int_distribution<int> probDis;
};

class Manager
{
public:
	enum PlaceStatus {STAY, LEAVE, VISIT, PASS, NONE};
	Manager();
	Random random;
	void run();
	PlaceStatus getPlaceStatus(Place*, Path) const;
	PlaceStatus getPlaceStatus(Range, Path) const;
	void setMap(Map* map) { m_map = map; }
	Map* map() { return m_map; }
private:
	Map* m_map;
};