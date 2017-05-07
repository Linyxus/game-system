#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include "map.h"
#include "car.h"
class Manager;
using namespace std;

class Punisher;
class Place;
class Range;
class CrossRoad;
class Slower;

class Punisher
{
public:
	Punisher(Manager* manager);
	void common(Car&);
private:
	Manager* m_manager;
};

class Range
{
public:
	Range(Map* parent);
	void loadFromString(string str);
	void loadFromSStream(stringstream & ss);
	bool in(Position) const;
private:
	Map* m_parent;
	vector<int> m_nodes;
	vector<int> m_roads;
};

class Place
{
public:
	Place(Map* parent) : m_range(parent) {}
	Place(Map* parent, Punisher* punisher) : m_range(parent), punish(punisher) {}
	void loadRange(string str);
	void loadRange(stringstream & ss);
	Range range() const { return m_range; }
	virtual void onPass(Car&, Path) = 0;
	virtual void onVisit(Car&, Path) = 0;
	virtual void onLeave(Car&, Path) = 0;
	virtual void onStay(Car&, Path) = 0;
	virtual double diffic() const { return 0.0; };
	bool in(Position) const;
protected:
	Range m_range;
	Punisher* punish;
};

class CrossRoad : public Place
{
public:
	CrossRoad(Map* parent, Punisher* punisher) : Place(parent, punisher) {}
	CrossRoad(Map* parent) : Place(parent) {}
	void onPass(Car&, Path) {
		//cout << "Car pass." << endl;
	}
	void onVisit(Car& car, Path) {
		//cout << "!!!Car visit!!!" << endl;
		if (car.decideEvent("RED_LIGHT"))
			punish->common(car);
		else
			car.setStopTurns(2);
	}
	void onLeave(Car&, Path) {
		//cout << "Car Leave." << endl;
	}
	void onStay(Car&, Path) {
		//cout << "Car stay." << endl;
	}
};

class Slower : public Place
{
public:
	Slower(Map* parent, Punisher* punisher) : Place(parent, punisher) {}
	Slower(Map* parent) : Place(parent) {}
	void onPass(Car&, Path) {
		//cout << "Car pass." << endl;
	}
	void onVisit(Car& car, Path) {
		//cout << "!!!Car visit!!!" << endl;
		car.setSpeedRate(0.5, 1000);
	}
	void onLeave(Car& car, Path) {
		//cout << "Car Leave." << endl;
		car.setSpeedRate(0.5, 0);
	}
	void onStay(Car&, Path) {
		//cout << "Car stay." << endl;
	}
};