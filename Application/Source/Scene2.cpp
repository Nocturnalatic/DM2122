#include "Scene2.h"
#include "GL\glew.h"
#include "Mtx44.h"
#include <time.h>
#include <random>
#include <iostream>
#include "shader.hpp"


Scene2::Scene2()
{

}

Scene2::~Scene2()
{

}


void Scene2::Init()
{
	srand(time(NULL));
	speed = 0;
	translateY = 10;
	rotateAngle = 45;
	translateX = 0.1;
	scaleAll = 1.1;

	m_programID = LoadShaders("Shader//TransformVertexShader.vertexshader",
	"Shader//SimpleFragmentShader.fragmentshader");
	glUseProgram(m_programID);
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);// Set BG Color

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	glGenBuffers(NUM_GEOMETRY, &m_vertexBuffer[0]);
	glGenBuffers(NUM_GEOMETRY, &m_colorBuffer[0]);

	static const GLfloat vertex_buffer_data[] = {
		-0.5f, 0.0f, 0.0f, 
		0.5f, 0.0f, 0.0f,
		0.0f, 0.8f, 0.0f,
		-0.5f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.0f,
		0.0f, -0.8f, 0.0f,
	};

	static const GLfloat vertex_buffer_data_2[] = {
		-0.5f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.0f,
		0.0f, 0.8f, 0.0f,
		-0.5f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.0f,
		0.0f, -0.8f, 0.0f,
	};

	static const GLfloat vertex_buffer_data_3[] = {
		-0.5f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.0f,
		0.0f, 0.8f, 0.0f,
		-0.5f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.0f,
		0.0f, -0.8f, 0.0f,
	};

	static const GLfloat color_buffer_data_blackd[] = {
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
	};

	static const GLfloat color_buffer_data_yellow[] = {
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
	};

	static const GLfloat color_buffer_data_white[] = {
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
	};

	static const GLfloat color_buffer_data_black[] = {
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
	};

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_OBJ_1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_OBJ_1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data_black), color_buffer_data_black, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_OBJ_2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_OBJ_2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data_black), color_buffer_data_black, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_OBJ_3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_OBJ_3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data_black), color_buffer_data_black, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_OBJ_4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_OBJ_4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data_black), color_buffer_data_black, GL_STATIC_DRAW);

}

void Scene2::Update(double dt)
{
	speed = 5 + (rand() % 15);
	std::cout << speed << std::endl;
	rotateAngle += (float)(100 * dt); //rate
	scaleAll += (float)(5 * dt);
	translateX -= (float)(2 * dt);
	translateX1 -= (float)(2 * dt);
	translateX2 -= (float)(2 * dt);
	translateY -= (float)(speed * dt);

	if (translateY > 10)
	{
		translateY = -10;
	}
	else if (translateY < -10)
	{
		translateX = -10 + (rand() % 10);
		translateX1 = -10 + (rand() % 10);
		translateX2 = -10 + (rand() % 10);
		translateY = 10;
	}

	if (translateX > 10)
	{
		translateX = -10;
	}
	else if (translateX < -10)
	{
		translateX = 10;
	}

	if (translateX1 > 10)
	{
		translateX1 = -10;
	}
	else if (translateX1 < -10)
	{
		translateX1 = 10;
	}

	if (translateX2 > 10)
	{
		translateX2 = -10;
	}
	else if (translateX2 < -10)
	{
		translateX2 = 10;
	}

}

void Scene2::Render()
{
	// Init //
	Mtx44 translate, rotate, scale;
	Mtx44 model;
	Mtx44 view;
	Mtx44 projection;
	Mtx44 MVP;

	translate.SetToIdentity();
	rotate.SetToIdentity();
	scale.SetToIdentity();
	model.SetToIdentity();
	view.SetToIdentity();
	projection.SetToOrtho(-10,10,-10,10,-10,10);
	// Transformation //
	/*scale.SetToScale(1,1,1);
	rotate.SetToRotation(0,0,0,1);
	translate.SetToTranslation(0,0,0);
	model = translate * rotate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);*/

	glClear(GL_COLOR_BUFFER_BIT);
	glEnableVertexAttribArray(0); 
	glEnableVertexAttribArray(1);

	scale.SetToScale(1, 1, 1);
	rotate.SetToRotation(rotateAngle, 0, 0, 1);
	translate.SetToTranslation(translateX, translateY, 0);
	model = translate * rotate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_OBJ_1]); 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_OBJ_1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// Draw the triangle
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// --------------------- //

	model.SetToIdentity();
	scale.SetToScale(1, 1, 1);
	rotate.SetToRotation(rotateAngle, 0, 0, 1);
	translate.SetToTranslation(translateX2, translateY, 0);
	model = translate * rotate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_OBJ_2]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_OBJ_2]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// Draw the triangle
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// --------------------- //

	model.SetToIdentity();
	scale.SetToScale(1, 1, 1);
	rotate.SetToRotation(rotateAngle, 0, 0, 1);
	translate.SetToTranslation(translateX1, translateY, 0);
	model = translate * rotate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_OBJ_3]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_OBJ_3]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// Draw the triangle
	glDrawArrays(GL_TRIANGLES, 0, 6);


	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	// Render VBO here
}

void Scene2::Exit()
{
	glDeleteBuffers(NUM_GEOMETRY, &m_vertexBuffer[0]);
	glDeleteBuffers(NUM_GEOMETRY, &m_colorBuffer[0]);
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
	// Cleanup VBO here
}
