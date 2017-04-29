#pragma once

#include <vector>
#include <string>
#include <iostream>
#include "map.h"
#include "car.h"
#include "manager.h"
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
	void common(Car);
private:
	Manager* m_manager;
};

class Range
{
public:
	Range(Map* parent);
	void loadFromString(string str);
	bool in(Position) const;
private:
	Map* m_parent;
	vector<int> m_nodes;
	vector<int> m_roads;
};

class Place
{
public:
	Place(Map* parent, Punisher* punisher) : m_range(parent), punish(punisher) {}
	void loadRange(string str);
	virtual void onPass(Car, Path) = 0;
	virtual void onVisit(Car, Path) = 0;
	virtual void onLeave(Car, Path) = 0;
	bool in(Position) const;
protected:
	Range m_range;
	Punisher* punish;
};

class CrossRoad : public Place
{
public:
	void onPass(Car, Path) {
		cout << "A car pass by CrossRoad!" << endl;
	}
	void onVisited(Car, Path) {
		m_range.loadFromString("123");
		cout << "A car visited CrossRoad!" << endl;
	}
};

class Slower : public Place
{
public:
	void onPass(Car, Path) {
		cout << "A car pass by Slower!" << endl;
	}
	void onVisit(Car, Path) {
		m_range.loadFromString("123");
		cout << "A car visited Slower!" << endl;
	}
};