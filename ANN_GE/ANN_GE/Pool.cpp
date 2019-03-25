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
	std::sort(m_population.begin(), m_population.end(), sortMembersByFitness);
	for (auto & m : m_population)
	{
		std::cout << m.fitness << std::endl;
	}
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
		}
		m_population.push_back(member);
	}
}

void Pool::performCrossovers()
{
	// TBI
}

Chromosome Pool::crossover(Chromosome parent1, Chromosome parent2)
{
	return Chromosome(); // TBI
}

void Pool::performMutations()
{
	// TBI
}
