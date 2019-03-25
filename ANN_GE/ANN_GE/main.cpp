#include <iostream>
#include "Ann.h"
#include "Pool.h"

int main()
{
	std::cout << "Program Start" << std::endl;
	//ANN ann(0.5f, 1.f);
	//ann.createInputNode("input");
	//ann.createNode("hidden1");
	//ann.createNode("hidden2");
	//ann.createOutputNode("output");
	//ann.createConnection("input", "hidden1", 1.f);
	//ann.createConnection("input", "hidden2", 1.f);
	//ann.createConnection("hidden1", "output", 1.f);
	//ann.createConnection("hidden2", "output", 1.f);
	//auto inputs = ann.getInputNodes();
	//inputs["input"]->setValue(1.f);
	//ann.genChromosome().print();
	//auto c = ann.genChromosome();
	//for (auto & g : c.getGenes())
	//{
	//	g.weight = 2.f;
	//}
	//ann.applyChromosome(c);
	//ann.genChromosome().print();
	//std::system("PAUSE");

	//ANN ann2(0.5f, 1.f);
	//ann2.genFromChromosome(c);
	//ann2.genChromosome().print();

	ANN ann(0.5f, 1.f, false);
	ann.createInputNode("input");
	ann.createNode("hidden1");
	ann.createNode("hidden2");
	ann.createOutputNode("output");
	ann.createOutputNode("output2");
	ann.createConnection("input", "hidden1", 1.f);
	ann.createConnection("input", "hidden2", 1.f);
	ann.createConnection("hidden1", "output", 1.f);
	ann.createConnection("hidden2", "output", 1.f);
	ann.createConnection("hidden1", "output2", 1.f);
	ann.createConnection("hidden2", "output2", 1.f);
	Pool pool(ann.genChromosome(), 6, 0.2f, -2.f, 2.f);
	for (auto & member : pool.getMembers())
	{
		ann.applyChromosome(member.chromosome);
		ann.reset();
		auto nodes = ann.getAllNodes();
		nodes["input"]->setValue(-1.f);
		float outLow = nodes["output"]->getOutput() - nodes["output2"]->getOutput();
		nodes["input"]->setValue(1.f);
		float outHigh = nodes["output2"]->getOutput() - nodes["output"]->getOutput();
		member.fitness = (outHigh * -1) + (outLow);
		std::cout << member.fitness << std::endl;
	}
	std::system("PAUSE");
}
