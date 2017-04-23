#include "car.h"

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
