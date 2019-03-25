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
	ann.createInputNode("input1");
	ann.createInputNode("input2");
	ann.createInputNode("input3");
	ann.createNode("hidden1");
	ann.createNode("hidden2");
	ann.createNode("hidden3");
	ann.createNode("hidden4");
	ann.createNode("hidden5");
	ann.createNode("hidden6");
	ann.createNode("hidden7");
	ann.createNode("hidden8");
	ann.createNode("hidden9");
	ann.createNode("hidden10");
	ann.createNode("hidden11");
	ann.createOutputNode("output");
	ann.createConnection("input1", "hidden1");
	ann.createConnection("input1", "hidden2");
	ann.createConnection("input1", "hidden3");
	ann.createConnection("input1", "hidden4");
	ann.createConnection("input2", "hidden1");
	ann.createConnection("input2", "hidden2");
	ann.createConnection("input2", "hidden3");
	ann.createConnection("input2", "hidden4");
	ann.createConnection("input3", "hidden1");
	ann.createConnection("input3", "hidden2");
	ann.createConnection("input3", "hidden3");
	ann.createConnection("input3", "hidden4");
	ann.createConnection("hidden1", "hidden5");
	ann.createConnection("hidden1", "hidden6");
	ann.createConnection("hidden1", "hidden7");
	ann.createConnection("hidden1", "hidden8");
	ann.createConnection("hidden1", "hidden9");
	ann.createConnection("hidden2", "hidden5");
	ann.createConnection("hidden2", "hidden6");
	ann.createConnection("hidden2", "hidden7");
	ann.createConnection("hidden2", "hidden8");
	ann.createConnection("hidden2", "hidden9");
	ann.createConnection("hidden3", "hidden5");
	ann.createConnection("hidden3", "hidden6");
	ann.createConnection("hidden3", "hidden7");
	ann.createConnection("hidden3", "hidden8");
	ann.createConnection("hidden3", "hidden9");
	ann.createConnection("hidden4", "hidden5");
	ann.createConnection("hidden4", "hidden6");
	ann.createConnection("hidden4", "hidden7");
	ann.createConnection("hidden4", "hidden8");
	ann.createConnection("hidden4", "hidden9");
	ann.createConnection("hidden5", "hidden10");
	ann.createConnection("hidden5", "hidden11");
	ann.createConnection("hidden6", "hidden10");
	ann.createConnection("hidden6", "hidden11");
	ann.createConnection("hidden7", "hidden10");
	ann.createConnection("hidden7", "hidden11");
	ann.createConnection("hidden8", "hidden10");
	ann.createConnection("hidden8", "hidden11");
	ann.createConnection("hidden9", "hidden10");
	ann.createConnection("hidden9", "hidden11");
	ann.createConnection("hidden10", "output");
	ann.createConnection("hidden11", "output");

	//ANN ann(0.5f, 1.f, false);
	//ann.createInputNode("input");
	//ann.createNode("hidden1");
	//ann.createNode("hidden2");
	//ann.createOutputNode("output");
	//ann.createConnection("input", "hidden1");
	//ann.createConnection("input", "hidden2");
	//ann.createConnection("hidden1", "output");
	//ann.createConnection("hidden2", "output");
	Pool pool(ann.genChromosome(), 600, 0.2f, -4.5f, 4.5f);
	float highest = 0.f;
	while (highest < 7.5f)
	{
		// Set fitness
		for (auto & member : pool.getMembers())
		{
			//ann.applyChromosome(member.chromosome);
			//ann.reset();
			//auto nodes = ann.getAllNodes();
			//nodes["input"]->setValue(0.f);
			//float outLow = nodes["output"]->getOutput();
			//ann.reset();
			//nodes["input"]->setValue(1.f);
			//float outHigh = nodes["output"]->getOutput();
			//member.fitness = outLow - outHigh;
			////std::cout << member.fitness << std::endl;
			//if (member.fitness > highest)
			//{
			//	highest = member.fitness;
			//	std::cout << "Highest: " << highest << std::endl;
			//}
			float fitness = 0.f;
			ann.applyChromosome(member.chromosome);

			auto nodes = ann.getAllNodes();
			ann.reset();
			nodes["input1"]->setValue(0.f);
			nodes["input2"]->setValue(0.f);
			nodes["input3"]->setValue(0.f);
			fitness += nodes["output"]->getOutput();
			ann.reset();
			nodes["input1"]->setValue(1.f);
			nodes["input2"]->setValue(0.f);
			nodes["input3"]->setValue(0.f);
			fitness += 1 - nodes["output"]->getOutput();
			ann.reset();
			nodes["input1"]->setValue(0.f);
			nodes["input2"]->setValue(1.f);
			nodes["input3"]->setValue(0.f);
			fitness += 1 - nodes["output"]->getOutput();
			ann.reset();
			nodes["input1"]->setValue(0.f);
			nodes["input2"]->setValue(0.f);
			nodes["input3"]->setValue(1.f);
			fitness += 1 - nodes["output"]->getOutput();
			ann.reset();
			nodes["input1"]->setValue(1.f);
			nodes["input2"]->setValue(1.f);
			nodes["input3"]->setValue(0.f);
			fitness += nodes["output"]->getOutput();
			ann.reset();
			nodes["input1"]->setValue(0.f);
			nodes["input2"]->setValue(1.f);
			nodes["input3"]->setValue(1.f);
			fitness += nodes["output"]->getOutput();
			ann.reset();
			nodes["input1"]->setValue(1.f);
			nodes["input2"]->setValue(0.f);
			nodes["input3"]->setValue(1.f);
			fitness += nodes["output"]->getOutput();
			ann.reset();
			nodes["input1"]->setValue(1.f);
			nodes["input2"]->setValue(1.f);
			nodes["input3"]->setValue(1.f);
			fitness += 1 - nodes["output"]->getOutput();
			member.fitness = fitness;
			if (member.fitness > highest)
			{
				highest = member.fitness;
				std::cout << "Highest: " << highest << std::endl;
			}
		}
		//std::cout << "Highest: " << highest << std::endl;
		pool.runGeneration();
	}
	std::system("PAUSE");
}
