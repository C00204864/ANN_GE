#include "ANN.h"

/// <summary>
/// Constructor function for the ANN object
/// </summary>
/// <param name="threshold">Threshold value for each perceptron</param>
/// <param name="bias">Bias value to be used by perceptrons</param>
/// <param name="isOutputDigital">Whether or not to cast perceptron outputs to 1/0 or to leave them as continuous values</param>
ANN::ANN(float threshold, float bias, bool isOutputDigital)
	: m_threshold(threshold),
	m_bias(bias),
	m_isDigital(isOutputDigital) 
{
}

/// <summary>
/// ANN Destructor function
/// </summary>
ANN::~ANN() 
{
	for (auto & node : m_allNodes)
	{
		delete node.second;
	}
}

/// <summary>
/// Function used to create input nodes for the ANN
/// </summary>
/// <param name="id">Id string for the new perceptron</param>
/// <returns></returns>
Perceptron * ANN::createInputNode(std::string id)
{
	Perceptron * inputNode = createNode(id);
	m_inputNodes.insert(std::pair<std::string, Perceptron *>(id, inputNode));
	return inputNode;
}

/// <summary>
/// Function used to create output nodes for the ANN
/// </summary>
/// <param name="id">Id string for the new perceptron</param>
/// <returns></returns>
Perceptron * ANN::createOutputNode(std::string id)
{
	Perceptron * outputNode = createNode(id);
	m_outputNodes.insert(std::pair<std::string, Perceptron *>(id, outputNode));
	return outputNode;
}

/// <summary>
/// Function used to create a connection between two nodes
/// </summary>
/// <param name="fromId">Id of first perceptron of the connection</param>
/// <param name="toId">Id of the second perceptron of the connection</param>
void ANN::createConnection(std::string fromId, std::string toId)
{
	createConnection(fromId, toId, 0.f);
}

/// <summary>
/// Function used to create a connection between two nodes with a defined weight
/// </summary>
/// <param name="fromId">Id of first perceptron of the connection</param>
/// <param name="toId">Id of the second perceptron of the connection</param>
/// <param name="weight">Connection weight</param>
void ANN::createConnection(std::string fromId, std::string toId, float weight)
{
	Perceptron * from = m_allNodes[fromId];
	Perceptron * to = m_allNodes[toId];
	if (nullptr != from && nullptr != to)
	{
		from->addOutput(to);
		to->addInput(from, weight);
	}
}

/// <summary>
/// Return all the nodes/perceptrons in the ANN
/// </summary>
/// <returns>Map containing all perceptrons by id</returns>
std::map<std::string, Perceptron*> ANN::getAllNodes()
{
	return m_allNodes;
}

/// <summary>
/// Return all the input nodes/perceptrons in the ANN
/// </summary>
/// <returns>Map containing all input perceptrons by id</returns>
std::map<std::string, Perceptron*> ANN::getInputNodes()
{
	return m_inputNodes;
}

/// <summary>
/// Return all the output nodes/perceptrons in the ANN
/// </summary>
/// <returns>Map containing all output perceptrons by id</returns>
std::map<std::string, Perceptron*> ANN::getOutputNodes()
{
	return m_outputNodes;
}

/// <summary>
/// Create a chromosome that represents the ANN
/// </summary>
/// <returns>Chromosome representing the ANN</returns>
Chromosome ANN::genChromosome()
{
	Chromosome chromosome;
	for (auto & node : m_allNodes)
	{
		auto & inputs = node.second->getInputs();
		for (auto & input : inputs)
		{
			Gene gene;
			gene.toId = node.second->getId();
			gene.fromId = input.second.node->getId();
			gene.weight = input.second.weight;
			chromosome.addGene(gene);
		}
		Gene gene;
		gene.toId = node.second->getId();
		gene.fromId = "bias";
		gene.weight = node.second->getBiasWeight();
		chromosome.addGene(gene);
	}
	return chromosome;
}

/// <summary>
/// Apply a chromosome to the ANN
/// </summary>
/// <param name="chromosome">Chromosome to be applied</param>
void ANN::applyChromosome(Chromosome chromosome)
{
	for (auto & gene : chromosome.getGenes())
	{
		Perceptron * p = m_allNodes[gene.toId];
		if (nullptr != p)
		{
			if (gene.fromId.find("bias") != std::string::npos)
			{
				p->setBiasWeight(gene.weight);
			}
			else
			{
				auto & inputs = p->getInputs();
				inputs[gene.fromId].weight = gene.weight;
			}
		}
	}
}

/// <summary>
/// Generate the ANN from a chromosome
/// </summary>
/// <param name="chromosome">Chromosome from which to generate ANN</param>
void ANN::genFromChromosome(Chromosome chromosome)
{
	for (auto & node : m_allNodes)
	{
		delete node.second;
	}
	m_allNodes.clear();
	for (auto & gene : chromosome.getGenes())
	{
		std::string toId = gene.toId;
		std::string fromId = gene.fromId;
		Perceptron * to = m_allNodes.find(toId) == m_allNodes.end() ? createNode(toId) : m_allNodes[toId];
		if (gene.fromId.find("bias") != std::string::npos)
		{
			to->setBiasWeight(gene.weight);
		}
		else
		{
			Perceptron * from = m_allNodes.find(fromId) == m_allNodes.end() ? createNode(fromId) : m_allNodes[fromId];
			createConnection(fromId, toId, gene.weight);
			to->getInputs()[from->getId()].weight = gene.weight;
		}
	}
}

/// <summary>
/// Reset the ANN perceptrons
/// </summary>
void ANN::reset()
{
	for (auto & node : m_allNodes)
	{
		node.second->reset();
	}
}

/// <summary>
/// Create a perceptron in the ANN
/// </summary>
/// <param name="id">Perceptron id string</param>
/// <returns>Pointer to the new perceptron</returns>
Perceptron * ANN::createNode(std::string id)
{
	Perceptron * node = new Perceptron(id, m_threshold, m_bias, m_isDigital);
	m_allNodes.insert(std::pair<std::string, Perceptron *>(id, node));
	return node;
}
