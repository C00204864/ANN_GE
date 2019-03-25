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
	ann.createConnection("input", "hidden1");
	ann.createConnection("input", "hidden2");
	ann.createConnection("hidden1", "output");
	ann.createConnection("hidden2", "output");
	Pool pool(ann.genChromosome(), 600, 0.2f, -1.f, 1.f);
	float highest = 0.f;
	while (highest < 0.9f)
	{
		for (auto & member : pool.getMembers())
		{
			ann.applyChromosome(member.chromosome);
			ann.reset();
			auto nodes = ann.getAllNodes();
			nodes["input"]->setValue(0.f);
			float outLow = nodes["output"]->getOutput();
			ann.reset();
			nodes["input"]->setValue(1.f);
			float outHigh = nodes["output"]->getOutput();
			member.fitness = outLow - outHigh;
			//std::cout << member.fitness << std::endl;
			if (member.fitness > highest)
			{
				highest = member.fitness;
			}
		}
		std::cout << "Highest: " << highest << std::endl;
	}
	std::system("PAUSE");
}
