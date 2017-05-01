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
	return m_manager->random.hitted(p);
}

Path Car::decidePath(int step) const
{
	return controller->decidePath(step);
}

bool Car::decideEvent(string id) const
{
	return controller->decideEvent(id);
}

Path AI::decidePath(int step) const
{
	return this->m_parent->manager()->map()->queryPath(
		this->m_parent->pos,
		this->m_parent->manager()->dest(),
		step);
}

bool AI::decideEvent(string id) const
{
	return m_parent->hitted(this->prob);
}
