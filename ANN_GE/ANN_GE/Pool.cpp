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
	std::vector<Member> newPop;
	for (int i = 0; i < m_populationSize / 2; ++i)
	{
		newPop.push_back(selectByWeight(true));
	}
	m_population = newPop;
	std::sort(m_population.begin(), m_population.end(), sortMembersByFitness);
	performCrossovers();
	performMutations();
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
	int currentSize = m_population.size();
	for (int i = 0; i < currentSize / 2; ++i)
	{
		int rand1 = randomBetweenTwoInts(0, currentSize - 1);
		int rand2 = randomBetweenTwoInts(0, currentSize - 1);
		if (rand1 == rand2)
		{
			--i;
			continue;
		}
		else
		{
			Member newChild;
			newChild.fitness = 0.f;
			newChild.chromosome = mutate(crossover(m_population[rand1].chromosome, m_population[rand2].chromosome));
			m_population.push_back(newChild);
		}
	}
}

Chromosome Pool::crossover(Chromosome parent1, Chromosome parent2)
{
	auto & parent1Genes = parent1.getGenes();
	auto & parent2Genes = parent2.getGenes();
	Chromosome child;
	if (parent1Genes.size() == parent2Genes.size())
	{
		for (int i = 0; i < parent1Genes.size(); --i)
		{
			child.addGene(randomBetweenTwoInts(0, 1) < 1 ? parent1Genes.at(i) : parent2Genes.at(i));
		}
		return child;
	}
	return parent1;
}

void Pool::performMutations()
{
	int currentSize = m_population.size();
	for (int i = 0; i < m_populationSize - currentSize; ++i)
	{
		Member mutatedMemeber;
		int randIndex = randomBetweenTwoInts(0, currentSize - 1);
		mutatedMemeber.chromosome = mutate(m_population[randIndex].chromosome);
		mutatedMemeber.fitness = 0.f;
		m_population.push_back(mutatedMemeber);
	}
}

Chromosome Pool::mutate(Chromosome parent)
{
	for (auto & gene : parent.getGenes())
	{
		if (randomBetweenTwoFloats(0, 1) < m_mutationChance)
		{
			gene.weight = randomBetweenTwoFloats(m_minWeight, m_maxWeight);
		}
	}
	return parent;
}

Member Pool::selectByWeight(bool destructive)
{
	float totalSum = 0.f;
	for (auto & member : m_population)
	{
		totalSum += member.fitness;
	}
	float randInFitnessSum = randomBetweenTwoFloats(0, totalSum);
	float currSum = 0.f;
	for (int i = 0; i < m_population.size(); ++i)
	{
		Member member = m_population.at(i);
		currSum += member.fitness;
		if (currSum > randInFitnessSum)
		{
			if (destructive)
			{
				m_population.erase(m_population.begin() + i);
			}
			return member;
		}
	}
}
