#include <iostream>
#include "map.h"
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
	Position pos1, pos2;
	pos1.rid = 1; pos1.rpos = 1;
	pos2.rid = 3; pos2.rpos = 3;
	Path path = map.caculatePath(pos1, pos2);
	for (int i = 0; i < path.positions.size(); i++) {
        Position pos = path.positions[i];
        cout << pos.rid << " " << pos.rpos << endl;
	}

	cin.get();
	return 0;
}
