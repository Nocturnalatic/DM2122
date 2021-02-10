#include "Scene5.h"
#include "GL\glew.h"
#include "Mtx44.h"
#include <time.h>
#include <random>
#include <iostream>
#include "shader.hpp"
#include "Application.h"
#include "MeshBuilder.h"
#include "Vertex.h"


Scene5::Scene5()
{

}

Scene5::~Scene5()
{

}


void Scene5::Init()
{
	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f/3.f, 0.1f, 1000.0f);
	projectionStack.LoadMatrix(projection);
	glEnable(GL_CULL_FACE);
	rotateAngle = 10;
	m_programID = LoadShaders("Shader//TransformVertexShader.vertexshader",
	"Shader//SimpleFragmentShader.fragmentshader");
	glUseProgram(m_programID);
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);// Set BG Color

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad",Color(0,1,0),1.f);
	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", Color(1,0,1), 1.f);
	meshList[GEO_CIRCLE] = MeshBuilder::GenerateCircle("circle", Color(0.9,1,0), 30, 1);
	meshList[GEO_RING_7] = MeshBuilder::GenerateCircle("Ring", Color(0, 0, 0), 30, 1);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("Sun", Color(1,1,0), 30, 30, 1);
	meshList[GEO_SPHERE_2] = MeshBuilder::GenerateSphere("Mercury", Color(0.2, 0.2, 0.2), 30, 30, 1);
	meshList[GEO_SPHERE_3] = MeshBuilder::GenerateSphere("Venus", Color(1,0.5,0), 30, 30, 1);
	meshList[GEO_SPHERE_4] = MeshBuilder::GenerateSphere("Earth", Color(0,1,1), 30, 30, 1);
	meshList[GEO_SPHERE_5] = MeshBuilder::GenerateSphere("Mars", Color(1, 0, 0), 30, 30, 1);
	meshList[GEO_SPHERE_6] = MeshBuilder::GenerateSphere("Jupiter", Color(0.85, 1, 0), 30, 30, 1);
	meshList[GEO_SPHERE_7] = MeshBuilder::GenerateSphere("Saturn", Color(0.9, 1, 0), 30, 30, 1);
	meshList[GEO_SPHERE_8] = MeshBuilder::GenerateSphere("Uranus", Color(0, 0.7, 1), 30, 30, 1);
	meshList[GEO_SPHERE_9] = MeshBuilder::GenerateSphere("Neptune", Color(0, 0, 1), 30, 30, 1);
	//meshList[GEO_RING_NEG_7] = MeshBuilder::GenerateCircle("Inv-Ring", Color(0, 0, 0), 36, 2);

	camera.Init(Vector3(20,15,15), Vector3(0,0,0), Vector3(0,1,0));
}

void Scene5::Update(double dt)
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
}

void Scene5::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	// Init //
	Mtx44 MVP;

	viewStack.LoadIdentity();
	viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z, camera.target.x, camera.target.y, camera.target.z, camera.up.x, camera.up.y, camera.up.z);
	modelStack.LoadIdentity();
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	meshList[GEO_AXES]->Render();

	modelStack.PushMatrix();
	modelStack.Scale(3, 3, 3);
	modelStack.Rotate(0, 0, 1, 0);
	modelStack.Translate(0, 0, 0);
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	meshList[GEO_SPHERE]->Render(); //Sun
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Rotate(rotateAngle,0,1,0);
	modelStack.Translate(-8, 0, 0);
	modelStack.Rotate(0, 0, 1, 0);
	modelStack.Scale(0.5, 0.5, 0.5);
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	meshList[GEO_SPHERE_2]->Render(); //Mercury
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Rotate(rotateAngle2, 0, 1, 0);
	modelStack.Translate(-13, 0, 0);
	modelStack.Rotate(rotateAngle2, 0, 1, 0);
	modelStack.Scale(0.7, 0.7, 0.7);
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	meshList[GEO_SPHERE_3]->Render(); //Venus
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Rotate(rotateAngle, 0, 1, 0);
	modelStack.Translate(-20, 0, 0);
	modelStack.Rotate(rotateAngle, 0, 1, 0);
	modelStack.Scale(1.3, 1.3, 1.3);
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	meshList[GEO_SPHERE_4]->Render(); //Earth
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Rotate(rotateAngle3, 0, 1, 0);
	modelStack.Translate(-28, 0, 0);
	modelStack.Rotate(rotateAngle3, 0, 1, 0);
	modelStack.Scale(0.9, 0.9, 0.9);
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	meshList[GEO_SPHERE_5]->Render(); //Mars
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Rotate(rotateAngle, 0, 1, 0);
	modelStack.Translate(-40, 0, 0);
	modelStack.Rotate(rotateAngle, 0, 1, 0);
	modelStack.Scale(2.1, 2.1, 2.1);
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	meshList[GEO_SPHERE_6]->Render(); //Jupiter
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Rotate(rotateAngle2, 0, 1, 0);
	modelStack.Translate(-48, 0, 0);
	modelStack.Rotate(rotateAngle2, 0, 1, 0);
	modelStack.Scale(1.7, 1.7, 1.7);
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	meshList[GEO_SPHERE_7]->Render(); //Saturn

	modelStack.PushMatrix();
	modelStack.Scale(2, 2, 2);
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	meshList[GEO_CIRCLE]->Render(); //Saturn Ring
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Scale(1.5, 1.5, 1.5);
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	meshList[GEO_RING_7]->Render(); //Saturn Ring
	modelStack.PopMatrix();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Rotate(rotateAngle3, 0, 1, 0);
	modelStack.Translate(-58, 0, 0);
	modelStack.Rotate(rotateAngle3, 0, 1, 0);
	modelStack.Scale(1.6, 1.6, 1.6);
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	meshList[GEO_SPHERE_8]->Render(); //Uranus
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Rotate(rotateAngle, 0, 1, 0);
	modelStack.Translate(-65, 0, 0);
	modelStack.Rotate(rotateAngle, 0, 1, 0);
	modelStack.Scale(1.4, 1.4, 1.4);
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	meshList[GEO_SPHERE_9]->Render(); //Neptune
	modelStack.PopMatrix();
	// Render VBO here
}

void Scene5::Exit()
{
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
	// Cleanup VBO here
}
