#pragma once

#include "Bee/Include/Bee.hpp"
#include "Bee3D/Include/Bee3D.hpp"
#include "Bee3D/Include/DX12/Renderer.hpp"
#include "Bee/Include/EntryPoint/Win32/Entry.hpp"

class Application : public Bee::App::IApplication
{
	Bee::DX12::Renderer m_Renderer = Bee::DX12::Renderer(
		BEE_DX12_RENDERER_MAKE_WINDOW_FLAG);

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Destroy() override;

};
