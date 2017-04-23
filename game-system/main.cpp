#include <iostream>
#include "map.h"
using namespace std;

int main()
{
	cout << "Hello!" << endl
		<< "Welcome to Game System!" << endl;
	Map map("");
	string str;
	str = "3\n0 1 4\n0 2 1\n1 2 3";
	map.loadFromString(str);
	cout << map << endl;

	cin.get();
	return 0;
}