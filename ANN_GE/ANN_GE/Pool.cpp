#include "Pool.h"

Pool::Pool(Chromosome baseChromosome, int populationSize, float mutationChance, float minWeight, float maxWeight)
	: m_populationSize(populationSize),
	m_mutationChance(mutationChance),
	m_minWeight(minWeight),
	m_maxWeight(maxWeight)
{
	genPopulation(baseChromosome);
}

Pool::~Pool() 
{
}

void Pool::runGeneration()
{
}

Chromosome Pool::getBestChromosome()
{
	float bestFitness = -1.f;
	Chromosome bestChromosome;
	for (auto & member : m_population)
	{
		if (member.fitness > bestFitness)
		{
			bestChromosome = member.chromosome;
		}
	}
	return bestChromosome;
}

std::vector<Member> & Pool::getMembers()
{
	return m_population;
}

void Pool::genPopulation(Chromosome baseChromosome)
{
	for (int i = 0; i < m_populationSize; ++i)
	{
		Member member;
		member.fitness = 0.f;
		member.chromosome = baseChromosome;
		for (auto & gene : member.chromosome.getGenes())
		{
			gene.weight = randomBetweenTwoFloats(m_minWeight, m_maxWeight);
			std::cout << "w: " << gene.weight << std::endl;
		}
		m_population.push_back(member);
	}
}

void Pool::performCrossovers()
{
}

Chromosome Pool::crossover(Chromosome parent1, Chromosome parent2)
{
	return Chromosome();
}

void Pool::performMutations()
{
}
