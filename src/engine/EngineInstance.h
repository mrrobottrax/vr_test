#pragma once

class EngineInstance
{
public:
	void Init();
	void Cleanup();
	void Frame();
};

inline EngineInstance& Engine()
{
	static EngineInstance game;
	return game;
}