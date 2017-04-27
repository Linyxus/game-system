#pragma once

#include <string>
#include <vector>
#include <initializer_list>
#include <iostream>
#include <algorithm>
using namespace std;

class Map;
struct Position;
class Node;
class Road;
struct Path;

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
	int greaterNode() const { return max(m_nodes[0], m_nodes[1]); }
	int smallerNode() const { return min(m_nodes[0], m_nodes[1]); }
	int anotherNode(int id) const { return (id == greaterNode()) ? smallerNode() : greaterNode(); }
private:
	int m_id;
	int m_len;
	vector<int> m_nodes;
};

struct Path
{
	vector<Position> positions;
};

ostream& operator<<(ostream& out, const Map& map);
void _DISPLAY_PATH(const Path& _p);

class Map
{
	friend ostream& operator<<(ostream& out, const Map& map);
public:
	Map(const string& filename);
	Map(){};
	void loadFromFile(const string& filename);
	void loadFromString(const string& str);
	Path caculatePath(Position s, Position e);
	Path queryPath(Position s, Position e, int step);
	int pathLen(const Path&) const;
	int lenBetweenPos(Position pos1, Position pos2) const;
	Path spfa(int s, int dest);
	Node commonNode(Road r1, Road r2) const;
	int commonRoad(Node n1, Node n2) const;
	int posNodeId(Position pos) const;
	vector<Road> roads() const { return m_roads; }
	vector<Node> nodes() const { return m_nodes; }
	Position nodeToPosition(Node node) const;
private:
	int* d;
	int* p;
	int* inq;
	vector<Road> m_roads;
	vector<Node> m_nodes;
};
