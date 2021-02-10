#ifndef SCENE_ASSIGNMENT2_H
#define SCENE_ASSIGNMENT2_H

#include "Scene.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "LoadOBJ.h"

struct Vector2 //No y
{
	float x;
	float z;

	Vector2()
	{
		x, z = 0;
	}

	Vector2(float x, float z)
	{
		this->x = x;
		this->z = z;
	}
};

struct CHitbox //Use relative to object centre
{
	Vector3 minPoint;
	Vector3 maxPoint;
};

struct SphereHitbox
{
	Vector3 Centre;
	float radius;
	/**
	 Returns true if the difference of each axis is less than radius of the hitbox
	 */
	bool Collide(Vector3 ObjCtr, SphereHitbox Hitbox)
	{
		if ((ObjCtr-Hitbox.Centre).Length() <= Hitbox.radius)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};
class Assignment2 : public Scene
{
	enum GEOMETRY_TYPE {
		GEO_AXES = 0,
		GEO_QUAD,
		GEO_CUBE,
		GEO_CIRCLE,
		GEO_CYLINDER,
		GEO_SPHERE,
		GEO_BLACK_SPHERE,
		GEO_LIGHTBALL,
		GEO_LIGHTBALL_2,
		GEO_HEMISPHERE,
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,
		GEO_TEXT,
		GEO_SCORE,
		GEO_PROJECTILE,
		GEO_POS,
		GEO_MODEL,
		GEO_CROSSHAIR,
		GEO_HPBAR,
		GEO_SCOREBG,
		GEO_BEARHP, 
		GEO_TREE,
		GEO_STONE,
		GEO_MAIN,
		NUM_GEOMETRY,
	};

	enum UNIFORM_TYPE {
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHTENABLED,
		U_LIGHT0_TYPE,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
		// Light 2 //
		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,
		U_LIGHT1_TYPE,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,
		//Light 3
		U_LIGHT2_POSITION,
		U_LIGHT2_COLOR,
		U_LIGHT2_POWER,
		U_LIGHT2_KC,
		U_LIGHT2_KL,
		U_LIGHT2_KQ,
		U_LIGHT2_TYPE,
		U_LIGHT2_SPOTDIRECTION,
		U_LIGHT2_COSCUTOFF,
		U_LIGHT2_COSINNER,
		U_LIGHT2_EXPONENT,
		U_NUMLIGHTS,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,
	};

	unsigned m_parameters[U_TOTAL];

public:
	Assignment2();
	~Assignment2();
	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	Mesh* meshList[NUM_GEOMETRY];
	unsigned m_programID, m_vertexArrayID;
	float rotateAngle, rotateAngle2, LSPEED, FPS, HP, MAXHP, projectileFlyby, BearMoveSPD, BearHP, BearMAXHP, BearCD, Score;
	std::string BearHPText;
	SphereHitbox hitbox, headhitbox;
	Vector3 pelletPos, ProjectilePos, fireDir, fireOrigin, prevMoveTO; //Objects
	Vector3 playerView;
	Vector3 BearPosition; //Characters
	Color HPBarCol;
	bool pelletSpawned, Alive, ActiveProjectile, ActiveBear;
	Camera3 camera;
	MS modelStack, viewStack, projectionStack;
	void RenderMesh(Mesh* mesh, bool enableLight); //Render Mesh in world
	void FireProjectile(Vector3 direction, Vector3 position);
	float AngleBetweenVectors(Vector3 vec1, Vector3 vec2);
	void SpawnPellet();
	void RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey);
	void RenderHPBAROnScreen(Mesh* mesh, int x, int y, int sizex, int sizey);
	void RenderScoreBGOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey);
	Light light[3];
	bool CollisionCheck(Vector3 pos1, Vector3 pos2, float radius);
	void RenderBear();
	void RenderSkybox();
	void RenderText(Mesh* mesh,std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
};

#endif