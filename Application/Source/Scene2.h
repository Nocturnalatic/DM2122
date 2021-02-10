#ifndef SCENE_2_H
#define SCENE_2_H

#include "Scene.h"

class Scene2 : public Scene
{
	enum GEOMETRY_TYPE {
		GEO_OBJ_1 = 0,
		GEO_OBJ_2,
		GEO_OBJ_3,
		GEO_OBJ_4,
		NUM_GEOMETRY,
	};

	enum UNIFORM_TYPE {
		U_MVP = 0,
		U_TOTAL,
	};

	unsigned m_parameters[U_TOTAL];

public:
	Scene2();
	~Scene2();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	unsigned m_vertexArrayID;
	unsigned m_vertexBuffer[NUM_GEOMETRY];
	unsigned m_colorBuffer[NUM_GEOMETRY];
	unsigned m_programID;
	float rotateAngle;
	int speed;
	float translateX;
	float translateX1;
	float translateX2;
	float scaleAll;
	float translateY;
};

#endif