#include "place.h"
#include "manager.h"
#include <sstream>
using namespace std;

Range::Range(Map * parent)
{
	m_parent = parent;
	for (int i = 0; i < parent->nodes().size(); i++)
		m_nodes.push_back(0);
	for (int i = 0; i < parent->roads().size(); i++)
		m_roads.push_back(0);
}

void Range::loadFromString(string str)
{
	stringstream ss(str);
	int n, m;
	ss >> n >> m;
	for (int i = 0; i < n; i++) {
		int p;
		ss >> p;
		m_nodes[p] = 1;
	}
	for (int i = 0; i < m; i++) {
		int p;
		ss >> p;
		m_roads[p] = 1;
	}
	if (!ss)
		throw string("Range::loadFromString bad string inputed!");
}

void Range::loadFromSStream(stringstream & ss)
{
	int n, m;
	ss >> n >> m;
	for (int i = 0; i < n; i++) {
		int p;
		ss >> p;
		m_nodes[p] = 1;
	}
	for (int i = 0; i < m; i++) {
		int p;
		ss >> p;
		m_roads[p] = 1;
	}
	if (!ss)
		throw string("Range::loadFromString bad string inputed!");
}

bool Range::in(Position pos) const
{
	if (m_parent->posNodeId(pos) == -1) {
		return m_roads[pos.rid];
	}
	else
		return m_nodes[m_parent->posNodeId(pos)];
}

void Place::loadRange(string str)
{
	m_range.loadFromString(str);
}

void Place::loadRange(stringstream & ss)
{
	m_range.loadFromSStream(ss);
}

bool Place::in(Position pos) const
{
	return m_range.in(pos);
}

Punisher::Punisher(Manager * manager)
{
	m_manager = manager;
}

void Punisher::common(Car & car)
{
	if (m_manager->random->hitted(30)) {
		car.setSpeedRate(0.5, 5);
	}
	if (m_manager->random->hitted(50)) {
		car.setStopTurns(5);
	}
}
