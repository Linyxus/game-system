#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "manager.h"
#include <cstdlib>
#include <array>
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
	cout << ">>> Enter crossroad data file path (d as default: E:/data/tp.txt): ";
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
	
	//main body
	//___test function
	vector<double> probs = { 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9 };
	array<int, 9> sumturns;
	array<int, 9> times;
	for (int i = 0; i < sumturns.size(); i++) {
		times[i] = 0;
		sumturns[i] = 0;
	}
	//___
	for (int i = 0; i < N; i++) {
		if (i % 10 == 0) {
			cout << i << " / " << N << endl;
		}
		Manager manager;
		Punisher punisher(&manager);
		manager.random = &random;
		manager.initFromFile(&map, stfn);
		//crossroad load
		vector<CrossRoad> tps;
		loadPlace(manager, &punisher, tpfn, tps);
		//___test function
		manager._SET_PROB(0, probs[i % 9]);
		//___
		manager.run();
		//manager.recorder().output(sfn); DISABLED

		//___test function
		times[i % 9]++;
		sumturns[i % 9] += manager.recorder().turns(0);
		//___
	}

	//___test function
	for (int i = 0; i < probs.size(); i++) {
		cout << probs[i] << " average turns : " << double(sumturns[i]) / double(times[i]) << endl;
	}
	//___
	system("PAUSE>NUL");
	return 0;
}
