#include <pch.h>
#include "EngineInstance.h"
#include <gl/GlInstance.h>
#include <vr/VrInstance.h>
#include <app/App.h>

void EngineInstance::Init()
{
	//Vr().Init();
	Gl().Init(App().MainWindow());
}

void EngineInstance::Cleanup()
{
	Gl().Cleanup();
	//Vr().Cleanup();
}

void EngineInstance::Frame()
{
	Gl().RenderFrame();
}