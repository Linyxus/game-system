#include <iostream>
#include "map.h"
#include "manager.h"
using namespace std;

int main()
{
	cout << "Hello!" << endl
		<< "Welcome to Game System!" << endl;
	Map map;
	string str;
	str = "4\n0 1 1\n0 2 4\n1 2 2\n2 3 3";
	map.loadFromString(str);
	cout << map << endl;
	//Map::posInPath Unit Test
	Path path;
	Position pos;
	pos.rid = 1; pos.rpos = 2;
	path.positions.push_back(pos);
	pos.rid = 2; pos.rpos = 2;
	path.positions.push_back(pos);
	pos.rid = 3; pos.rpos = 1;
	path.positions.push_back(pos);
	pos.rid = 2; pos.rpos = 2;
	cout << map.posInPath(pos, path) << endl;

	cin.get();
	return 0;
}
