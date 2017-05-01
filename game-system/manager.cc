#include "manager.h"
#include <cmath>
using namespace std;

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
	while (true) {
		bool ended = true;
		for (int i = 0; i < m_cars.size(); i++) {
			if (m_recorder.status(i) == Recorder::OUT || m_recorder.status(i) == Recorder::REACH)
				continue;
			ended = false;
			Car& car = m_cars[i];
			double sr = car.speedRate();
			if (!car.turnable())
				continue;
			int step = random.randPoint();
			step = round(double(step) * sr);
			Path path = car.decidePath(step);
			for (int ip = 0; ip < m_places.size(); ip++) {
				Manager::PlaceStatus ps = getPlaceStatus(m_places[ip], path);
				if (ps == Manager::LEAVE) {
					m_places[i]->onLeave(car, path);
				}
				if (ps == Manager::VISIT) {
					m_places[i]->onVisit(car, path);
				}
				if (ps == Manager::STAY) {
					m_places[i]->onStay(car, path);
				}
				if (ps == Manager::PASS) {
					m_places[i]->onPass(car, path);
				}
			}
			if (car.outed) {
				m_recorder.setStatus(i, Recorder::OUT);
			}
			if (m_map->posEqual(car.pos, m_dest) {
				m_recorder.setStatus(i, Recorder::REACH);
			}
		}
	}
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

Recorder::Recorder()
{
}

void Recorder::output() const
{
}

void Recorder::addRecord(int index, Record record)
{
	m_records[index].push_back(record);
}

int Recorder::addCar()
{
	m_records.push_back(Recorder::Records());
	m_status.push_back(Recorder::RUN);
	return m_records.size() - 1;
}

void Recorder::setStatus(int index, CarStatus cs)
{
	m_status[index] = cs;
}

Recorder::CarStatus Recorder::status(int index) const
{
	return m_status[index];
}
