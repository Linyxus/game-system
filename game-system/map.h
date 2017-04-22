#pragma once

#include <string>
#include <vector>
using namespace std;

class Map;
struct Position;
class Node;
class Road;

class Position {
	int rid;
	int rpos;
};



class Map
{
public:
	Map(const string& filename);
	void loadFromFile(const string& filename);
private:

};