#include "map.h"
#include <sstream>

/* Map */
Map::Map(const string& filename)
{
	loadFromFile(filename);
}

void Map::loadFromFile(const string& filename)
{

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
