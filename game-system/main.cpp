#include <iostream>
#include "map.h"
#include "manager.h"
using namespace std;

int main()
{
	cout << "Hello!" << endl
		<< "Welcome to Game System!" << endl;
	Map map("");
	Manager manager;
	string str;
	str = "3\n0 1 4\n0 2 1\n1 2 3";
	map.loadFromString(str);
	cout << map << endl;
	int sum = 0;
	for (int i = 0; i < 1000; i++) {
		if (manager.random.hitted(50))
			sum++;
	}
	cout << sum << endl;

	cin.get();
	return 0;
}