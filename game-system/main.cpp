#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
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

template <typename T>
void loadPlace(Manager & manager, Punisher * punisher, string fn, vector<T> & vec)
{
	std::ifstream t(fn);
	std::stringstream buffer;
	buffer << t.rdbuf();
	int N;
	buffer >> N;
	buffer.get();
	for (int i = 0; i < N; i++) {
		T place(manager.m_map, punisher);
		string str;
		getline(buffer, str);
		place.loadRange(str);
		vec.push_back(place);
	}
	for (int i = 0; i < N; i++) {
		manager.addPlace(&(vec[i]));
	}
}

int main()
{
	cout << "<<< Welcome to Game System! >>>" << endl;
	Map map;
	Random random;

	//load map data
	cout << ">>> Enter map file path (d as default: E:/data/map.txt): ";
	string mapfn;
	cin >> mapfn;
	if (mapfn == "d") {
		mapfn = "E:/data/map.txt";
	}
	cout << "Loading from [" << mapfn << "]\n";
	map.loadFromFile(mapfn);
	cout << "Map data loaded." << endl;
	cout << map;

	//init settings data
	cout << ">>> Enter init data file path (d as default: E:/data/settings.txt): ";
	string stfn;
	cin >> stfn;
	if (stfn == "d") {
		stfn = "E:/data/settings.txt";
	}

	//load testplace
	cout << ">>> Enter test place data file path (d as default: E:/data/tp.txt): ";
	string tpfn;
	cin >> tpfn;
	if (tpfn == "d") {
		tpfn = "E:/data/tp.txt";
	}

	cout << ">>> Enter times to simulate: ";
	int N;
	cin >> N;
	cout << ">>> Enter file path to save: ";
	string sfn;
	cin >> sfn;
	bool first = true;
	vector<int> res15;
	vector<int> res80;
	for (int i = 0; i < N; i++) {
		if (i % 10 == 0) {
			cout << i << " / " << N << endl;
		}
		Manager manager;
		Punisher punisher(&manager);
		manager.random = &random;
		manager.initFromFile(&map, stfn);
		//tp load
		vector<TestPlace> tps;
		loadPlace(manager, &punisher, tpfn, tps);
		manager.run();
		manager.recorder().output(sfn);
		res15.push_back(manager.recorder().result(0));
		res80.push_back(manager.recorder().result(1));
	}
	int sum15 = 0, sum80 = 0;
	for (int i = 0; i < res15.size(); i++)
		if (res15[i] == 0)
			sum15++;
	for (int i = 0; i < res80.size(); i++)
		if (res80[i] == 0)
			sum80++;
	cout << "15%: " << double(sum15) / double(N) << endl;
	cout << "80%: " << double(sum80) / double(N) << endl;

	system("PAUSE>NUL");
	return 0;
}
