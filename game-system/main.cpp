#include <iostream>
#include <string>
#include "manager.h"
#include <cstdlib>
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
	cout << "<<< Welcome to Game System! >>>" << endl;
	Map map;
	Manager manager;

	//load map data
	cout << ">>> Enter map file path (d as default: D:/data/map.txt): ";
	string mapfn;
	cin >> mapfn;
	if (mapfn == "d") {
		mapfn = "D:/data/map.txt";
	}
	cout << "Loading from [" << mapfn << "]\n";
	map.loadFromFile(mapfn);
	cout << "Map data loaded." << endl;
	cout << map;

	//init settings data
	cout << ">>> Enter init data file path (d as default: D:/data/settings.txt): ";
	string stfn;
	cin >> stfn;
	if (stfn == "d") {
		stfn = "D:/data/settings.txt";
	}
	cout << "Loading from [" << stfn << "]\n";
	manager.initFromFile(&map, stfn);
	cout << "Manager inited." << endl;
	manager._DISPLAY_CARS();
	try {
		manager.run();
	}
	catch (string str) {
		cout << str << endl;
	}
	manager.recorder().output("f:/out.txt");

	system("PAUSE>NUL");
	return 0;
}
