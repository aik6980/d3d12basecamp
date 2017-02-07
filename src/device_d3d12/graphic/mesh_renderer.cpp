#include "stdafx.h"

#include "mesh_renderer.h"

#include "common/common.cpp.h"
#include "engineImpl.h"
#include "engine.h"
#include "renderer/renderer_cpp.h"


MESH_RENDERER::MESH_RENDERER(ENGINE& engine)
	: m_engine(engine)
{

}

void MESH_RENDERER::init()
{

}

void MESH_RENDERER::load_resources()
{
	m_shader_list["default.vs"] = D3D12::SHADER_MANAGER::load_from_objfile(L"shaders_hlsl/default.vs.obj");
	m_shader_list["mesh.vs"] = D3D12::SHADER_MANAGER::load_from_objfile(L"shaders_hlsl/mesh.vs.obj");
	m_shader_list["default.ps"] = D3D12::SHADER_MANAGER::load_from_objfile(L"shaders_hlsl/default.ps.obj");

	build_root_signature();
	build_pso_list();

	if (!m_engine.resource_mgr().request_mesh_buffer(m_mesh_buffer_name))
	{
		build_geometry();
	}
}

void MESH_RENDERER::draw()
{
	auto&& render_device = m_engine.render_device();
	auto&& command_list = m_engine.render_device().commmand_list();

	// Specify the buffers we are going to render to.
	command_list->OMSetRenderTargets(1, &render_device.curr_backbuffer_view(), true, 
		&render_device.curr_backbuffer_depth_stencil_view());
	command_list->RSSetViewports(1, &render_device.get_window_viewport());
	command_list->RSSetScissorRects(1, &render_device.get_window_rect());
	
	command_list->SetPipelineState(m_pso_list["default"].Get());
	command_list->SetGraphicsRootSignature(m_root_signature.Get());
	command_list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	command_list->DrawInstanced(3, 1, 0, 0);

	auto&& mesh_buffer = m_engine.resource_mgr().request_mesh_buffer(m_mesh_buffer_name);
	if (mesh_buffer)
	{
		command_list->SetPipelineState(m_pso_list["mesh"].Get());
		command_list->IASetVertexBuffers(mesh_buffer.m_vertex_buffer_gpu)
		command_list->DrawIndexedInstanced(6, 1, 0, 0, 0);
	}
}

void MESH_RENDERER::build_pso_list()
{
	D3D12::GRAPHICS_PIPELINE_STATE_DESC pso_desc;
	pso_desc.NumRenderTargets = 1;
	pso_desc.RTVFormats[0]	= m_engine.render_device().get_swap_chain_desc().BufferDesc.Format;
	pso_desc.DSVFormat = m_engine.render_device().get_depth_stencil_format();

	pso_desc.set_vertex_shader(m_shader_list["default.vs"].Get());
	pso_desc.set_pixel_shader(m_shader_list["default.ps"].Get());

	pso_desc.pRootSignature = m_root_signature.Get();

	DBG::ThrowIfFailed(m_engine.render_device().
		device()->CreateGraphicsPipelineState(&pso_desc, IID_PPV_ARGS(&m_pso_list["default"])));

	build_pso_mesh_technique();
}

void MESH_RENDERER::build_pso_mesh_technique()
{
	D3D12::GRAPHICS_PIPELINE_STATE_DESC pso_desc;
	pso_desc.NumRenderTargets = 1;
	pso_desc.RTVFormats[0] = m_engine.render_device().get_swap_chain_desc().BufferDesc.Format;
	pso_desc.DSVFormat = m_engine.render_device().get_depth_stencil_format();

	pso_desc.set_vertex_shader(m_shader_list["mesh.vs"].Get());
	pso_desc.set_pixel_shader(m_shader_list["default.ps"].Get());

	// create input_layout here
	D3D12::INPUT_LAYOUT_ELEMENT_DESC elem;
	m_input_layout_desc.m_layout.push_back(elem);
	//////////////////////////////////////////////////////////////////////////
	pso_desc.InputLayout = m_input_layout_desc.as_d3d12_input_layout_desc();
	pso_desc.pRootSignature = m_root_signature.Get();

	DBG::ThrowIfFailed(m_engine.render_device().
		device()->CreateGraphicsPipelineState(&pso_desc, IID_PPV_ARGS(&m_pso_list["mesh"])));
}

void MESH_RENDERER::build_root_signature()
{
	// A root signature is an array of root parameters.
	CD3DX12_ROOT_SIGNATURE_DESC root_sig_desc(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
	//CD3DX12_ROOT_SIGNATURE_DESC root_sig_desc(0, nullptr);

	ComPtr<ID3DBlob> serialized_root_sig = nullptr;
	ComPtr<ID3DBlob> error_blob = nullptr;
	DBG::ThrowIfFailed(D3D12SerializeRootSignature(&root_sig_desc, D3D_ROOT_SIGNATURE_VERSION_1,
		&serialized_root_sig, &error_blob));

	if (error_blob != nullptr)
	{
		DBG::OutputString((char*)error_blob->GetBufferPointer());
	}

	auto&& render_device = m_engine.render_device();
	DBG::ThrowIfFailed(render_device.device()->CreateRootSignature(
		0,
		serialized_root_sig->GetBufferPointer(),
		serialized_root_sig->GetBufferSize(),
		IID_PPV_ARGS(&m_root_signature)));
}

void MESH_RENDERER::build_geometry()
{
	MESH_VERTEX_ARRAY verts;
	MESH_INDEX_ARRAY indices;

	MESH_DATA_GENERATOR::create_unit_quad(verts, indices);

	// upload data to the GPU
	auto&& device = m_engine.render_device().device();
	auto&& cmd_list = m_engine.render_device().commmand_list();
	auto&& frame_resource = m_engine.render_device().frame_resource();

	auto&& vtx_data = verts.m_position.data();
	auto&& vtx_size = verts.m_position.size() * sizeof(XMFLOAT3);
	auto&& vb = m_engine.resource_mgr().create_default_buffer(*device, *cmd_list, frame_resource, vtx_size, vtx_data);

	auto&& idx_data = indices.m_indices32.data();
	auto&& idx_size = indices.m_indices32.size() * sizeof(uint32_t);
	auto&& ib = m_engine.resource_mgr().create_default_buffer(*device, *cmd_list, frame_resource, idx_size, idx_data);

	D3D12::MESH_BUFFER mesh_buffer;
	mesh_buffer.m_vertex_buffer_gpu = vb;
	mesh_buffer.m_index_buffer_gpu = ib;
	D3D12::MESH_LOCATION loc;
	loc.index_count = (uint32_t)indices.m_indices32.size();
	mesh_buffer.m_mesh_location_list[m_mesh_buffer_name] = loc;

	m_engine.resource_mgr().register_mesh_buffer(m_mesh_buffer_name, mesh_buffer);
}

