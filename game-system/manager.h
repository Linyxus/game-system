#pragma once

#include "map.h"
#include "place.h"
#include "car.h"
#include <random>
#include <vector>
#include <ctime>
using namespace std;

class Random;
struct Record;
class Recorder;
class Manager;

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

struct Record
{
	int coins, credits, length;
	double pcoins, pcredits, plength, diffic, pdiffic, prob, pprob;
};

class Recorder
{
public:
	typedef vector<Record> Records;
	enum CarStatus {OUT, REACH, RUN};
	Recorder();
	void output() const;
	void addRecord(int, Record);
	int addCar();
	void setStatus(int, CarStatus);
	CarStatus status(int) const;
private:
	vector<Records> m_records;
	vector<CarStatus> m_status;
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
	void setDest(Position pos) { m_dest = pos; }
	Position dest() const { return m_dest; }
private:
	Map* m_map;
	Position m_dest;
	vector<Place*> m_places;
	Recorder m_recorder;
	vector<Car> m_cars;
};