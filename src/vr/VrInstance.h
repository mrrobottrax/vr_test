#pragma once

class VrInstance
{
public:
	void Init();
	void Cleanup();
};

inline VrInstance& Vr()
{
	static VrInstance vr;
	return vr;
}