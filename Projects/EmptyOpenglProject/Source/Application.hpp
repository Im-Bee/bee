#pragma once

#include "Bee/Include/Bee.hpp"
#include "Bee3D/Include/Bee3D.hpp"
#include "Bee/Include/EntryPoint/Win32/Entry.hpp"

#include "Bee/Include/UserInput.hpp"

#include "Bee3D/Include/OpenGL/RaycasterGL.hpp"
#include "Bee3D/Include/OpenGL/RendererGL.hpp"

class Application : public Bee::App::IApplication
{
	::Bee::GL::RaycasterRenderer m_Raycaster;
	::Bee::GL::RendererGL m_GLRenderer;

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Destroy() override;

};
