#pragma once
#include "Types.h"
#include "PatternData.h"
#include "PatternNeighbours.h"

class IFindNeighbourStrategy 
{

};

class PatternManager
{
	std::map<int, PatternData> m_patternDataMap;
	std::map<int, PatternNeighbours> m_possibleNeighbours;
	int m_patternSize = -1;

	IFindNeighbourStrategy m_strategy;
};