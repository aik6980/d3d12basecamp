#pragma once

#include "stdafx.h"

struct CAMERA
{
	XMFLOAT4X4	view;
	XMFLOAT4X4	projection;
};

class MESH_RENDERER
{
public:
	void init();
	void destroy();

	void update();
private:
};