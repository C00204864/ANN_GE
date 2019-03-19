#ifndef CHROMOSOME_H
#define CHROMOSOME_H

#include <iostream>
#include <string>
#include <vector>

struct Gene
{
	std::string toId;
	std::string fromId;
	float weight;
};

class Chromosome
{
public:
	Chromosome();
	~Chromosome();
	void addGene(Gene gene);
	std::vector<Gene> & getGenes();
	void print();
private:
	std::vector<Gene> m_genes;
};

#endif // !CHROMOSOME_H
