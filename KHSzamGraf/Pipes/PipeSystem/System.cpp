#include "System.h"

System::System(int size, int pipeCount): elements(0), pipeCount(pipeCount), gridSize(size)
{
	Builder::resetIds();
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
			elementIDs.push_back(builder->ID);
			if (!canContinue)
			{
				delete(builder);
				++closedBuilderCount;
			}
			else builders.push_back(builder);
		}
		else delete(builder);
	}
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
			elementIDs.push_back(builder->ID);
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
}

void System::reColorAt(int at, glm::vec3 freshColor)
{
	for (int i = 0; i < builders.size(); ++i)
	{
		if (builders[i]->ID == at)
		{
			builders[i]->color = freshColor;
			break;
		}
	}
}

System::~System()
{
	for (int i = 0; i < builders.size(); ++i)
	{
		delete(builders[i]);
		builders[i] = nullptr;
	}

	for (int i = 0; i < freshElements.size(); ++i)
	{
		delete(freshElements[i]);
		freshElements[i] = nullptr;
	}

	for (int i = 0; i < elements.size(); ++i)
	{
		delete(elements[i]);
		elements[i] = nullptr;
	}

}

glm::vec3 System::colorRandomiser()
{
	return glm::vec3((float)(rand() % 255) / 255, (float)(rand() % 255) / 255, (float)(rand() % 255) / 255);
}