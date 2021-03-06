#ifndef PERCEPTRON_H
#define PERCEPTRON_H

#include <iostream>
#include <vector>
#include <map>

class Perceptron; // Predeclaration for circular references

/// <summary>
/// Input struct represents input node and weight
/// as one unit
/// </summary>
struct Input
{
	Perceptron * node;
	float weight;
};

/// <summary>
/// Perceptronclass  represents a single node in 
/// an artificial neural network
/// </summary>
class Perceptron
{
public:
	// Public Functions
	Perceptron(std::string id, float threshold, float bias, bool isOutputDigital);
	~Perceptron();
	void addInput(Perceptron * inputPerceptron, float weight);
	void addOutput(Perceptron * outputPerceptron);
	void setValue(float val);
	std::string getId();
	std::map<std::string, Input> & getInputs();
	float getBiasWeight();
	void setBiasWeight(float weight);
	float getOutput();
	void signalInput();
	void reset();
private:
	// Private Functions
	float sigmoid(float val);
	float fastSigmoid(float val);

	// Private Members
	std::string m_id;
	std::map<std::string, Input> m_inputs;
	std::vector<Perceptron *> m_outputs;
	float m_outputVal;
	int m_inputCounter;
	float m_threshold;
	float m_bias;
	float m_biasWeight;
	bool m_isDigital;
};

#endif // !PERCEPTRON_H
