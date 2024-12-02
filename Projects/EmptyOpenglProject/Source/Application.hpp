#pragma once

#include "Bee/Include/Bee.hpp"
#include "Bee3D/Include/Bee3D.hpp"
#include "Bee/Include/EntryPoint/Win32/Entry.hpp"

class Application : public Bee::App::IApplication
{
	Bee::GL::RendererGL m_Renderer = {};

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Destroy() override;

};
