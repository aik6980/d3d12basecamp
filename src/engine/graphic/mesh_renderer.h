#pragma once


class ENGINE;

class MESH_RENDERER
{
public:
	MESH_RENDERER(ENGINE& engine);

	void init();
private:
	ENGINE&	m_engine;

	// for a simple draw

};