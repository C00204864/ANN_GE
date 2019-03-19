#include "ANN.h"

ANN::ANN(float threshold, float bias)
	: m_threshold(threshold),
	m_bias(bias) {}

ANN::~ANN() {}

Perceptron * ANN::createInputNode(std::string id)
{
	Perceptron * inputNode = createNode(id);
	m_inputNodes.insert(std::pair<std::string, Perceptron *>(id, inputNode));
	return inputNode;
}

Perceptron * ANN::createOutputNode(std::string id)
{
	Perceptron * outputNode = createNode(id);
	m_outputNodes.insert(std::pair<std::string, Perceptron *>(id, outputNode));
	return outputNode;
}

void ANN::createConnection(std::string fromId, std::string toId)
{
	createConnection(fromId, toId, 0.f);
}

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

std::map<std::string, Perceptron*> ANN::getInputNodes()
{
	return m_inputNodes;
}

std::map<std::string, Perceptron*> ANN::getOutputNodes()
{
	return m_outputNodes;
}

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
	}
	return chromosome;
}

void ANN::applyChromosome(Chromosome chromosome)
{
	for (auto & gene : chromosome.getGenes())
	{
		Perceptron * p = m_allNodes[gene.toId];
		if (nullptr != p)
		{
			auto & inputs = p->getInputs();

			inputs[gene.fromId].weight = gene.weight;
		}
	}
}

Perceptron * ANN::createNode(std::string id)
{
	Perceptron * node = new Perceptron(id, m_threshold, m_bias);
	m_allNodes.insert(std::pair<std::string, Perceptron *>(id, node));
	return node;
}