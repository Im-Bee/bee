#pragma once

#include "Bee/Include/Bee.hpp"
#include "Bee/Include/EntryPoint/Win32/Entry.hpp"

class Application : public Bee::App::IApplication
{
public:
	virtual void Initialize() override;
	virtual void Update() override {};
	virtual void Destroy() override {};
};