#include "map.h"
#include <sstream>
#include <queue>
#include <fstream>
#include <algorithm>
#include <iostream>
using namespace std;

/* Map */
Map::Map(const string& filename)
{
	loadFromFile(filename);
}

Position Map::nodeToPosition(Node node) const
{
    Position pos;
    pos.rid = node.roads()[0];
    if (m_roads[pos.rid].smallerNode() == node.id())
        pos.rpos = 0;
    else
        pos.rpos = m_roads[pos.rid].len();
    return pos;
}

bool Map::posInPath(Position position, Path path) const
{
	if (path.positions.size() == 0) {
		return false;
	}
	if (path.positions.size() == 1) {
		Position pos = path.positions[0];
		if (posNodeId(position) == -1 && posNodeId(pos) == -1) {
			return (position.rid == pos.rid) && (position.rpos == pos.rpos);
		}
		if (posNodeId(position) != -1 && posNodeId(pos) != -1) {
			return posNodeId(position) == posNodeId(pos);
		}
		else {
			return false;
		}
	}
	if (path.positions.size() == 2) {
		Position pos1 = path.positions[0];
		Position pos2 = path.positions[1];
		int nid1 = posNodeId(pos1);
		int nid2 = posNodeId(pos2);
		if (posNodeId(position) != -1) {
			return (posNodeId(position) == nid1) || (posNodeId(position) == nid2);
		}
		if (nid1 != -1 && nid2 != -1) {
			int rid = commonRoad(m_nodes[nid1], m_nodes[nid2]);
			return position.rid == rid;
		}
		else if (nid1 == -1 && nid2 == -1) {
			if (pos1.rid != pos2.rid) {
				throw string("Map::posInPath illigal positions besides");
			}
			if (position.rid != pos1.rid) {
				return false;
			}
			else {
				return (position.rpos >= min(pos1.rpos, pos2.rpos)) && (position.rpos <= max(pos1.rpos, pos2.rpos));
			}
		}
		else {
			Node node;
			Position pos;
			Road road;
			if (posNodeId(pos1) != -1) {
				node = m_nodes[posNodeId(pos1)];
				pos = pos2;
				road = m_roads[pos.rid];
			}
			else {
				node  = m_nodes[posNodeId(pos2)];
				pos = pos1;
				road = m_roads[pos.rid];
			}
			if (position.rid != road.id()) {
				return false;
			}
			if (node.id() == road.smallerNode()) {
				return position.rpos <= pos.rpos;
			}
			else if (node.id() == road.greaterNode()) {
				return position.rpos >= pos.rpos;
			}
			else {
				throw string("Map::posInPath: illigal positions besides");
			}
		}
	}
	//here, path length >= 3
	for (int i = 0; i < path.positions.size() - 1; i++) {
		Path t;
		t.positions.push_back(path.positions[0]);
		t.positions.push_back(path.positions[1]);
		if (posInPath(position, t))
			return true;
	}
	return false;
}

bool Map::posEqual(Position l, Position r) const
{
	if (posNodeId(l) == -1 && posNodeId(r) == -1) {
		return (l.rid == r.rid) && (l.rpos == r.rpos);
	}
	else if (posNodeId(l) != -1 && posNodeId(r) != -1) {
		return posNodeId(l) == posNodeId(r);
	}
	else {
		return false;
	}
}

void Map::loadFromFile(const string& filename)
{
	std::ifstream t(filename);
	std::stringstream buffer;
	buffer << t.rdbuf();
	std::string contents(buffer.str());
	loadFromString(contents);
}

void Map::loadFromString(const string& str)
{
	stringstream ss(str);
	int n;
	ss >> n;
	for (int i = 0; i < n; i++) {
		m_nodes.push_back(Node(i));
	}
	int u, v, w;
	int cnt = 0;
	while (ss >> u >> v >> w) {
		m_roads.push_back(Road(cnt, w, { u, v }));
		m_nodes[u].addRoads({ cnt });
		m_nodes[v].addRoads({ cnt });
		cnt++;
	}
}

Path Map::spfa(int s, int dest)
{
	if (dest == s) {
        Path p;
        p.positions.push_back(nodeToPosition(m_nodes[s]));
        return p;
	}
	queue<int> q;
	int n = m_nodes.size();
	vector<int> d, p, inq;
	for (int i = 0; i < n; i++) {
		d.push_back(-1);
		p.push_back(-1);
		inq.push_back(0);
	}
	d[s] = 0;
	inq[s] = true;
	q.push(s);
	while (!q.empty()) {
		int u = q.front(); q.pop();
		inq[u] = false;
		Node node = m_nodes[u];
		for (int i = 0; i < node.roads().size(); i++) {
			Road road = m_roads[node.roads()[i]];
			int v = road.anotherNode(u);
			if (d[v] == -1 || d[v] > d[u] + road.len()) {
				d[v] = d[u] + road.len();
				p[v] = u;
				if (!inq[v]) {
					q.push(v);
					inq[v] = true;
				}
			}
		}
	}
	Path path;
	vector<int> nodes;
	for (int u = dest; u != s; u = p[u]) {
		nodes.push_back(u);
	}
	nodes.push_back(s);
	for (int i = nodes.size() - 1; i >= 0; i--) {
        path.positions.push_back(nodeToPosition(m_nodes[nodes[i]]));
	}
	return path;
}

Node Map::commonNode(Road r1, Road r2) const
{
	Node node(-1);
	if (r1.greaterNode() == r2.greaterNode())
		node = Node(r1.greaterNode());
	else if (r1.greaterNode() == r2.smallerNode())
		node = Node(r1.greaterNode());
	else if (r1.smallerNode() == r2.greaterNode())
		node = Node(r1.smallerNode());
	else if (r1.smallerNode() == r2.smallerNode())
		node = Node(r1.smallerNode());
	return node;
}

int Map::commonRoad(Node n1, Node n2) const
{
	int n = m_roads.size();
	vector<int> vis;
	for (int i = 0; i < n; i++) {
        vis.push_back(0);
	}
	for (int i = 0; i < n1.roads().size(); i++) {
		vis[n1.roads()[i]] = 1;
	}
	for (int i = 0; i < n2.roads().size(); i++) {
		if (vis[n2.roads()[i]])
			return n2.roads()[i];
	}
	return -1;
}

int Map::posNodeId(Position pos) const
{
	Road road = m_roads[pos.rid];
	if (pos.rpos == 0)
		return road.smallerNode();
	else if (pos.rpos == road.len())
		return road.greaterNode();
	else
		return -1;
}

void _DISPLAY_PATH(const Path& _p)
{
    cout << "## Debug # Path" << endl;
    for (int i = 0; i < _p.positions.size(); i++) {
        cout << "Position #RID " << _p.positions[i].rid << " #RPOS " << _p.positions[i].rpos << endl;
    }
    cout << endl;
}

Path Map::caculatePath(Position s, Position e)
{
	int nid1 = m_roads[s.rid].greaterNode();
	int nid2 = m_roads[s.rid].smallerNode();
	int dest1 = m_roads[e.rid].smallerNode();
	int dest2 = m_roads[e.rid].greaterNode();
	vector<Path> paths;
	paths.push_back(spfa(nid1, dest1));
	paths.push_back(spfa(nid2, dest1));
	paths.push_back(spfa(nid1, dest2));
	paths.push_back(spfa(nid2, dest2));
	int minlen = -1;
	Path minPath;
	for (int i = 0; i < paths.size(); i++) {
		Path path = paths[i];
		if (posNodeId(path.positions[0]) != posNodeId(s)) {
            path.positions.insert(path.positions.begin(), s);
		}
		if (posNodeId(path.positions[path.positions.size()-1]) != posNodeId(e)) {
            path.positions.push_back(e);
		}
		int len = pathLen(path);
		if (minlen == -1 || minlen > len) {
			minlen = len;
			minPath = path;
		}
	}
	return minPath;
}

Path Map::queryPath(Position s, Position e, int step)
{
	Path path = caculatePath(s, e);
	if (pathLen(path) <= step)
		return path;
	Path ret;
	int cur = 0;
	while (pathLen(ret) < step) {
		ret.positions.push_back(path.positions[cur++]);
	}
	if (pathLen(ret) == step)
		return ret;
	Position pathend = *(--ret.positions.end());
	ret.positions.pop_back();
	Position npos = *(--ret.positions.end());
	if (posNodeId(pathend) != -1 && posNodeId(npos) != -1) {
		Road r = m_roads[commonRoad(m_nodes[posNodeId(pathend)], m_nodes[posNodeId(npos)])];
		if (r.smallerNode() == posNodeId(npos)) {
			npos.rid = r.id();
			npos.rpos = step - pathLen(ret);
		}
		else {
			npos.rid = r.id();
			npos.rpos = r.len() - (step - pathLen(ret));
		}
		ret.positions.push_back(npos);
	}
	else if (posNodeId(pathend) == -1 && posNodeId(npos) == -1) {
		npos.rpos += pathend.rpos - npos.rpos;
		ret.positions.push_back(npos);
	}
	else {
		if (posNodeId(pathend) == -1) {
			Node node = m_nodes[posNodeId(npos)];
			Road road = m_roads[pathend.rid];
			if (node.id() == road.smallerNode()) {
				npos.rid = pathend.rid;
				npos.rpos = step - pathLen(ret);
			}
			else if (node.id() == road.greaterNode()) {
				npos.rid = pathend.rid;
				npos.rpos = road.len() - (step - pathLen(ret));
			}
			else {
				throw string("queryPath: path not legal. The two positions beside are not on the same raod.");
			}
		}
		else {
			Node node = m_nodes[posNodeId(pathend)];
			Road road = m_roads[npos.rid];
			if (node.id() == road.smallerNode()) {
				npos.rpos -= step - pathLen(ret);
			}
			else if (node.id() == road.greaterNode()) {
				npos.rpos += step - pathLen(ret);
			}
			else {
				throw string("queryPath: path not legal. The two positions beside are not on the same raod.");
			}
		}
		ret.positions.push_back(npos);
	}
	return ret;
}

int Map::pathLen(const Path &p) const //calculate the path length
{
	int ret = 0;
	vector<Position> poss = p.positions;
	if (poss.size() == 0)
		return -1;
	Position prev = poss[0];
	for (int i = 1; i < poss.size(); i++) {
		Position pos = poss[i];
		ret += lenBetweenPos(prev, pos);
		prev = pos;
	}
	return ret;
}

int Map::lenBetweenPos(Position pos1, Position pos2) const
{
	int nid1 = posNodeId(pos1);
	int nid2 = posNodeId(pos2);
	if (nid1 != -1 && nid2 != -1) {
		int rid = commonRoad(m_nodes[nid1], m_nodes[nid2]);
		return m_roads[rid].len();
	}
	else if (nid1 == -1 && nid2 == -1) {
		if (pos1.rid != pos2.rid) {
			throw string("function lenBetweenPos: Positions not legal");
		}
		Road road = m_roads[pos1.rid];
		return abs(pos1.rpos - pos2.rpos);
	}
	else {
		if (nid1 == -1) {
			swap(nid1, nid2);
			swap(pos1, pos2);
		}
		Node node = m_nodes[nid1];
		int rid = -1;
		for (int i = 0; i < node.roads().size(); i++) {
			if (node.roads()[i] == pos2.rid)
				rid = pos2.rid;
		}
		if (rid == -1) {
			throw string("function lenBetweenPos: Positions not legal");
		}
		if (node.id() == m_roads[rid].greaterNode()) {
			return m_roads[rid].len() - pos2.rpos;
		}
		else {
			return pos2.rpos;
		}
	}
}

ostream & operator<<(ostream & out, const Map & map)
{
	out << "### Test Data ### Map" << endl
		<< "### Roads" << endl;
	for (int i = 0; i < map.m_roads.size(); i++) {
		out << "### Road #" << i << endl;
		out << "Len: " << map.m_roads[i].len() << endl;
		out << "u, v: " << map.m_roads[i].nodes()[0] << " " << map.m_roads[i].nodes()[1] << endl;
	}
	for (int i = 0; i < map.m_nodes.size(); i++) {
		out << "### Node #" << i << endl;
		for (int j = 0; j < map.m_nodes[i].roads().size(); j++)
			out << "Road: " << map.m_nodes[i].roads()[j] << " ";
		out << endl;
	}
	return out;
}
