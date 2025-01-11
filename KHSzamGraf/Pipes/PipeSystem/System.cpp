#include "System.h"

System::System(int size, int pipeCount): elements(0), pipeCount(pipeCount), gridSize(size)
{

	std::srand(std::time(nullptr));
	availability.reserve(size * size * size);
	for (int i = 0; i < size*size*size; ++i)
	{
		availability.push_back(true);
	}

	builders.reserve(pipeCount);
	for (int i = 0; i < pipeCount; ++i)
	{
		bool canContinue = true;
		bool noMoreSpace = false;
		Builder* builder = new Builder(availability, gridSize, freshElements, colorRandomiser(), canContinue, noMoreSpace);
		if (!noMoreSpace)
		{
			if (!canContinue)
			{
				delete(builder);
				++closedBuilderCount;
			}
			else builders.push_back(builder);
		}
		else delete(builder);
	}
	//TODO new for every deleted
}

void System::next()
{
	for (auto element : freshElements)
	{
		elements.push_back(element);
	}

	freshElements.erase(freshElements.begin(), freshElements.end());

	int cloasedBuilderCountInRound = 0;
	for (int i = 0; i < builders.size(); ++i)
	{
		bool canContinue = builders[i]->next();
		if (!canContinue)
		{
			delete(builders[i]);
			builders[i] = nullptr;
			++cloasedBuilderCountInRound;
		}
	}

	builders.erase(std::remove_if(builders.begin(), builders.end(), [](Builder* x) { return x == nullptr; }), builders.end());

	for (int i = 0; i < closedBuilderCount; ++i)
	{
		bool canContinue = true;
		bool noMoreSpace = false;
		Builder* builder = new Builder(availability, gridSize, freshElements, colorRandomiser(), canContinue, noMoreSpace);
		if (!noMoreSpace)
		{
			if (!canContinue)
			{
				delete(builder);
				++cloasedBuilderCountInRound;
			}
			else builders.push_back(builder);
		}
		else delete(builder);
	}

	closedBuilderCount = cloasedBuilderCountInRound;

	//TODO new for every deleted
}

System::~System()
{
	//TODO clear heap
}

glm::vec3 System::colorRandomiser()
{
	return glm::vec3((float)(rand() % 255) / 255, (float)(rand() % 255) / 255, (float)(rand() % 255) / 255);
}