#ifndef CHROMOSOME_H
#define CHROMOSOME_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

/// <summary>
/// Gene represents one single connection in an ANN
/// to id, from id and weighting
/// </summary>
struct Gene
{
	std::string toId;
	std::string fromId;
	float weight;
};

/// <summary>
/// Chromosome represents a collection of genes that 
/// follows the structure of a neural network
/// </summary>
class Chromosome
{
public:
	Chromosome();
	~Chromosome();
	void addGene(Gene gene);
	std::vector<Gene> & getGenes();
	void print();
	void saveToTextFile(std::string path);
	void loadFromTextFile(std::string path);
private:
	std::vector<Gene> m_genes;
};

#endif // !CHROMOSOME_H
