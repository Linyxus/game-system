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
