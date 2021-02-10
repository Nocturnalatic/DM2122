#define _USE_MATH_DEFINES
#include "Assignment2.h"
#include "GL\glew.h"
#include "Mtx44.h"
#include <time.h>
#include <random>
#include <iostream>
#include "shader.hpp"
#include "Application.h"
#include "MeshBuilder.h"
#include "Vertex.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <iomanip>
#include <time.h>
#include <string>
#include <cmath>

Assignment2::Assignment2()
{
	ProjectilePos = Vector3(0, -1000, 0);
	rotateAngle = Score = 0.f;
	LSPEED = 100.f;
	HP = MAXHP = 100;
	BearHP = BearMAXHP = 80;
	BearCD = 600;
	BearPosition = Vector3(20, -13, 20);
	prevMoveTO = Vector3(0, 0, 1);
	hitbox.Centre = BearPosition; headhitbox.Centre = BearPosition + Vector3(0, 1.25 * 2, 0);
	headhitbox.radius = 1; hitbox.radius = 2; BearMoveSPD = .25f;
	Alive = true;
	ActiveBear = false;
	ActiveProjectile = false;
}

Assignment2::~Assignment2()
{

}


void Assignment2::Init()
{
	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f/3.f, 0.1f, 1000.0f);
	projectionStack.LoadMatrix(projection);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");
	// ↓ m_parameters ↓ //
	{
		m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
		m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
		m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
		m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
		m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
		m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
		m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
		m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
		m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
		m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
		m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
		m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
		m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
		m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
		m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
		m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
		m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
		m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
		m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
		m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
		m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
		m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
		m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
		m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
		m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
		m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
		m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
		m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
		m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
		m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
		m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");
		m_parameters[U_LIGHT2_POSITION] = glGetUniformLocation(m_programID, "lights[2].position_cameraspace");
		m_parameters[U_LIGHT2_COLOR] = glGetUniformLocation(m_programID, "lights[2].color");
		m_parameters[U_LIGHT2_POWER] = glGetUniformLocation(m_programID, "lights[2].power");
		m_parameters[U_LIGHT2_KC] = glGetUniformLocation(m_programID, "lights[2].kC");
		m_parameters[U_LIGHT2_KL] = glGetUniformLocation(m_programID, "lights[2].kL");
		m_parameters[U_LIGHT2_KQ] = glGetUniformLocation(m_programID, "lights[2].kQ");
		m_parameters[U_LIGHT2_TYPE] = glGetUniformLocation(m_programID, "lights[2].type");
		m_parameters[U_LIGHT2_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[2].spotDirection");
		m_parameters[U_LIGHT2_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[2].cosCutoff");
		m_parameters[U_LIGHT2_COSINNER] = glGetUniformLocation(m_programID, "lights[2].cosInner");
		m_parameters[U_LIGHT2_EXPONENT] = glGetUniformLocation(m_programID, "lights[2].exponent");
		m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
		m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
		m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
		m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
		m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");
	}
	glUseProgram(m_programID);
	// ↓ Light Initialisation ↓ //
	{
		light[0].type = Light::LIGHT_POINT;
		light[0].position.Set(0, 350, 0);
		light[0].color.Set(1, 1, 0.7f);
		light[0].power = 200;
		light[0].kC = 1.f;
		light[0].kL = 0.01f;
		light[0].kQ = 0.001f;
		light[0].cosCutoff = cos(Math::DegreeToRadian(45));
		light[0].cosInner = cos(Math::DegreeToRadian(30));
		light[0].exponent = 1.f;
		light[0].spotDirection.Set(0.f, 1.f, 0.f);

		light[1].type = Light::LIGHT_POINT;
		light[1].position.Set(0, 0, 0);
		light[1].color.Set(1, 0.5, 0);
		light[1].power = 1.f;
		light[1].kC = 1.f;
		light[1].kL = 0.01f;
		light[1].kQ = 0.001f;
		light[1].cosCutoff = cos(Math::DegreeToRadian(45));
		light[1].cosInner = cos(Math::DegreeToRadian(30));
		light[1].exponent = 3.f;
		light[1].spotDirection.Set(0.f, 1.f, 0.f);

		light[2].type = Light::LIGHT_POINT;
		light[2].position.Set(0, 0, 0);
		light[2].color.Set(0, 1, 1);
		light[2].power = 1.2f;
		light[2].kC = 1.f;
		light[2].kL = 0.01f;
		light[2].kQ = 0.001f;
		light[2].cosCutoff = cos(Math::DegreeToRadian(45));
		light[2].cosInner = cos(Math::DegreeToRadian(30));
		light[2].exponent = 3.f;
		light[2].spotDirection.Set(0.f, 1.f, 0.f);
	}

	// Make sure you pass uniform parameters after glUseProgram()
	glUniform1i(m_parameters[U_NUMLIGHTS], 3); // Change for multiple lights
	// Light 1
	{
		glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
		glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
		glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
		glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
		glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);
		glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
		glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
		glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
		glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
		glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
		glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);
		glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], light[0].cosCutoff);
		glUniform1f(m_parameters[U_LIGHT0_COSINNER], light[0].cosInner);
		glUniform1f(m_parameters[U_LIGHT0_EXPONENT], light[0].exponent);
	}
	// Light 2
	{
		glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &light[1].color.r);
		glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
		glUniform1f(m_parameters[U_LIGHT1_KC], light[1].kC);
		glUniform1f(m_parameters[U_LIGHT1_KL], light[1].kL);
		glUniform1f(m_parameters[U_LIGHT1_KQ], light[1].kQ);
		glUniform1i(m_parameters[U_LIGHT1_TYPE], light[1].type);
		glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &light[1].color.r);
		glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
		glUniform1f(m_parameters[U_LIGHT1_KC], light[1].kC);
		glUniform1f(m_parameters[U_LIGHT1_KL], light[1].kL);
		glUniform1f(m_parameters[U_LIGHT1_KQ], light[1].kQ);
		glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], light[1].cosCutoff);
		glUniform1f(m_parameters[U_LIGHT1_COSINNER], light[1].cosInner);
		glUniform1f(m_parameters[U_LIGHT1_EXPONENT], light[1].exponent);
	}
	// Light 3
	{
		glUniform3fv(m_parameters[U_LIGHT2_COLOR], 1, &light[2].color.r);
		glUniform1f(m_parameters[U_LIGHT2_POWER], light[2].power);
		glUniform1f(m_parameters[U_LIGHT2_KC], light[2].kC);
		glUniform1f(m_parameters[U_LIGHT2_KL], light[2].kL);
		glUniform1f(m_parameters[U_LIGHT2_KQ], light[2].kQ);
		glUniform1i(m_parameters[U_LIGHT2_TYPE], light[2].type);
		glUniform3fv(m_parameters[U_LIGHT2_COLOR], 1, &light[2].color.r);
		glUniform1f(m_parameters[U_LIGHT2_POWER], light[2].power);
		glUniform1f(m_parameters[U_LIGHT2_KC], light[2].kC);
		glUniform1f(m_parameters[U_LIGHT2_KL], light[2].kL);
		glUniform1f(m_parameters[U_LIGHT2_KQ], light[2].kQ);
		glUniform1f(m_parameters[U_LIGHT2_COSCUTOFF], light[2].cosCutoff);
		glUniform1f(m_parameters[U_LIGHT2_COSINNER], light[2].cosInner);
		glUniform1f(m_parameters[U_LIGHT2_EXPONENT], light[2].exponent);
	}
	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);// Set BG Color

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuadLargeTexture("quad",Color(.7f,.7f,.7f),1.f);
	meshList[GEO_QUAD]->textureID = LoadTGALoop("Image//grass.tga");
	// ↓ Init Skyboxes ↓
	{
		meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.f);
		meshList[GEO_FRONT]->textureID = LoadTGA("Image//front1.tga");
		meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1), 1.f);
		meshList[GEO_BACK]->textureID = LoadTGA("Image//back1.tga");
		meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1), 1.f);
		meshList[GEO_TOP]->textureID = LoadTGA("Image//top1.tga");
		meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1.f);
		meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//bottom1.tga");
		meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1), 1.f);
		meshList[GEO_LEFT]->textureID = LoadTGA("Image//left1.tga");
		meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1), 1.f);
		meshList[GEO_RIGHT]->textureID = LoadTGA("Image//right1.tga");
	} 
	meshList[GEO_CROSSHAIR] = MeshBuilder::GenerateQuad("crosshair", Color(1,1,1), 1);
	meshList[GEO_CROSSHAIR]->textureID = LoadTGA("Image//crosshair.tga");
	meshList[GEO_HPBAR] = MeshBuilder::GenerateQuad("hpbar", Color(0,1,0), 1);
	meshList[GEO_SCOREBG] = MeshBuilder::GenerateQuad("scorebg", Color(0,0,0), 1);
	meshList[GEO_BEARHP] = MeshBuilder::GenerateText("bearhp", 16, 16);
	meshList[GEO_BEARHP]->textureID = LoadTGA("Image//Arial_N.tga");
	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", Color(1,0,1), 1.f);
	meshList[GEO_CIRCLE] = MeshBuilder::GenerateCircle("circle", Color(0.9f,1,0), 30, 1);
	meshList[GEO_CYLINDER] = MeshBuilder::GenerateCylinder("Cylinder", Color(1,0.5f,0), 19, 19, .2f, 1);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("Pellet", Color(1,0.5f,0), 32, 32, 1);
	meshList[GEO_BLACK_SPHERE] = MeshBuilder::GenerateSphere("BlackSphere", Color(0, 0, 0), 32, 32, 1);
	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("Light",Color(1,1,0.8f), 10, 10, 1);
	meshList[GEO_LIGHTBALL_2] = MeshBuilder::GenerateSphere("Light2", Color(1, 0, 0), 10, 10, 1);
	meshList[GEO_PROJECTILE] = MeshBuilder::GenerateSphere("proj", Color(0,1,1), 32, 32, 1);
	meshList[GEO_HEMISPHERE] = MeshBuilder::GenerateHemisphere("eyebrows", Color(0.1f, 0.1f, 0.1f), 10, 10, 1);
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//Arial_N.tga");
	meshList[GEO_SCORE] = MeshBuilder::GenerateText("score", 16, 16);
	meshList[GEO_SCORE]->textureID = LoadTGA("Image//Arial_N.tga");
	meshList[GEO_POS] = MeshBuilder::GenerateText("pos", 16, 16);
	meshList[GEO_POS]->textureID = LoadTGA("Image//Arial_N.tga");
	meshList[GEO_MODEL] = MeshBuilder::GenerateOBJMTL("cottage","OBJ//cottage_obj.obj", "OBJ//cottage_obj.mtl");
	meshList[GEO_MODEL]->textureID = LoadTGA("Image//cottage_diffuse.tga");
	meshList[GEO_TREE] = MeshBuilder::GenerateOBJMTL("tree", "OBJ//tree1.obj", "OBJ//tree1.mtl");
	meshList[GEO_STONE] = MeshBuilder::GenerateOBJMTL("stone", "OBJ//stone.obj", "OBJ//stone.mtl");
	meshList[GEO_MAIN] = MeshBuilder::GenerateOBJ("main", "OBJ//protagonist.obj");
	meshList[GEO_MAIN]->material.kAmbient.Set(0,0,0);

	camera.Init(Vector3(20,-10,15), Vector3(0,0,0), Vector3(0,1,0));
	Mesh::SetMaterialLoc(m_parameters[U_MATERIAL_AMBIENT], m_parameters[U_MATERIAL_DIFFUSE], 
		m_parameters[U_MATERIAL_SPECULAR], m_parameters[U_MATERIAL_SHININESS]);
}

void Assignment2::Update(double dt)
{
	camera.Update(dt);
	playerView = Vector3(camera.view.x, 0, camera.view.z);
	if (playerView.x > 0 and playerView.z > 0)
	{
		rotateAngle = 360 - AngleBetweenVectors(playerView, Vector3(0, 0, 1));
	}
	else if (playerView.x < 0 and playerView.z < 0)
	{
		rotateAngle = AngleBetweenVectors(playerView, Vector3(0, 0, 1));
	}
	else if (playerView.x > 0 and playerView.z < 0)
	{
		rotateAngle = 360 - AngleBetweenVectors(playerView, Vector3(0, 0, 1));
	}
	else
	{
		rotateAngle = AngleBetweenVectors(playerView, Vector3(0, 0, 1));
	}
	FPS = 1 / float(dt);
	projectileFlyby += 2;
	HPBarCol = Color(1 - HP / MAXHP, HP / MAXHP, 0);
	meshList[GEO_HPBAR]->material.kAmbient.Set(HPBarCol.r, HPBarCol.g, HPBarCol.b);
	light[2].position.Set(ProjectilePos.x, ProjectilePos.y, ProjectilePos.z);
	if (!Alive)
	{
		Sleep(3000);
		HP = 100;
		camera.position = Vector3(20, -10, 15);
		Alive = true;
		camera.doCamera = true;
	}
	if (Alive)
	{
		Score += 0.2;
		HP -= 0.015f;
	}
	if (!pelletSpawned)
	{
		SpawnPellet();
	}
	if (Application::IsKeyPressed('E'))
	{
		if ((CollisionCheck(camera.position, pelletPos, 4)) && (pelletSpawned))
		{
			Score += 50;
			HP += 8;
			pelletPos.y = -1000;
			pelletSpawned = false;
		}
	}
	if (Application::IsKeyPressed(VK_SPACE))
	{
		if (!ActiveProjectile)
		{
			FireProjectile(camera.target, camera.position);
		}
	}
	if (rotateAngle > 360)
	{
		rotateAngle = 0;
	}
	if (BearHP <= 0.f)
	{
		BearPosition = Vector3(0, -1000, 0);
		ActiveBear = false;
		BearHPText = "";
	}
	if (ActiveBear)
	{
		Vector3 MoveTO = (camera.position - BearPosition).Normalized();
		if (MoveTO.x > 0 and MoveTO.z > 0)
		{
			rotateAngle2 = 360 - AngleBetweenVectors(MoveTO, Vector3(0, 0, 1));
		}
		else if (MoveTO.x < 0 and MoveTO.z < 0)
		{
			rotateAngle2 = AngleBetweenVectors(MoveTO, Vector3(0, 0, 1));
		}
		else if (MoveTO.x > 0 and MoveTO.z < 0)
		{
			rotateAngle2 = 360 - AngleBetweenVectors(MoveTO, Vector3(0, 0, 1));
		}
		else
		{
			rotateAngle2 = AngleBetweenVectors(MoveTO, Vector3(0, 0, 1));
		}
		BearPosition += MoveTO * BearMoveSPD;
		hitbox.Centre = BearPosition;
		headhitbox.Centre = BearPosition + Vector3(0, 1.25 * 2, 0);
		BearHPText = "HP:" + std::to_string(int(BearHP)) + "/" + std::to_string(int(BearMAXHP));
		if (CollisionCheck(camera.position, BearPosition, 2))
		{
			HP -= 1;
		}
	}
	else
	{
		BearCD -= 1;
	}
	if (BearCD <= 0 && !ActiveBear) //Respawn
	{
		BearCD = 600;
		BearPosition = Vector3(0, -13, 0);
		BearMAXHP = BearMAXHP * 1.25;
		BearMoveSPD += 0.01;
		BearHP = BearMAXHP;
		ActiveBear = true;
	}
	if (ActiveProjectile)
	{
		ProjectilePos = fireOrigin + (projectileFlyby * fireDir);
		if (projectileFlyby >= 200)
		{
			ProjectilePos = Vector3(0, -1000, 0);
			ActiveProjectile = false;
		}
		if (hitbox.Collide(ProjectilePos, hitbox))
		{
			ProjectilePos = Vector3(0, -1000, 0);
			Score += 5;
			BearHP -= 2;
			HP += 0.5;
			ActiveProjectile = false;
		}
		if (hitbox.Collide(ProjectilePos, headhitbox))
		{
			ProjectilePos = Vector3(0, -1000, 0);
			Score += 10;
			BearHP -= 4;
			HP += 1;
			ActiveProjectile = false;
		}
		if (ProjectilePos.y < -15)
		{
			ProjectilePos = Vector3(0, -1000, 0);
			ActiveProjectile = false;
		}
	}

	if (HP <= 0)
	{
		Score = 0;
		Alive = false;
	}
	if (Alive == false)
	{
		camera.target = Vector3(0, 1, 0);
		camera.doCamera = false;
	}
	if (HP > MAXHP)
	{
		HP = MAXHP;
	}
	if (Application::IsKeyPressed('8'))
	{
		HP++;
	}
	if (Application::IsKeyPressed('9'))
	{
		HP--;
	}
	if (Application::IsKeyPressed('1'))
	{
		glEnable(GL_CULL_FACE);
	}
	if (Application::IsKeyPressed('2'))
	{
		glDisable(GL_CULL_FACE);
	}
	if (Application::IsKeyPressed('3'))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	if (Application::IsKeyPressed('4'))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (Application::IsKeyPressed('I'))
		light[0].position.z -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('K'))
		light[0].position.z += (float)(LSPEED * dt);
	if (Application::IsKeyPressed('J'))
		light[0].position.x -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('L'))
		light[0].position.x += (float)(LSPEED * dt);
	if (Application::IsKeyPressed('O'))
		light[0].position.y -= (float)(LSPEED * dt);
	if (Application::IsKeyPressed('P'))
		light[0].position.y += (float)(LSPEED * dt);

	if (Application::IsKeyPressed('5'))
	{
		light[0].type = Light::LIGHT_TYPE::LIGHT_POINT;
	}
	else if (Application::IsKeyPressed('6'))
	{
		light[0].type = Light::LIGHT_TYPE::LIGHT_DIRECTIONAL;
	}
	else if (Application::IsKeyPressed('7'))
	{
		light[0].type = Light::LIGHT_TYPE::LIGHT_SPOT;
	}

	glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	glUniform1i(m_parameters[U_LIGHT1_TYPE], light[1].type);
	glUniform1i(m_parameters[U_LIGHT1_TYPE], light[2].type);
}

void Assignment2::FireProjectile(Vector3 direction, Vector3 position)
{
	fireOrigin = position;
	fireDir = (direction - position).Normalize();
	projectileFlyby = 0;
	ActiveProjectile = true;
}

void Assignment2::SpawnPellet()
{
	float x = float(rand() % 400) - 200;
	float z = float(rand() % 400) - 200;
	pelletPos = Vector3(x, -14, z);
	light[1].position.Set(x, -10, z);

	std::cout << pelletPos << std::endl;
	std::cout << "Light Position: " << std::to_string(light[1].position.x) + std::to_string(light[1].position.y) + std::to_string(light[1].position.z) << std::endl;
	pelletSpawned = true;
}

float Assignment2::AngleBetweenVectors(Vector3 vec1, Vector3 vec2)
{
	float product = acosf((vec1.Dot(vec2))/((vec1.Length()*vec2.Length())));
	return product * (180.f / M_PI);
}

bool Assignment2::CollisionCheck(Vector3 pos1, Vector3 pos2, float radius) //pos1 is player position, pos2 is object position
{
	if ((fabs(pos1.x - pos2.x) <= radius) && (fabs(pos1.y - pos2.y) <= radius) && (fabs(pos1.z - pos2.z) <= radius))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Assignment2::RenderBear()
{
	modelStack.PushMatrix();
	//to do: transformation code here
	modelStack.Translate(BearPosition.x, BearPosition.y,BearPosition.z);
	modelStack.Rotate(-rotateAngle2, 0, 1, 0);
	modelStack.Scale(2, 2, 2);
	RenderMesh(meshList[GEO_SPHERE], true); //Body
	modelStack.PushMatrix();
	//to do: transformation code here
	modelStack.Translate(0, 1.25f, 0);
	modelStack.Scale(0.5f, 0.5f, 0.5f);
	RenderMesh(meshList[GEO_SPHERE], true); //Head
	modelStack.PushMatrix();
	//to do: transformation code here
	modelStack.Translate(-0.75f, 0.7f, 0);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Rotate(-45, 0, 1, 0);
	modelStack.Scale(.4f, .4f, .4f);
	RenderMesh(meshList[GEO_CYLINDER], true); //Left Ear
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	//to do: transformation code here
	modelStack.Translate(0.75f, 0.7f, 0);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Rotate(45, 0, 1, 0);
	modelStack.Scale(.4f, .4f, .4f);
	RenderMesh(meshList[GEO_CYLINDER], true); //Right Ear
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	//to do: transformation code here
	modelStack.Translate(.52f, 0.7f, .44f);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Rotate(45, 0, 1, 0);
	modelStack.Scale(0.1f, 0.1f, 0.1f);
	RenderMesh(meshList[GEO_HEMISPHERE], true); //Left Eyebrow
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	//to do: transformation code here
	modelStack.Translate(-.52f, 0.7f, .44f);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Rotate(-45, 0, 1, 0);
	modelStack.Scale(0.1f, 0.1f, 0.1f);
	RenderMesh(meshList[GEO_HEMISPHERE], true); //Right Eyebrow
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	//to do: transformation code here
	modelStack.Translate(-.3f, 0.45f, 0.8f);
	modelStack.Rotate(0, 0, 1, 0);
	modelStack.Scale(0.08f, 0.08f, 0.08f);
	RenderMesh(meshList[GEO_HEMISPHERE], true); //Left Eye
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	//to do: transformation code here
	modelStack.Translate(.3f, 0.45f, 0.8f);
	modelStack.Rotate(0, 0, 1, 0);
	modelStack.Scale(0.08f, 0.08f, 0.08f);
	RenderMesh(meshList[GEO_HEMISPHERE], true); //Right Eye
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	//to do: transformation code here
	modelStack.Translate(0, 0.2f, 1);
	modelStack.Rotate(0, 0, 1, 0);
	modelStack.Scale(0.12f, 0.12f, 0.12f);
	RenderMesh(meshList[GEO_BLACK_SPHERE], true); //Nose
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	//to do: transformation code here
	modelStack.Translate(0, -0.270f, 0.865f);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(0.4f, 0.1f, 0.2f);
	RenderMesh(meshList[GEO_HEMISPHERE], true); //Mouth
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	//to do: transformation code here
	modelStack.Translate(-.88f, 0.5f, 0);
	modelStack.Scale(0.3f, 0.3f, 0.3f);
	RenderMesh(meshList[GEO_SPHERE], true); //Left Joint
	modelStack.PushMatrix();
	//to do: transformation code here
	modelStack.Translate(0, 0, 2.25f);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(0.45f / 2 / 0.3f, 15 / 2 / 0.3f, 0.45f / 2 / 0.3f);
	RenderMesh(meshList[GEO_CYLINDER], true); //Left Hand
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	//to do: transformation code here
	modelStack.Translate(.88f, 0.5f, 0);
	modelStack.Scale(0.3f, 0.3f, 0.3f);
	RenderMesh(meshList[GEO_SPHERE], true); //Right Joint
	modelStack.PushMatrix();
	//to do: transformation code here
	modelStack.Translate(0, 0, 2.25f);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(0.45f / 2 / 0.3f, 15 / 2 / 0.3f, 0.45f / 2 / 0.3f);
	RenderMesh(meshList[GEO_CYLINDER], true); //Right Hand
	modelStack.PopMatrix();
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	//to do: transformation code here
	modelStack.Translate(-0.5f, -0.5f, 0.6f);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(0.35f, 10 / 2, 0.7f / 2);
	RenderMesh(meshList[GEO_CYLINDER], true); //Left Foot
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	//to do: transformation code here
	modelStack.Translate(.5f, -0.5f, 0.6f);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(0.35f, 5, 0.35f);
	RenderMesh(meshList[GEO_CYLINDER], true); //Right Foot
	modelStack.PopMatrix();
	modelStack.PopMatrix();
}

void Assignment2::RenderMesh(Mesh* mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
	if (enableLight)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}
	if (light[0].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(light[0].position.x, light[0].position.y, light[0].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (light[0].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * light[0].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else 
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	}
	if (light[1].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(light[1].position.x, light[1].position.y, light[1].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (light[1].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * light[1].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
	}
	if (light[2].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(light[2].position.x, light[2].position.y, light[2].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (light[2].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[2].position;
		glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * light[2].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT2_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[2].position;
		glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1, &lightPosition_cameraspace.x);
	}

	if (mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render();
	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Assignment2::RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey)
{
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	//to do: scale and translate accordingly
	modelStack.Translate(40,30,0);
	modelStack.Scale(20, 20, 20);
	RenderMesh(mesh, false); //UI should not have light
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void Assignment2::RenderHPBAROnScreen(Mesh* mesh, int x, int y, int sizex, int sizey)
{
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	//to do: scale and translate accordingly
	modelStack.Translate(-10+(float(HP*2)/MAXHP)*10, 5, 0);
	modelStack.Scale(20, 1, 1);
	RenderMesh(mesh, false); //UI should not have light
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void Assignment2::RenderScoreBGOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey)
{
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	//to do: scale and translate accordingly
	modelStack.Translate(40, 59,0);
	modelStack.Scale(80, 2, 1);
	RenderMesh(mesh, false); //UI should not have light
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void Assignment2::RenderSkybox()
{
	modelStack.PushMatrix(); //TOP
	modelStack.Translate(0 + camera.position.x, 500 + camera.position.y, 0 + camera.position.z);
	modelStack.Rotate(90,1,0,0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix(); //TOP
	modelStack.Translate(0 + camera.position.x, -500 + camera.position.y, 0 + camera.position.z);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix(); //TOP
	modelStack.Translate(-500 + camera.position.x, 0 + camera.position.y, 0 + camera.position.z);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix(); //TOP
	modelStack.Translate(500 + camera.position.x, 0 + camera.position.y, 0 + camera.position.z);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix(); //FRONT
	modelStack.Translate(0 + camera.position.x, 0 + camera.position.y, -500 + camera.position.z);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix(); //FRONT
	modelStack.Translate(0 + camera.position.x, 0 + camera.position.y, 500 + camera.position.z);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(1000, 1000, 1000);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();
}

void Assignment2::RenderText(Mesh* mesh, std::string text, Color color)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;

	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

void Assignment2::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;

	glDisable(GL_DEPTH_TEST);
	//Add these code just after glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);

	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(0.5f + i * 1.0f, 0.5f, 0);
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);


		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void Assignment2::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Init //
	Mtx44 MVP;

	viewStack.LoadIdentity();
	viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z, camera.target.x, camera.target.y, camera.target.z, camera.up.x, camera.up.y, camera.up.z);
	modelStack.LoadIdentity();
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	RenderMesh(meshList[GEO_AXES], false);
	Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
	glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	lightPosition_cameraspace = viewStack.Top() * light[1].position;
	glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
	lightPosition_cameraspace = viewStack.Top() * light[2].position;
	glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1, &lightPosition_cameraspace.x);

	RenderSkybox();
	RenderBear();

	modelStack.PushMatrix();
	modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
	RenderMesh(meshList[GEO_LIGHTBALL], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	//to do: transformation code here
	modelStack.Translate(0, -15, 0);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(10000, 10000, 10000);
	RenderMesh(meshList[GEO_QUAD], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	//scale, translate, rotate
	modelStack.Translate(0, -15, 0);
	modelStack.Scale(1, 1, 1);
	RenderMesh(meshList[GEO_MODEL], true);
	modelStack.PopMatrix();

	for (float i = 0; i < 250; i+=25)
	{
		modelStack.PushMatrix();
		//scale, translate, rotate
		modelStack.Translate(-30+i, -15, i/2);
		modelStack.Scale(25, 25, 25);
		RenderMesh(meshList[GEO_TREE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		//scale, translate, rotate
		modelStack.Translate(30 + i, -15, -i / 2);
		modelStack.Scale(25, 25, 25);
		RenderMesh(meshList[GEO_TREE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		//scale, translate, rotate
		modelStack.Translate(-30 - i, -15, i / 2);
		modelStack.Scale(25, 25, 25);
		RenderMesh(meshList[GEO_TREE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		//scale, translate, rotate
		modelStack.Translate(-30 - i, -15, -i / 2);
		modelStack.Scale(25, 25, 25);
		RenderMesh(meshList[GEO_TREE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		//scale, translate, rotate
		modelStack.Translate(-30 + i/2, -15, i / 2);
		modelStack.Scale(25, 25, 25);
		RenderMesh(meshList[GEO_TREE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		//scale, translate, rotate
		modelStack.Translate(30 + i/2, -15, -i / 2);
		modelStack.Scale(25, 25, 25);
		RenderMesh(meshList[GEO_TREE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		//scale, translate, rotate
		modelStack.Translate(-30 - i/2, -15, i / 2);
		modelStack.Scale(25, 25, 25);
		RenderMesh(meshList[GEO_TREE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		//scale, translate, rotate
		modelStack.Translate(-30 - i/2, -15, -i / 2);
		modelStack.Scale(25, 25, 25);
		RenderMesh(meshList[GEO_TREE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		//scale, translate, rotate
		modelStack.Translate(-30 + i, -15, 0);
		modelStack.Scale(25, 25, 25);
		RenderMesh(meshList[GEO_TREE], true);
		modelStack.PopMatrix();
	}

	modelStack.PushMatrix();
	//scale, translate, rotate
	modelStack.Translate(50, -15, 0);
	modelStack.Scale(25, 25, 25);
	RenderMesh(meshList[GEO_STONE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	//scale, translate, rotate
	modelStack.Translate(pelletPos.x, pelletPos.y, pelletPos.z);
	modelStack.Scale(1, 1, 1);
	RenderMesh(meshList[GEO_SPHERE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	//scale, translate, rotate
	modelStack.Translate(ProjectilePos.x, ProjectilePos.y, ProjectilePos.z);
	modelStack.Scale(0.25, 0.25, .25);
	RenderMesh(meshList[GEO_PROJECTILE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	//scale, translate, rotate
	modelStack.Translate(BearPosition.x - (BearHPText.length()/2), BearPosition.y + 5, BearPosition.z);
	modelStack.Rotate(-rotateAngle2, 0, 1, 0);
	modelStack.Scale(1, 1, 1);
	RenderText(meshList[GEO_BEARHP], BearHPText, Color(1,0.2f,0.2f));
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	//scale, translate, rotate
	modelStack.Translate(camera.position.x, camera.position.y - 5, camera.position.z);
	modelStack.Rotate(-rotateAngle, 0, 1, 0);
	modelStack.Scale(.025f, .025f, .025f);
	RenderMesh(meshList[GEO_MAIN], true);
	modelStack.PopMatrix();
	RenderMeshOnScreen(meshList[GEO_CROSSHAIR], 0, 0, 3, 3);
	RenderScoreBGOnScreen(meshList[GEO_SCOREBG], 0, 0, 1, 1);
	RenderHPBAROnScreen(meshList[GEO_HPBAR], 0, 0, 1, 1);
	RenderTextOnScreen(meshList[GEO_TEXT], "HP:" + std::to_string(int(HP)) + "/" + std::to_string(int(MAXHP)), HPBarCol, 3, 0, 0);
	RenderTextOnScreen(meshList[GEO_SCORE], "Score:"+std::to_string((int)Score), Color(1,1,1), 2, 1, 58);
	// Render VBO here
}

void Assignment2::Exit()
{
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
	// Cleanup VBO here
}
