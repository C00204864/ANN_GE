#include <iostream>
#include "Ann.h"

int main()
{
	std::cout << "Program Start" << std::endl;
	ANN ann(0.5f, 1.f);
	ann.createInputNode("input");
	ann.createNode("hidden1");
	ann.createNode("hidden2");
	ann.createOutputNode("output");
	ann.createConnection("input", "hidden1", 1.f);
	ann.createConnection("input", "hidden2", 1.f);
	ann.createConnection("hidden1", "output", 1.f);
	ann.createConnection("hidden2", "output", 1.f);
	auto inputs = ann.getInputNodes();
	inputs["input"]->setValue(1.f);
	ann.genChromosome().print();
	auto c = ann.genChromosome();
	for (auto & g : c.getGenes())
	{
		g.weight = 2.f;
	}
	ann.applyChromosome(c);
	ann.genChromosome().print();
	std::system("PAUSE");
}