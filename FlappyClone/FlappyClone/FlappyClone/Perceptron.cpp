#include "Perceptron.h"

/// <summary>
/// Perceptron constructor function
/// </summary>
/// <param name="id">String representing the perceptron id</param>
/// <param name="threshold">Threshold value for the activation of the perceptron</param>
/// <param name="bias">bias value of the perceptron</param>
/// <param name="isOutputDigital">Bool representing whether to cast output to 1/0 or keep it continuous</param>
Perceptron::Perceptron(std::string id, float threshold, float bias, bool isOutputDigital)
	: m_id(id),
	m_outputVal(0.f),
	m_inputCounter(0),
	m_threshold(threshold),
	m_bias(bias),
	m_isDigital(isOutputDigital) 
{
	m_biasWeight = 0.f;
}

/// <summary>
/// Perceptron desructor function
/// </summary>
Perceptron::~Perceptron() 
{
}

/// <summary>
/// Function used to add an input to the perceptron
/// </summary>
/// <param name="perceptron">Perceptron from which to take input</param>
/// <param name="weight">Weight value of the new connection</param>
void Perceptron::addInput(Perceptron * perceptron, float weight)
{
	Input input;
	input.node = perceptron;
	input.weight = weight;
	m_inputs.insert(std::pair<std::string, Input>(perceptron->getId(), input));
}

/// <summary>
/// Function used to add an output perceptron
/// </summary>
/// <param name="perceptron">Perceptron to add as output connection</param>
void Perceptron::addOutput(Perceptron * perceptron)
{
	m_outputs.push_back(perceptron);
}

/// <summary>
/// Set the value of a perceptron as if it had calculated it itself
/// </summary>
/// <param name="val">Desired perceptron value</param>
void Perceptron::setValue(float val)
{
	m_outputVal = val;
	for (Perceptron * outputNode : m_outputs)
	{
		outputNode->signalInput();
	}
}

/// <summary>
/// Function gets the perceptron id
/// </summary>
/// <returns>String perceptron id</returns>
std::string Perceptron::getId()
{
	return m_id;
}

/// <summary>
/// Function gets a map containing all the perceptron inputs
/// </summary>
/// <returns>Map containing all the perceptron inputs</returns>
std::map<std::string, Input> & Perceptron::getInputs()
{
	return m_inputs;
}

/// <summary>
/// Function gets the bias weight of the perceptron
/// </summary>
/// <returns>Bias weight of the perceptron</returns>
float Perceptron::getBiasWeight()
{
	return m_biasWeight;
}

/// <summary>
/// Function sets the perceptron's bias weight
/// </summary>
/// <param name="weight">Desired bias weight</param>
void Perceptron::setBiasWeight(float weight)
{
	m_biasWeight = weight;
}

/// <summary>
/// Get the output of a perceptron
/// </summary>
/// <returns>Output value of the perceptron</returns>
float Perceptron::getOutput()
{
	return m_outputVal;
}

/// <summary>
/// Signal the perceptron that one of its' inputs has been calculated
/// This will propogate through the network
/// </summary>
void Perceptron::signalInput()
{
	++m_inputCounter;
	if (m_inputCounter == m_inputs.size()) // When all perceptron inputs are ready
	{
		float inputSum = 0.f;
		for (auto & pair : m_inputs)
		{
			Input input = pair.second;
			inputSum += (input.weight * input.node->getOutput());
		}
		inputSum += m_bias * m_biasWeight; // Calculate the sum of all inputs
		float sigmoid = fastSigmoid(inputSum);
		m_outputVal = m_isDigital ? (sigmoid >= m_threshold ? 1.f : 0.f) : sigmoid;
		for (Perceptron * outputNode : m_outputs)
		{
			outputNode->signalInput(); // Signal all output nodes that this perceptron has calculated its' value
		}
	}
}

/// <summary>
/// Reset the values of the perceptron
/// </summary>
void Perceptron::reset()
{
	m_inputCounter = 0;
	m_outputVal = 0.f;
}

/// <summary>
/// Sigmoid function used as a threshold function
/// </summary>
/// <param name="val">Input for the sigmoid function</param>
/// <returns>Sigmoid output</returns>
float Perceptron::sigmoid(float val)
{
	return 1 / (1 + pow(2.7, val));
}

/// <summary>
/// Fast sigmoid equivalent function used as a threshold function
/// </summary>
/// <param name="val">Input for the fast sigmoid function</param>
/// <returns>Fast sigmoid output</returns>
float Perceptron::fastSigmoid(float val)
{
	return val / (1 + abs(val));
}
