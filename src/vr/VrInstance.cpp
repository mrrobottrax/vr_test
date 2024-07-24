#include "pch.h"
#include "VrInstance.h"

void VrInstance::Init()
{
	vr::EVRInitError error;
	vr::EVRApplicationType appType = vr::VRApplication_Scene;

	vr::VR_Init(&error, appType);

	if (error != vr::VRInitError_None)
	{
		throw std::runtime_error(std::format("failed to init vr: {:x}", (uint32_t)error));
	}
}

void VrInstance::Cleanup()
{
	vr::VR_Shutdown();
}