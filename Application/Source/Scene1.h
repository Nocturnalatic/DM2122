#ifndef SCENE_1_H
#define SCENE_1_H

#include "Scene.h"

class Scene1 : public Scene
{
	enum GEOMETRY_TYPE {
		GEO_TRIANGLE_1 = 0,
		GEO_TRIANGLE_2 = 1,
		GEO_TRIANGLE_3 = 2,
		GEO_TRIANGLE_4 = 3,
		GEO_STAR_1 = 4,
		GEO_STAR_2 = 5,
		GEO_STAR_3 = 6,
		GEO_CS = 7,
		GEO_CS_2 = 8,
		NUM_GEOMETRY,
	};
public:
	Scene1();
	~Scene1();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	unsigned m_vertexArrayID;
	unsigned m_vertexBuffer[NUM_GEOMETRY];
	unsigned m_colorBuffer[NUM_GEOMETRY];
	unsigned m_programID;
};

#endif