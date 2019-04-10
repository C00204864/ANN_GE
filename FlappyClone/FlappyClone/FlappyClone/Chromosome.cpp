#include "Chromosome.h"

/// <summary>
/// Chromosome constructor function
/// </summary>
Chromosome::Chromosome() 
{
}

/// <summary>
/// Chromosome destructor function
/// </summary>
Chromosome::~Chromosome() 
{
}

/// <summary>
/// Function used to add a gene to the chromosome
/// </summary>
/// <param name="gene">Gene to add to the chromosome</param>
void Chromosome::addGene(Gene gene)
{
	m_genes.push_back(gene);
}

/// <summary>
/// Functio used to get all the genes in the chromosome
/// </summary>
/// <returns>Vector of Gene objects</returns>
std::vector<Gene> & Chromosome::getGenes()
{
	return m_genes;
}

/// <summary>
/// Print the contents of the chromosome to the console
/// </summary>
void Chromosome::print()
{
	for (auto & gene : m_genes)
	{
		std::cout << "From Id: " << gene.fromId << ", To Id: " << gene.toId << ", Weight: " << gene.weight << std::endl;
	}
}

/// <summary>
/// Save a chromosome to a text file
/// </summary>
/// <param name="path">String path to save chromosome to</param>
void Chromosome::saveToTextFile(std::string path)
{
	std::ofstream fileStream;
	fileStream.open(path, std::ofstream::out | std::ofstream::trunc);
	for (auto & gene : m_genes)
	{
		fileStream << gene.fromId << "," << gene.toId << "," << gene.weight << std::endl;
	}
	fileStream.close();
}

/// <summary>
/// Load a chromosome from a text file
/// </summary>
/// <param name="path">String path to load a chromosome from</param>
void Chromosome::loadFromTextFile(std::string path)
{
	m_genes.clear();
	std::string delimiter = ",";
	int pos = 0;
	int count = 0;
	std::string output;
	std::string value[3];
	std::ifstream fileStream;
	fileStream.open(path);
	while(!fileStream.eof())
	{
		output = "";
		fileStream >> output;
		if ("" != output)
		{
			while ((pos = output.find(delimiter)) != std::string::npos) 
			{
				value[count] = output.substr(0, pos);
				output.erase(0, pos + delimiter.length());
				++count;
			}
			value[2] = output;
			Gene loadedGene;
			loadedGene.fromId = value[0];
			loadedGene.toId = value[1];
			loadedGene.weight = std::stof(value[2]);
			m_genes.push_back(loadedGene);
			count = 0;
		}
	}
	fileStream.close();
}
