#ifndef SCENE_5_H
#define SCENE_5_H

#include "Scene.h"
#include "Camera.h"
#include "Mesh.h"
#include "MatrixStack.h"


class Scene5 : public Scene
{
	enum GEOMETRY_TYPE {
		GEO_AXES = 0,
		GEO_QUAD,
		GEO_CUBE,
		GEO_CIRCLE,
		GEO_SPHERE,
		GEO_SPHERE_2,
		GEO_SPHERE_3,
		GEO_SPHERE_4,
		GEO_SPHERE_5,
		GEO_SPHERE_6,
		GEO_SPHERE_7,
		GEO_RING_7,
		GEO_SPHERE_8,
		GEO_SPHERE_9,
		NUM_GEOMETRY,
	};

	enum UNIFORM_TYPE {
		U_MVP = 0,
		U_TOTAL,
	};

	unsigned m_parameters[U_TOTAL];

public:
	Scene5();
	~Scene5();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
	unsigned m_programID;
	float rotateAngle, rotateAngle2, rotateAngle3;
	Camera camera;
	MS modelStack, viewStack, projectionStack;
};

#endif