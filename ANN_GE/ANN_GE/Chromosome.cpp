#include "Chromosome.h"

Chromosome::Chromosome() {}

Chromosome::~Chromosome() {}

void Chromosome::addGene(Gene gene)
{
	m_genes.push_back(gene);
}

std::vector<Gene> & Chromosome::getGenes()
{
	return m_genes;
}

void Chromosome::print()
{
	for (auto & gene : m_genes)
	{
		std::cout << "From Id: " << gene.fromId << ", To Id: " << gene.toId << ", Weight: " << gene.weight << std::endl;
	}
}
