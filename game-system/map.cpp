#include "map.h"
#include <sstream>
#include <cstring>
#include <queue>
#include <fstream>
using namespace std;

/* Map */
Map::Map(const string& filename)
{
	loadFromFile(filename);
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
	if (dest == s)
		return Path();
	queue<int> q;
	int n = m_nodes.size();
	if (d == nullptr)
		d = new int[n];
	if (p == nullptr)
		p = new int[n];
	memset(p, -1, sizeof p);
	memset(inq, 0, sizeof inq);
	inq = new int[n];
	for (int i = 0; i < n; i++) {
		d[i] = -1;
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
	for (int i = nodes.size() - 1; i > 0; i--) {
		int r = commonRoad(Node(m_nodes[nodes[i]]), Node(m_nodes[nodes[i - 1]]));
		Position pos;
		pos.rid = r;
		if (nodes[i] == m_roads[r].greaterNode())
			pos.rpos = m_roads[r].len();
		else
			pos.rpos = 0;
		path.positions.push_back(pos);
	}
	if (path.positions[path.positions.size() - 1].rpos == 0) {
		Position pos;
		pos.rid = path.positions[path.positions.size() - 1].rid;
		pos.rpos = m_roads[path.positions[path.positions.size() - 1].rid].len();
		path.positions.push_back(pos);
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
	int* vis = new int[n];
	memset(vis, 0, sizeof vis);
	for (int i = 0; i < n1.roads().size(); i++) {
		vis[n1.roads()[i]] = 1;
	}
	for (int i = 0; i < n2.roads().size(); i++) {
		if (vis[n2.roads()[i]])
			return n2.roads()[i];
	}
	return -1;
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
		path.positions.insert(path.positions.begin(), s);
		path.positions.push_back(e);
		int len = pathLen(path);
		if (minlen == -1 || minlen < len) {
			minlen = len;
			minPath = path;
		}
	}
	return minPath;
}

Path Map::queryPath(Position s, Position e, int step)
{
	Path path = caculatePath(s, e);
	Path ret;
	int cur = 0;
	while (pathLen(ret) < step) {
		ret.positions.push_back(path.positions[cur++]);
	}
	ret.positions[ret.positions.size() - 1].rpos += step - pathLen(ret);
	return ret;
}

int Map::pathLen(const Path &p) const //caculate the path length
{
	int ret = 0;
	vector<Position> poss = p.positions;
	if (poss.size() == 0)
		return -1;
	Position prev = poss[0];
	for (int i = 1; i < poss.size(); i++) {
		Position pos = poss[i];
		Road r1 = this->m_roads[prev.rid];
		Road r2 = this->m_roads[pos.rid];
		Node node = commonNode(r1, r2);
		if (node.id() == -1)
			return -1;
		int len1, len2;
		if (node.id() == r1.greaterNode()) {
			len1 = r1.len() - prev.rpos;
		}
		else {
			len1 = prev.rpos;
		}
		if (node.id() == r2.greaterNode()) {
			len2 = r2.len() - pos.rpos;
		}
		else {
			len2 = pos.rpos;
		}
		ret += len1 + len2;
		prev = pos;
	}
	return ret;
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
