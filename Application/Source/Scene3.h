#ifndef SCENE_3_H
#define SCENE_3_H

#include "Scene.h"

class Scene3 : public Scene
{
	enum GEOMETRY_TYPE {
		GEO_TREE_1 = 0,
		GEO_TREE_2,
		GEO_TREE_BARK_1,
		GEO_TREE_BARK_2,
		GEO_BACKGROUND,
		GEO_GRASS_1,
		GEO_GRASS_2,
		GEO_SNOWFLAKE_1,
		GEO_SNOWFLAKE_2,
		GEO_SNOWFLAKE_1a,
		GEO_SNOWFLAKE_1b,
		GEO_SNOWFLAKE_2a,
		GEO_SNOWFLAKE_2b,
		GEO_SNOWFLAKE_3a,
		GEO_SNOWFLAKE_3b,
		GEO_MOON_1a,
		GEO_MOON_1b,
		NUM_GEOMETRY,
	};

	enum UNIFORM_TYPE {
		U_MVP = 0,
		U_TOTAL,
	};

	unsigned m_parameters[U_TOTAL];

public:
	Scene3();
	~Scene3();

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
	float snowflakeFall;
};

#endif