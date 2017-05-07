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
	void output(string fn) const;
	void append(string fn) const;
	void addRecord(int, Record);
	int addCar();
	void setStatus(int, CarStatus);
	void setRanking(int, int);
	CarStatus status(int) const;
	int ranking(int) const;
	int result(int) const;
	int turns(int) const;
	void newTurn(int);
private:
	vector<Records> m_records;
	vector<CarStatus> m_status;
	vector<int> m_turns;
	vector<int> m_rankings;
};

class Manager
{
public:
	enum PlaceStatus {STAY, LEAVE, VISIT, PASS, NONE};
	Manager();
	~Manager();
	Random * random;
	void run();
	void init(Map* map, int cars, vector<Position> poss, vector<Controller*> ctls, Car initialCar);
	void initFromFile(Map *, string);
	PlaceStatus getPlaceStatus(Place*, Path) const;
	PlaceStatus getPlaceStatus(Range, Path) const;
	Map* m_map;
	vector<Position> dest;
	double caculateDiffic(Path) const;
	void _DISPLAY_CARS() const;
	vector<Car> cars() const { return m_cars; }
	const Recorder& recorder() const { return m_recorder; }
	void addPlace(Place* place) { m_places.push_back(place); }
	void _SET_PROB(int index, double prob) { m_cars[index].controller->prob = prob; }
private:
	vector<Place*> m_places;
	Recorder m_recorder;
	vector<Car> m_cars;
	vector<Controller*> m_ctls;
};