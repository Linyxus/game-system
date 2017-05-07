#pragma once
#include "map.h"
#include <string>
using namespace std;

class Manager;
class Car;
class Controller;

class Car
{
public:
	Car(Manager* parent) { m_manager = parent; m_stopTurns = 0; m_rateTurns = 0; }
	enum Status {BROKEN, SLOWERED, HIGHWAY};
	bool turnable();
	double speedRate();
	void setStopTurns(int t) { m_stopTurns = t; }
	void setSpeedRate(double sr, int t) { m_speedrate = sr; m_rateTurns = t; }
	Status status() const { return m_status; }
	void setStatus(Status status) { m_status = status; }
	int coins() const { return m_coins; }
	void setCoins(int coins) { m_coins = coins; }
	int credits() const { return m_credits; }
	void setCredits(int credits) { m_credits = credits; }
	bool hitted(double prob);
	Manager* manager() { return m_manager; }
	void setManager(Manager * manager) { m_manager = manager; }
	Position pos;
	Path decidePath(int step) const;
	bool decideEvent(string id) const;
	Controller* controller;
	bool outed() const;
private:
	int m_stopTurns;
	double m_speedrate;
	int m_rateTurns;
	Status m_status;
	int m_coins;
	int m_credits;
	Manager* m_manager;
};

class Controller
{
public:
	Controller() { }
	Controller(Car* parent) { m_parent = parent; }
	virtual Path decidePath(int step) = 0;
	virtual bool decideEvent(string) = 0;
	double prob;
	Car* m_parent;
};

class AI : public Controller
{
public:
	AI() : Controller() {}
	AI(Car * parent) : Controller(parent) {}
	Path decidePath(int step);
	bool decideEvent(string);
};