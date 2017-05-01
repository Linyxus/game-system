#include "manager.h"

Random::Random()
{
	generator = default_random_engine(time(NULL));
	pointDis = uniform_int_distribution<int>(1,6);
	probDis = uniform_int_distribution<int>(1,100);
}

int Random::randPoint()
{
	return pointDis(generator);
}

bool Random::hitted(int prob)
{
	return (probDis(generator) <= prob);
}

int Random::randProb()
{
	return probDis(generator);
}

Manager::Manager()
{
}

void Manager::run()
{
}

Manager::PlaceStatus Manager::getPlaceStatus(Place* place, Path path) const
{
	return getPlaceStatus(place->range(), path);
}

Manager::PlaceStatus Manager::getPlaceStatus(Range place, Path path) const
{
	if (path.positions.size() == 0)
		return Manager::PlaceStatus::NONE;
	else if (path.positions.size() == 1) {
		if (place.in(path.positions[0]))
			return Manager::PlaceStatus::STAY;
		else
			return Manager::PlaceStatus::NONE;
	}
	else if (path.positions.size() == 2) {
		bool begin = place.in(path.positions[0]);
		bool end = place.in(path.positions[1]);
		if (begin && end) {
			return Manager::PlaceStatus::STAY;
		}
		if ((!begin) && (!end)) {
			return Manager::PlaceStatus::NONE;
		}
		if ((!begin) && end) {
			return Manager::PlaceStatus::VISIT;
		}
		if (begin && (!end)) {
			return Manager::PlaceStatus::LEAVE;
		}
	}
	else { //path positions >= 3
		bool begin = place.in(path.positions[0]);
		bool end = place.in(path.positions[path.positions.size() - 1]);
		bool mid = false;
		for (int i = 1; i < path.positions.size() - 1; i++) {
			if (place.in(path.positions[i])) {
				mid = true;
				break;
			}
		}
		if (begin && end)
			return Manager::PlaceStatus::STAY;
		else if (begin && (!end))
			return Manager::PlaceStatus::LEAVE;
		else if ((!begin) && end)
			return Manager::PlaceStatus::VISIT;
		else if ((!begin) && (!end) && mid)
			return Manager::PlaceStatus::PASS;
		else
			return Manager::PlaceStatus::NONE;
	}
	return Manager::PlaceStatus::NONE;
}
