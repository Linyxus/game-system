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
	Car(Manager* parent) { m_manager = parent; outed = false; }
	enum Status {BROKEN, SPEEDED, HIGHWAY};
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
	Manager* manager() const { return m_manager; }
	Position pos;
	Path decidePath(int step) const;
	bool decideEvent(string id) const;
	Controller* controller;
	bool outed;
private:
	int m_stopTurns;
	double m_speedrate;
	double m_rateTurns;
	Status m_status;
	int m_coins;
	int m_credits;
	Manager* m_manager;
};

class Controller
{
public:
	Controller(Car* parent) { m_parent = parent; }
	virtual Path decidePath(int step) const = 0;
	virtual bool decideEvent(string) const = 0;
	double prob;
protected:
	Car* m_parent;
};

class AI : public Controller
{
public:
	Path decidePath(int step) const;
	bool decideEvent(string) const;
};