#pragma once

class Car
{
public:
	enum Status {BROKEN, SPEEDED, HIGHWAY};
	bool turnable();
	Status status() const { return m_status; }
	void setStatus(Status status) { m_status = status; }
	int coins() const { return m_coins; }
	void setCoins(int coins) { m_coins = coins; }
	int credits() const { return m_credits; }
	void setCredits(int credits) { m_credits = credits; }
private:
	int m_stopTurns;
	Status m_status;
	int m_coins;
	int m_credits;
};