#pragma once

#include <string>
#include <vector>
#include <initializer_list>
#include <iostream>
using namespace std;

class Map;
struct Position;
class Node;
class Road;

struct Position
{
	int rid;
	int rpos;
};

class Node
{
public:
	Node(int id) { m_id = id; }
	Node(int id, initializer_list<int> l) { m_id = id; addRoads(l); }
	vector<int> roads() const { return m_roads; }
	int id() const { return m_id; }
	void addRoads(initializer_list<int> l) {
		m_roads.insert(m_roads.end(), l.begin(), l.end());
	}
private:
	int m_id;
	vector<int> m_roads;
};

class Road
{
public:
	Road(int id, int len, initializer_list<int> l) { m_id = id; m_len = len; addNodes(l); }
	vector<int> nodes() const { return m_nodes; }
	int id() const { return m_id; }
	int len() const { return m_len; }
	void addNodes(initializer_list<int> l) {
		m_nodes.insert(m_nodes.end(), l.begin(), l.end());
	}
private:
	int m_id;
	int m_len;
	vector<int> m_nodes;
};

ostream& operator<<(ostream& out, const Map& map);

class Map
{
	friend ostream& operator<<(ostream& out, const Map& map);
public:
	Map(const string& filename);
	void loadFromFile(const string& filename);
	void loadFromString(const string& str);
private:
	vector<Road> m_roads;
	vector<Node> m_nodes;
};