#pragma once
#include "Pattern.h"

class PatternData
{
private:
	Pattern* m_pattern;
	int m_frequency;
	float m_relativeFrequency;
	float m_relativeFrequencyLog2;

public:
	PatternData(Pattern* pattern) : m_pattern(pattern) { }
	~PatternData() { }

	// Getters 
	Pattern* getPattern() { return m_pattern; }
	float GetRelativeFrequency() { return m_relativeFrequency; }
	float GetRalativeFrequencyLog2() { return m_relativeFrequencyLog2; }
	// Setters
	void SetPattern(Pattern* pattern) { m_pattern = pattern; }
	void SetRelativeFrequency(float value) { m_relativeFrequency = value; }
	void SetRelativeFrequencyLog2(float value) { m_relativeFrequencyLog2 = value; }

	// Other functions
	void IncrementFrequency() { m_frequency++; }
	void CalculateRelativeFrequency(int total)
	{
		m_relativeFrequency = (float) m_frequency / total;
		m_relativeFrequencyLog2 = (float) log2(m_relativeFrequencyLog2);
	}
	bool MatchesPAtterData(Direction direction, PatternData* data)
	{
		return m_pattern->EqualToPattern(direction, data->getPattern());
	}
};