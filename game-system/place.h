#pragma once

#include <vector>
#include <string>
#include <iostream>
#include "map.h"
using namespace std;

class Place;
class Range;
class CrossRoad;
class Slower;

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
	Place(Map* parent) : m_range(parent) {}
	//void loadRange(string str);
	virtual void onPassBy() = 0;
	virtual void onVisited() = 0;
	bool in() const {
		return true;
	}
protected:
	Range m_range;
};

class CrossRoad : public Place
{
public:
	void onPassBy() {
		cout << "A car pass by CrossRoad!" << endl;
	}
	void onVisited() {
		m_range.loadFromString("123");
		cout << "A car visited CrossRoad!" << endl;
	}
};

class Slower : public Place
{
public:
	void onPassBy() {
		cout << "A car pass by Slower!" << endl;
	}
	void onVisited() {
		m_range.loadFromString("123");
		cout << "A car visited Slower!" << endl;
	}
};