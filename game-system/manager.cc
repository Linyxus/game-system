#include "manager.h"
#include <cmath>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
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

Manager::~Manager()
{
	for (int i = 0; i < m_ctls.size(); i++)
		delete (m_ctls[i]);
}

template<typename T>
double _SUM(const T& vec)
{
	double s = 0.0;
	for (auto it = vec.begin(); it != vec.end(); it++) {
		if (*it != -1) {
			s += (*it);
		}
	}
	return s;
}

template <typename T>
double _GET_P(const T& vec, int index)
{
	double sum = _SUM(vec);
	return double(vec[index]) / sum;
}

void Manager::run()
{
	int r = 0;
	int arred = 0;
	while (true) {
		bool ended = true;
		vector<int> lens;
		vector<int> credits;
		vector<int> coins;
		vector<double> diffics;
		vector<double> probs;
		for (int i = 0; i < m_cars.size(); i++) {
			lens.push_back(-1);
			credits.push_back(-1);
			coins.push_back(-1);
			diffics.push_back(-1);
			probs.push_back(-1);
		}
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
			car.pos = path.positions[path.positions.size() - 1];
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
			if (car.outed()) {
				m_recorder.setStatus(i, Recorder::OUT);
			}
			if (m_map->posEqual(car.pos, dest)) {
				m_recorder.setStatus(i, Recorder::REACH);
				m_recorder.setRanking(i, arred++);
			}

			//caculate shortest path
			Path sp = m_map->caculatePath(car.pos, dest);
			int len = m_map->pathLen(sp);
			double diffic = caculateDiffic(sp);
			lens[i] = len;
			diffics[i] = diffic;
			coins[i] = car.coins();
			credits[i] = car.credits();
			probs[i] = car.controller->prob;

			cout << "Round #" << r++ << endl;
			_DISPLAY_CARS();
			cout << endl;
		}
		if (ended) {
			break;
		}
		else {
			for (int i = 0; i < m_cars.size(); i++) {
				if (probs[i] == -1) continue;
				Record rec;
				rec.coins = coins[i];
				rec.pcoins = _GET_P(coins, i);
				rec.credits = credits[i];
				rec.pcredits = _GET_P(credits, i);
				rec.length = lens[i];
				rec.plength = _GET_P(lens, i);
				rec.diffic = diffics[i];
				rec.pdiffic = _GET_P(diffics, i);
				rec.prob = probs[i];
				rec.pprob = _GET_P(probs, i);
				m_recorder.addRecord(i, rec);
			}
		}
	}
}

void Manager::init(Map * map, int cars, vector<Position> poss, vector<Controller*> ctls, Car initialCar)
{
	m_map = map;
	for (int i = 0; i < cars; i++) {
		m_recorder.addCar();
		m_cars.push_back(initialCar);
		m_cars[i].pos = poss[i];
		m_cars[i].controller = ctls[i];
		m_cars[i].setManager(this);
	}
	for (int i = 0; i < cars; i++) {
		m_cars[i].controller->m_parent = &(m_cars[i]);
	}
}

void Manager::initFromFile(Map * map, string fn)
{
	std::ifstream t(fn);
	std::stringstream buffer;
	buffer << t.rdbuf();
	std::string contents(buffer.str());
	stringstream ss(contents);

	int n;
	vector<Position> poses;
	ss >> n;
	for (int i = 0; i < n; i++) {
		int rid, rpos;
		ss >> rid >> rpos;
		Position pos;
		pos.rid = rid;
		pos.rpos = rpos;
		poses.push_back(pos);
	}
	for (int i = 0; i < n; i++) {
		Car tmp(this);
		double prob;
		ss >> prob;
		Controller *ctl = new AI();
		ctl->prob = prob;
		m_ctls.push_back(ctl);
	}
	int iniCoins, iniCredits;
	ss >> iniCoins >> iniCredits;
	Car car(this);
	car.setCoins(iniCoins);
	car.setCredits(iniCredits);
	init(map, n, poses, m_ctls, car);

	int rid, rpos;
	ss >> rid >> rpos;
	dest.rid = rid;
	dest.rpos = rpos;
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

double Manager::caculateDiffic(Path path) const
{
	double d = 0.0;
	for (int i = 0; i < path.positions.size(); i++) {
		Position pos = path.positions[i];
		for (int j = 0; j < m_places.size(); j++) {
			if (m_places[i]->in(pos))
				d += m_places[i]->diffic();
		}
	}
	return d;
}

void Manager::_DISPLAY_CARS() const
{
	cout << "### Manager Car Data:" << endl;
	for (int i = 0; i < m_cars.size(); i++) {
		cout << "<<<< Car #" << i << endl;
		cout << "Probability: " << m_cars[i].controller->prob << endl;
		cout << "Coins: " << m_cars[i].coins() << endl;
		cout << "Credits: " << m_cars[i].credits() << endl;
		cout << "Position: " << m_cars[i].pos.rid << ", " << m_cars[i].pos.rpos << endl;
	}
	cout << "<<<< Destnation: " << dest.rid << ", " << dest.rpos << endl;
	cout << endl;
}

Recorder::Recorder()
{
}

void Recorder::output(string fn) const
{
	ofstream fout;
	fout.open(fn);
	fout << "prob,pprob,len,plen,diffic,pdiffic,coins,pcoins,credits,pcredits,outcome,outed" << endl;
	for (int i = 0; i < m_records.size(); i++) {
		const Records &recs = m_records[i];
		for (int j = 0; j < recs.size(); j++) {
			Record rec = recs[j];
			fout << rec.prob << ","
				<< rec.pprob << ","
				<< rec.length << ","
				<< rec.plength << ","
				<< rec.diffic << ","
				<< rec.pdiffic << ","
				<< rec.coins << ","
				<< rec.pcoins << ","
				<< rec.credits << ","
				<< rec.pcredits << ","
				<< ranking(i) << ","
				<< ((status(i) == Recorder::OUT) ? 1 : 0)
				<< endl;
		}
	}
	fout.close();
}

void Recorder::append(string fn) const
{
	ofstream fout;
	fout.open(fn, ios_base::app);
	//fout << "prob,pprob,len,plen,diffic,pdiffic,coins,pcoins,credits,pcredits,outcome,outed" << endl;
	for (int i = 0; i < m_records.size(); i++) {
		const Records &recs = m_records[i];
		for (int j = 0; j < recs.size(); j++) {
			Record rec = recs[j];
			if (rec.plength == 0)
			fout << rec.prob << ","
				<< rec.pprob << ","
				<< rec.length << ","
				<< rec.plength << ","
				<< rec.diffic << ","
				<< rec.pdiffic << ","
				<< rec.coins << ","
				<< rec.pcoins << ","
				<< rec.credits << ","
				<< rec.pcredits << ","
				<< ranking(i) << ","
				<< ((status(i) == Recorder::OUT) ? 1 : 0)
				<< endl;
		}
	}
	fout.close();
}

void Recorder::addRecord(int index, Record record)
{
	m_records[index].push_back(record);
}

int Recorder::addCar()
{
	m_records.push_back(Recorder::Records());
	m_status.push_back(Recorder::RUN);
	m_rankings.push_back(0);
	return m_records.size() - 1;
}

void Recorder::setStatus(int index, CarStatus cs)
{
	m_status[index] = cs;
}

void Recorder::setRanking(int index, int r)
{
	m_rankings[index] = r;
}

Recorder::CarStatus Recorder::status(int index) const
{
	return m_status[index];
}

int Recorder::ranking(int index) const
{
	return m_rankings[index];
}
