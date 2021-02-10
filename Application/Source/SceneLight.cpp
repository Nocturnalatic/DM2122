#include "SceneLight.h"
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


SceneLight::SceneLight()
{

}

SceneLight::~SceneLight()
{

}


void SceneLight::Init()
{
	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f/3.f, 0.1f, 1000.0f);
	projectionStack.LoadMatrix(projection);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	rotateAngle = 10;
	LSPEED = 10.f;
	m_programID = LoadShaders("Shader//Shading.vertexshader", "Shader//Shading.fragmentshader");
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
	glUseProgram(m_programID);
	light[0].position.Set(0, 20, 0);
	light[0].color.Set(1, 1, 1);
	light[0].power = 1;
	light[0].kC = 1.f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;
	// Make sure you pass uniform parameters after glUseProgram()
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);// Set BG Color

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad",Color(0,1,1),1.f);
	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", Color(1,0,1), 1.f);
	meshList[GEO_CIRCLE] = MeshBuilder::GenerateCircle("circle", Color(0.9,1,0), 30, 1);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("Sun", Color(.1,.1,.1), 20, 20, 1);
	meshList[GEO_SPHERE_2] = MeshBuilder::GenerateSphere("Ball2", Color(0, 1, 1), 20, 20, 1);
	meshList[GEO_SPHERE_3] = MeshBuilder::GenerateSphere("Ball3", Color(0, 1, 0), 20, 20, 1);
	meshList[GEO_SPHERE_4] = MeshBuilder::GenerateSphere("Ball4", Color(1, 1, 0), 20, 20, 1);
	meshList[GEO_SPHERE_5] = MeshBuilder::GenerateSphere("Ball5", Color(1, 0, 1), 20, 20, 1);
	meshList[GEO_SPHERE_6] = MeshBuilder::GenerateSphere("Ball6", Color(0.5, 0, 1), 20, 20, 1);
	meshList[GEO_SPHERE_7] = MeshBuilder::GenerateSphere("Ball7", Color(1, 0.5, 0), 20, 20, 1);
	meshList[GEO_SPHERE_8] = MeshBuilder::GenerateSphere("Ball8", Color(0, 0.5, 1), 20, 20, 1);
	meshList[GEO_SPHERE_9] = MeshBuilder::GenerateSphere("Ball9", Color(1, 0, 0), 20, 20, 1);
	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("Light",Color(1,1,1), 10, 10, 1);
	meshList[GEO_SPHERE]->material.kAmbient.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_SPHERE]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_SPHERE]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_SPHERE]->material.kShininess = 1.f;
	meshList[GEO_SPHERE_2]->material.kAmbient.Set(0.5f, 0.5f, 0.5f); meshList[GEO_SPHERE_2]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_SPHERE_2]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);meshList[GEO_SPHERE_2]->material.kShininess = 1.f;
	meshList[GEO_SPHERE_3]->material.kAmbient.Set(0.5f, 0.5f, 0.5f); meshList[GEO_SPHERE_3]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_SPHERE_3]->material.kSpecular.Set(0.3f, 0.3f, 0.3f); meshList[GEO_SPHERE_3]->material.kShininess = 1.f;
	meshList[GEO_SPHERE_4]->material.kAmbient.Set(0.5f, 0.5f, 0.5f); meshList[GEO_SPHERE_4]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_SPHERE_4]->material.kSpecular.Set(0.3f, 0.3f, 0.3f); meshList[GEO_SPHERE_4]->material.kShininess = 1.f;
	meshList[GEO_SPHERE_5]->material.kAmbient.Set(0.5f, 0.5f, 0.5f); meshList[GEO_SPHERE_5]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_SPHERE_5]->material.kSpecular.Set(0.3f, 0.3f, 0.3f); meshList[GEO_SPHERE_5]->material.kShininess = 1.f;
	meshList[GEO_SPHERE_6]->material.kAmbient.Set(0.5f, 0.5f, 0.5f); meshList[GEO_SPHERE_6]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_SPHERE_6]->material.kSpecular.Set(0.3f, 0.3f, 0.3f); meshList[GEO_SPHERE_6]->material.kShininess = 1.f;
	meshList[GEO_SPHERE_7]->material.kAmbient.Set(0.5f, 0.5f, 0.5f); meshList[GEO_SPHERE_7]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_SPHERE_7]->material.kSpecular.Set(0.3f, 0.3f, 0.3f); meshList[GEO_SPHERE_7]->material.kShininess = 1.f;
	meshList[GEO_SPHERE_8]->material.kAmbient.Set(0.5f, 0.5f, 0.5f); meshList[GEO_SPHERE_8]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_SPHERE_8]->material.kSpecular.Set(0.3f, 0.3f, 0.3f); meshList[GEO_SPHERE_8]->material.kShininess = 1.f;
	meshList[GEO_SPHERE_9]->material.kAmbient.Set(0.5f, 0.5f, 0.5f); meshList[GEO_SPHERE_9]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_SPHERE_9]->material.kSpecular.Set(0.3f, 0.3f, 0.3f); meshList[GEO_SPHERE_9]->material.kShininess = 1.f;

	camera.Init(Vector3(20,15,15), Vector3(0,0,0), Vector3(0,1,0));
}

void SceneLight::Update(double dt)
{
	camera.Update(dt);
	rotateAngle += (float)(15 * dt);
	rotateAngle2 += (float)(28 * dt);
	rotateAngle3 += (float)(10 * dt);
	if (rotateAngle > 360)
	{
		rotateAngle = 0;
	}
	if (rotateAngle2 > 360)
	{
		rotateAngle2 = 0;
	}
	if (rotateAngle3 > 360)
	{
		rotateAngle3 = 0;
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
}

void SceneLight::RenderMesh(Mesh* mesh, bool enableLight)
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
	mesh->Render();
}

void SceneLight::Render()
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

	modelStack.PushMatrix();
	//to do: transformation code here
	modelStack.Scale(2,2,2);
	RenderMesh(meshList[GEO_SPHERE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Scale(2, 2, 2);
	modelStack.Translate(2,0,0);
	RenderMesh(meshList[GEO_SPHERE_2], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Scale(2, 2, 2);
	modelStack.Translate(2, 0, 2);
	RenderMesh(meshList[GEO_SPHERE_3], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Scale(2, 2, 2);
	modelStack.Translate(-2, 0, 2);
	RenderMesh(meshList[GEO_SPHERE_4], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Scale(2, 2, 2);
	modelStack.Translate(-2, 0, 0);
	RenderMesh(meshList[GEO_SPHERE_5], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Scale(2, 2, 2);
	modelStack.Translate(2, 0, -2);
	RenderMesh(meshList[GEO_SPHERE_6], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Scale(2, 2, 2);
	modelStack.Translate(-2, 0, -2);
	RenderMesh(meshList[GEO_SPHERE_7], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Scale(2, 2, 2);
	modelStack.Translate(0, 0, -2);
	RenderMesh(meshList[GEO_SPHERE_8], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Scale(2, 2, 2);
	modelStack.Translate(0, 0, 2);
	RenderMesh(meshList[GEO_SPHERE_9], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
	RenderMesh(meshList[GEO_LIGHTBALL], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	//to do: transformation code here
	modelStack.Translate(0, -4, 0);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(100, 100, 100);
	RenderMesh(meshList[GEO_QUAD], true);
	modelStack.PopMatrix();

	// Render VBO here
}

void SceneLight::Exit()
{
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
	// Cleanup VBO here
}
