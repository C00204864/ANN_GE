#include "Pool.h"

/// <summary>
/// Genetic Algorithm pool constructor function
/// </summary>
/// <param name="baseChromosome">Chromosome from which to generate a population</param>
/// <param name="populationSize">Size of the desired population</param>
/// <param name="crossoverChance">Chance for crossover in genetic algorithm</param>
/// <param name="mutationChance">Chance for mutation in genetic algorithm</param>
/// <param name="minWeight">Minimum weight for the ANN</param>
/// <param name="maxWeight">Maximum weight for the ANN</param>
Pool::Pool(Chromosome baseChromosome, int populationSize, float crossoverChance, float mutationChance, float minWeight, float maxWeight)
	: m_populationSize(populationSize),
	m_crossoverChance(crossoverChance),
	m_mutationChance(mutationChance),
	m_minWeight(minWeight),
	m_maxWeight(maxWeight)
{
	genPopulation(baseChromosome);
}

/// <summary>
/// Pool destructor function
/// </summary>
Pool::~Pool() 
{
}

/// <summary>
/// Run a generation of the genetic algorithm
/// </summary>
/// <param name="elitism">Elitism value to be used for the generation</param>
void Pool::runGeneration(int elitism)
{
	bool success = false;
	//for(auto & member : m_population)
	//{
	//	if (member.fitness > 0)
	//	{
	//		success = true;
	//		break;
	//	}
	//}
	//if (!success)
	//{
	//	Chromosome c = m_population.at(0).chromosome;
	//	m_population.clear();
	//	genPopulation(c);
	//	return;
	//}
	for (auto & member : m_population)
	{
		if (member.fitness > 45)
		{
			success = true;
			break;
		}
	}
	if (!success)
	{
		Chromosome c = m_population.at(0).chromosome;
		m_population.clear();
		genPopulation(c);
		return;
	}
	std::sort(m_population.begin(), m_population.end(), sortMembersByFitness);
	std::vector<Member> newPop;
	for (int i = 0; i < elitism; ++i)
	{
		newPop.push_back(m_population.at(i));
		m_population.erase(m_population.begin() + i);
	}
	for (int i = 0; i < (m_populationSize / 2) - elitism; ++i)
	{
		newPop.push_back(selectByWeight(true));
	}
	m_population = newPop;
	//m_population.erase(m_population.begin() + m_populationSize / 2, m_population.end());
	//std::sort(m_population.begin(), m_population.end(), sortMembersByFitness);
	performCrossovers();
	performMutations();
}

/// <summary>
/// Get the best chromosome in a population
/// </summary>
/// <returns>Best chromosome in the population</returns>
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

/// <summary>
/// Get all individuals in the population
/// </summary>
/// <returns>vector of population members</returns>
std::vector<Member> & Pool::getMembers()
{
	return m_population;
}

/// <summary>
/// Generate a population based on a chromosome for structure
/// </summary>
/// <param name="baseChromosome">Chromosome from which to base population off</param>
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

/// <summary>
/// Perform crossovers for genetic algorithm
/// </summary>
void Pool::performCrossovers()
{
	int currentSize = m_population.size();
	for (int i = 0; i < currentSize; ++i)
	{
		Member newChild;
		newChild.fitness = 0.f;
		if (randomBetweenTwoInts(0, 1) < 0.9f)
		{
			newChild.chromosome = mutate(crossover(selectByWeight(false).chromosome, selectByWeight(false).chromosome));
			m_population.push_back(newChild);
		}
		else
		{
			m_population.push_back(selectByWeight(false));
		}
	}
}

/// <summary>
/// Crossover poeration
/// </summary>
/// <param name="parent1">First parent for crossover</param>
/// <param name="parent2">Second parent for crossover</param>
/// <returns>New child chromosome</returns>
Chromosome Pool::crossover(Chromosome parent1, Chromosome parent2)
{
	auto & parent1Genes = parent1.getGenes();
	auto & parent2Genes = parent2.getGenes();
	Chromosome child;
	if (parent1Genes.size() == parent2Genes.size())
	{
		for (int i = 0; i < parent1Genes.size(); ++i)
		{
			child.addGene(randomBetweenTwoInts(0, 1) < 1 ? parent1Genes.at(i) : parent2Genes.at(i));
		}
		return child;
	}
	return parent1;
}

/// <summary>
/// Perform mutations for genetic algorithm
/// </summary>
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

/// <summary>
/// Mutation operation for genetic algorithm
/// </summary>
/// <param name="parent">Parent chromosome for mutation</param>
/// <returns>New child chromosome</returns>
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

/// <summary>
/// Select a chromosome with varying likliehood based on chromosome fitness
/// </summary>
/// <param name="destructive">Bool representing whether or not we need the choice to be unique</param>
/// <returns>Random population member</returns>
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
	return m_population.at(randomBetweenTwoInts(0, m_population.size() - 1));
}
