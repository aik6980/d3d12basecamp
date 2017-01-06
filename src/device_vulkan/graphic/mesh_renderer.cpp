#include "stdafx.h"

#include "mesh_renderer.h"

#include "dev/resource_mgr.h"
#include "app/app.h"

void MESH_RENDERER::init()
{
	APP::g().m_render_resource->create_uniform_buffer(sizeof(CAMERA));
}

void MESH_RENDERER::update()
{
	CAMERA camera;

	auto eye	= XMVectorZero();
	auto up		= XMVectorSet(0, 1, 0, 0);
	auto target = XMVectorSet(0, 0, -1, 1);
	XMMatrixLookAtLH(eye, target, up);
}
