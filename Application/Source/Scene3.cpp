#include "Scene3.h"
#include "GL\glew.h"
#include "Mtx44.h"
#include <time.h>
#include <random>
#include <iostream>
#include "shader.hpp"


Scene3::Scene3()
{

}

Scene3::~Scene3()
{

}


void Scene3::Init()
{
	srand(time(NULL));
	speed = 0;
	translateY = 10;
	rotateAngle = 45;
	translateX = 0.1;
	scaleAll = 1.1;
	snowflakeFall = 10;

	m_programID = LoadShaders("Shader//TransformVertexShader.vertexshader",
	"Shader//SimpleFragmentShader.fragmentshader");
	glUseProgram(m_programID);
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	glClearColor(0.0f, 0.0f, 0.1f, 0.0f);// Set BG Color

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	glGenBuffers(NUM_GEOMETRY, &m_vertexBuffer[0]);
	glGenBuffers(NUM_GEOMETRY, &m_colorBuffer[0]);

	static const GLfloat vertex_buffer_data_Ctree_1a[] = {
		-0.2f, 0.0f, 0.0f, 
		0.2f, 0.0f, 0.0f,
		0.0f, 0.2f, 0.0f,
	};

	static const GLfloat vbd_grass_1a[] = {
		-10.0f, -10.0f, 0.0f,
		10.0f, 0.0f, 0.0f,
		-10.0f, 0.0f, 0.0f,
	};

	static const GLfloat vbd_grass_1b[] = {
		10.0f, -10.0f, 0.0f,
		10.0f, 0.0f, 0.0f,
		-10.0f, -10.0f, 0.0f,
	};

	static const GLfloat vbd_tree_bark_a[] = {
		-0.1f, 0.0f, 0.0f,
		-0.1f, -0.1f, 0.0f,
		0.0f, 0.0f, 0.0f,
	};

	static const GLfloat vbd_tree_bark_b[] = {
		0.0f, 0.0f, 0.0f,
		-0.1f, -0.1f, 0.0f,
		0.0f, -0.1f, 0.0f,
	};

	// ---------------------------- //

	static const GLfloat color_buffer_data_black[] = {
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

	static const GLfloat color_buffer_data_grass[] = {
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
	};

	static const GLfloat color_buffer_data_dark_grass[] = {
		0.0f, 0.5f, 0.0f,
		0.0f, 0.5f, 0.0f,
		0.0f, 0.5f, 0.0f,
	};

	static const GLfloat color_buffer_data_blue[] = {
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
	};

	static const GLfloat color_buffer_data_brown[] = {
		0.5f, 0.25f, 0.0f,
		0.5f, 0.25f, 0.0f,
		0.5f, 0.25f, 0.0f,
	};

	static const GLfloat color_buffer_data_red[] = {
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
	};

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TREE_1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data_Ctree_1a), vertex_buffer_data_Ctree_1a, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TREE_1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data_dark_grass), color_buffer_data_dark_grass, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TREE_2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data_Ctree_1a), vertex_buffer_data_Ctree_1a, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TREE_2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data_dark_grass), color_buffer_data_dark_grass, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_GRASS_1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vbd_grass_1a), vbd_grass_1a, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_GRASS_1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data_grass), color_buffer_data_grass, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_GRASS_2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vbd_grass_1b), vbd_grass_1b, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_GRASS_2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data_grass), color_buffer_data_grass, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_BACKGROUND]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data_Ctree_1a), vertex_buffer_data_Ctree_1a, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_BACKGROUND]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data_red), color_buffer_data_red, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TREE_BARK_1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vbd_tree_bark_a), vbd_tree_bark_a, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TREE_BARK_1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data_brown), color_buffer_data_brown, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TREE_BARK_2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vbd_tree_bark_b), vbd_tree_bark_b, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TREE_BARK_2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data_brown), color_buffer_data_brown, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_SNOWFLAKE_1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vbd_tree_bark_a), vbd_tree_bark_a, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_SNOWFLAKE_1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data_white), color_buffer_data_white, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_SNOWFLAKE_2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vbd_tree_bark_b), vbd_tree_bark_b, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_SNOWFLAKE_2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data_white), color_buffer_data_white, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_SNOWFLAKE_1a]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vbd_tree_bark_a), vbd_tree_bark_a, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_SNOWFLAKE_1a]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data_white), color_buffer_data_white, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_SNOWFLAKE_1b]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vbd_tree_bark_b), vbd_tree_bark_b, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_SNOWFLAKE_1b]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data_white), color_buffer_data_white, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_SNOWFLAKE_2a]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vbd_tree_bark_a), vbd_tree_bark_a, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_SNOWFLAKE_2a]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data_white), color_buffer_data_white, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_SNOWFLAKE_2b]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vbd_tree_bark_b), vbd_tree_bark_b, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_SNOWFLAKE_2b]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data_white), color_buffer_data_white, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_SNOWFLAKE_3a]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vbd_tree_bark_a), vbd_tree_bark_a, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_SNOWFLAKE_3a]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data_white), color_buffer_data_white, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_SNOWFLAKE_3b]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vbd_tree_bark_b), vbd_tree_bark_b, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_SNOWFLAKE_3b]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data_white), color_buffer_data_white, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_MOON_1a]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vbd_grass_1a), vbd_grass_1a, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_MOON_1a]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data_white), color_buffer_data_white, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_MOON_1b]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vbd_grass_1b), vbd_grass_1b, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_MOON_1b]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data_white), color_buffer_data_white, GL_STATIC_DRAW);
}

void Scene3::Update(double dt)
{
	snowflakeFall -= (float)(5 * dt);
	translateX -= (float)(5 * dt);
	scaleAll -= (float)(1 * dt);

	if (scaleAll < 0)
	{
		scaleAll = 3;
		snowflakeFall = 10;
	}

	if (translateX < -10)
	{
		translateX = 10;
	}
}

void Scene3::Render()
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
	rotate.SetToRotation(0, 0, 0, 1);
	translate.SetToTranslation(0, 0, 0);
	model = translate * rotate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_GRASS_1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_GRASS_1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// Draw the triangle
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// --------------------- //

	model.SetToIdentity();
	scale.SetToScale(1, 1, 1);
	rotate.SetToRotation(0, 0, 0, 1);
	translate.SetToTranslation(0, 0, 0);
	model = translate * rotate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_GRASS_2]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_GRASS_2]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// Draw the triangle
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// --------------------- //

	model.SetToIdentity();
	scale.SetToScale(25, 25, 25);
	rotate.SetToRotation(0, 0, 0, 1);
	translate.SetToTranslation(0, 0, 0);
	model = translate * rotate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_BACKGROUND]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_BACKGROUND]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// Draw the triangle
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// --------------------- //

	model.SetToIdentity();
	scale.SetToScale(0.1, 0.2, 0.1);
	rotate.SetToRotation(0, 0, 0, 1);
	translate.SetToTranslation(7, 7, 0);
	model = translate * rotate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_MOON_1a]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_MOON_1a]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// Draw the triangle
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// --------------------- //

	model.SetToIdentity();
	scale.SetToScale(0.1, 0.2, 0.1);
	rotate.SetToRotation(0, 0, 0, 1);
	translate.SetToTranslation(7, 7, 7);
	model = translate * rotate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_MOON_1b]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_MOON_1b]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// Draw the triangle
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// --------------------- //

	model.SetToIdentity();
	scale.SetToScale(12, 12, 12);
	rotate.SetToRotation(0, 0, 0, 1);
	translate.SetToTranslation(0, 1, 0);
	model = translate * rotate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TREE_1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TREE_1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// Draw the triangle
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// --------------------- //

	model.SetToIdentity();
	scale.SetToScale(16, 16, 16);
	rotate.SetToRotation(0, 0, 0, 1);
	translate.SetToTranslation(0, -0.75-1, 0);
	model = translate * rotate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TREE_1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TREE_1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// Draw the triangle
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// --------------------- //

	model.SetToIdentity();
	scale.SetToScale(10, 16, 10);
	rotate.SetToRotation(0, 0, 0, 1);
	translate.SetToTranslation(0.5, -0.75-1, 0);
	model = translate * rotate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TREE_BARK_1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TREE_BARK_1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// Draw the triangle
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// --------------------- //

	model.SetToIdentity();
	scale.SetToScale(10, 16, 10);
	rotate.SetToRotation(0, 0, 0, 1);
	translate.SetToTranslation(0.5, -0.75-1, 0);
	model = translate * rotate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TREE_BARK_2]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TREE_BARK_2]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// Draw the triangle
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// --------------------- //

	model.SetToIdentity();
	scale.SetToScale(scaleAll, scaleAll, scaleAll);
	rotate.SetToRotation(0, 0, 0, 1);
	translate.SetToTranslation(0.25 - translateX, snowflakeFall, 0);
	model = translate * rotate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_SNOWFLAKE_1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_SNOWFLAKE_1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// Draw the triangle
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// --------------------- //

	model.SetToIdentity();
	scale.SetToScale(scaleAll, scaleAll, scaleAll);
	rotate.SetToRotation(0, 0, 0, 1);
	translate.SetToTranslation(0.25 - translateX, snowflakeFall, 0);
	model = translate * rotate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_SNOWFLAKE_2]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_SNOWFLAKE_2]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// Draw the triangle
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// --------------------- //

	model.SetToIdentity();
	scale.SetToScale(scaleAll, scaleAll, scaleAll);
	rotate.SetToRotation(0, 0, 0, 1);
	translate.SetToTranslation(-4 - translateX, snowflakeFall, 0);
	model = translate * rotate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_SNOWFLAKE_1a]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_SNOWFLAKE_1a]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// Draw the triangle
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// --------------------- //

	model.SetToIdentity();
	scale.SetToScale(scaleAll, scaleAll, scaleAll);
	rotate.SetToRotation(0, 0, 0, 1);
	translate.SetToTranslation(-4 - translateX,snowflakeFall, 0);
	model = translate * rotate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_SNOWFLAKE_1b]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_SNOWFLAKE_1b]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// Draw the triangle
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// --------------------- //

	model.SetToIdentity();
	scale.SetToScale(scaleAll, scaleAll, scaleAll);
	rotate.SetToRotation(0, 0, 0, 1);
	translate.SetToTranslation(3 - translateX, snowflakeFall, 0);
	model = translate * rotate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_SNOWFLAKE_2a]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_SNOWFLAKE_2a]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// Draw the triangle
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// --------------------- //

	model.SetToIdentity();
	scale.SetToScale(scaleAll, scaleAll, scaleAll);
	rotate.SetToRotation(0, 0, 0, 1);
	translate.SetToTranslation(3 - translateX, snowflakeFall, 0);
	model = translate * rotate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_SNOWFLAKE_2b]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_SNOWFLAKE_2b]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// Draw the triangle
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// --------------------- //

	model.SetToIdentity();
	scale.SetToScale(scaleAll, scaleAll, scaleAll);
	rotate.SetToRotation(0, 0, 0, 1);
	translate.SetToTranslation(-6 - translateX, snowflakeFall, 0);
	model = translate * rotate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_SNOWFLAKE_3a]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_SNOWFLAKE_3a]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// Draw the triangle
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// --------------------- //

	model.SetToIdentity();
	scale.SetToScale(scaleAll, scaleAll, scaleAll);
	rotate.SetToRotation(0, 0, 0, 1);
	translate.SetToTranslation(-6 - translateX, snowflakeFall, 0);
	model = translate * rotate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_SNOWFLAKE_3b]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_SNOWFLAKE_3b]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// Draw the triangle
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	// Render VBO here
}

void Scene3::Exit()
{
	glDeleteBuffers(NUM_GEOMETRY, &m_vertexBuffer[0]);
	glDeleteBuffers(NUM_GEOMETRY, &m_colorBuffer[0]);
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
	// Cleanup VBO here
}
