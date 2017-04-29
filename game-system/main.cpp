#include <iostream>
#include "map.h"
#include "place.h"
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
	//Place::Range Unit Test
	Range range(&map);
	range.loadFromString("2 1 2 0 1");
	Position pos;
	pos.rid = 0; pos.rpos = 1;
	cout << range.in(pos) << endl;

	cin.get();
	return 0;
}
