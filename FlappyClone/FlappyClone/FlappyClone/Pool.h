#ifndef POOL_H
#define POOL_H

#include <vector>
#include <algorithm>
#include "Chromosome.h"
#include "MathUtil.h"

/// <summary>
/// Struct represents an individual member of the total population
/// </summary>
struct Member
{
	Chromosome chromosome;
	float fitness;
};

/// <summary>
/// Function used to sort population by fitness
/// </summary>
/// <param name="a">Population member to be compared</param>
/// <param name="b">Population member to be compared against</param>
/// <returns></returns>
static bool sortMembersByFitness(const Member & a, const Member & b)
{
	return a.fitness > b.fitness;
}

/// <summary>
/// Pool class represents entire population for genetic algorithm
/// and has the ability to run a generation (perform mutation and
/// crossover operations)
/// </summary>
class Pool
{
public:
	Pool(Chromosome baseChromosome, int populationSize, float crossoverChance, float mutationChance, float minWeight, float maxWeight);
	~Pool();
	void runGeneration(int elitism);
	Chromosome getBestChromosome();
	std::vector<Member> & getMembers();
private:
	void genPopulation(Chromosome baseChromosome);
	void performCrossovers();
	Chromosome crossover(Chromosome parent1, Chromosome parent2);
	void performMutations();
	Chromosome mutate(Chromosome parent);
	Member selectByWeight(bool destructive);

	int m_populationSize;
	float m_crossoverChance;
	float m_mutationChance;
	float m_minWeight;
	float m_maxWeight;
	std::vector<Member> m_population;
};

#endif // !POOL_H
