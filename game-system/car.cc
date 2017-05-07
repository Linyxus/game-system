#include "car.h"
#include "manager.h"

bool Car::turnable()
{
	if (m_stopTurns > 0) {
		m_stopTurns--;
		return false;
	}
	else {
		return true;
	}
}

double Car::speedRate()
{
	if (m_rateTurns > 0) {
		m_rateTurns--;
		return m_speedrate;
	}
	else {
		return 1.0;
	}
}

bool Car::hitted(double prob)
{
	int p = prob * 100;
	return m_manager->random->hitted(p);
}

Path Car::decidePath(int step) const
{
	return controller->decidePath(step);
}

bool Car::decideEvent(string id) const
{
	return controller->decideEvent(id);
}

bool Car::outed() const
{
	bool b = false;
	if (this->m_coins < 0)
		b = true;
	if (this->m_credits <= 0)
		b = true;
	return b;
}

Path AI::decidePath(int step)
{
	Car * p = m_parent;
	Manager * m = p->manager();
	Map * map = m->m_map;
	Position pos = m_parent->pos;
	Position dest = m_parent->manager()->dest;
	return map->queryPath(pos, dest, step);
}

bool AI::decideEvent(string id)
{
	return m_parent->hitted(this->prob);
}
