#pragma once

#include <vector>
#include <string>
#include <iostream>
using namespace std;

class Place;
class Range;

class Range
{
public:
	Range() {}
	Range(string str) {};
	void loadFromString(string str) {};
	//bool in(Postion) const;
private:
	//vector<Node> m_nodes;
	//vector<Road> m_roads;
};

class Place
{
public:
	Place() {}
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