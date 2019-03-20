#ifndef POOL_H
#define POOL_H

#include <vector>
#include "Chromosome.h"
#include "MathUtil.h"

struct Member
{
	Chromosome chromosome;
	float fitness;
};

class Pool
{
public:
	Pool(Chromosome baseChromosome, int populationSize, float mutationChance, float minWeight, float maxWeight);
	~Pool();
	void runGeneration();
	Chromosome getBestChromosome();
	std::vector<Member> & getMembers();
private:
	void genPopulation(Chromosome baseChromosome);

	int m_populationSize;
	float m_mutationChance;
	float m_minWeight;
	float m_maxWeight;
	std::vector<Member> m_population;
};

#endif // !POOL_H
