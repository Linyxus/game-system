#include <iostream>
#include "manager.h"
using namespace std;

void _DISPLAY_PS(Manager::PlaceStatus ps)
{
	if (ps == Manager::NONE)
		cout << "NONE" << endl;
	if (ps == Manager::PASS)
		cout << "PASS" << endl;
	if (ps == Manager::VISIT)
		cout << "VISIT" << endl;
	if (ps == Manager::LEAVE)
		cout << "LEAVE" << endl;
	if (ps == Manager::STAY)
		cout << "STAY" << endl;
}

int main()
{
	cout << "Hello!" << endl
		<< "Welcome to Game System!" << endl;
	Map map;
	string str;
	str = "4\n0 1 1\n0 2 4\n1 2 2\n2 3 3";
	map.loadFromString(str);
	cout << map << endl;
	//Manager::getPlaceStatus Unit Test
	Manager manager;
	Path path;
	Position pos;
	pos.rid = 0; pos.rpos = 0;
	path.positions.push_back(pos);
	pos.rid = 2; pos.rpos = 0;
	path.positions.push_back(pos);
	pos.rid = 1; pos.rpos = 4;
	path.positions.push_back(pos);
	Range range(&map);
	str = "3 2 0 1 2 0 2";
	range.loadFromString(str);
	_DISPLAY_PS(manager.getPlaceStatus(range, path));

	cin.get();
	return 0;
}
