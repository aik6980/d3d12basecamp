#pragma once

#include "stdafx.h"

class ENGINE;

class MESH_RENDERER
{
public:
	MESH_RENDERER(ENGINE& engine);

	void init();
	void load_resources();
	void draw();
private:
	void build_pso_list();
	void build_pso_mesh_technique();
	void build_root_signature();

	ENGINE&	m_engine;

	// simple drawing
	unordered_map<string, ComPtr<ID3DBlob>>				m_shader_list;
	unordered_map<string, ComPtr<ID3D12PipelineState>>	m_pso_list; // PSO == Technique in D3D11
	ComPtr<ID3D12RootSignature>							m_root_signature;
};