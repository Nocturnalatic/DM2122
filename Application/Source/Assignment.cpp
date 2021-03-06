#include "Assignment.h"
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
#include <iomanip>


Assignment::Assignment()
{

}

Assignment::~Assignment()
{

}


void Assignment::Init()
{
	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f/3.f, 0.1f, 1000.0f);
	projectionStack.LoadMatrix(projection);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	waveAngle, waveAngle2 = 0;
	anim1, anim2, anim3 = false;
	LSPEED = 10.f;
	m_programID = LoadShaders("Shader//Shading.vertexshader", "Shader//LightSource.fragmentshader");
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
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	glUseProgram(m_programID);
	light[0].type = Light::LIGHT_POINT;
	light[0].position.Set(0, 20, 10);
	light[0].color.Set(1, 1, 0.8);
	light[0].power = 2;
	light[0].kC = 1.2f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;
	light[0].cosCutoff = cos(Math::DegreeToRadian(45));
	light[0].cosInner = cos(Math::DegreeToRadian(30));
	light[0].exponent = 3.f;
	light[0].spotDirection.Set(0.f, 1.f, 0.f);
	light[1].type = Light::LIGHT_DIRECTIONAL;
	light[1].position.Set(0, 10, 0);
	light[1].color.Set(1, 1, 1);
	light[1].power = 0; //Off
	light[1].kC = 1.f;
	light[1].kL = 0.01f;
	light[1].kQ = 0.001f;
	light[1].cosCutoff = cos(Math::DegreeToRadian(45));
	light[1].cosInner = cos(Math::DegreeToRadian(30));
	light[1].exponent = 3.f;
	light[1].spotDirection.Set(0.f, 1.f, 0.f);

	// Make sure you pass uniform parameters after glUseProgram()
	glUniform1i(m_parameters[U_NUMLIGHTS], 2);
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
	// V Light 2 V
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
	glClearColor(0.0f, 0.6f, 0.7f, 0.0f);// Set BG Color

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad",Color(0,0.75,0),1.f);
	meshList[GEO_CIRCLE] = MeshBuilder::GenerateCircle("circle", Color(0.9,1,0), 30, 1);
	meshList[GEO_CYLINDER] = MeshBuilder::GenerateCylinder("ear", Color(0.8, 0.55, 0), 19, 19, .2, 1);
	meshList[GEO_HEMISPHERE] = MeshBuilder::GenerateHemisphere("eyebrows", Color(0.1, 0.1, 0.1), 10, 10, 1);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("Body", Color(0.8,0.55,0), 20, 20, 1);
	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("Light",Color(1,1,0), 10, 10, 1);
	meshList[GEO_BLACK_SPHERE] = MeshBuilder::GenerateSphere("BlackSphere", Color(0, 0, 0), 10, 10, 1);
	meshList[GEO_BARK] = MeshBuilder::GenerateCylinder("Bark", Color(1, 0.5, 0), 10, 10, 10, 1);
	meshList[GEO_LEAF] = MeshBuilder::GenerateSphere("Lead", Color(0, 1, 0), 10, 10, 1);
	meshList[GEO_SPOON] = MeshBuilder::GenerateCylinder("Stick", Color(0.95, 0.77, 0.55), 10, 10, 10, 1);
	meshList[GEO_SCOOP] = MeshBuilder::GenerateHemisphere("Scoop", Color(0.95, 0.77, 0.55), 10, 10, 1);

	camera.Init(Vector3(20,15,15), Vector3(0,0,0), Vector3(0,1,0));
}

void Assignment::Update(double dt)
{

	camera.Update(dt);
	/*twirlAngle += float(360 * dt);
	if (twirlAngle >= 360)
	{
		twirlAngle = 0;
	}*/

	if (Application::IsKeyPressed('C'))
	{
		if (not anim2 && not anim3)
		{
			anim3 = true;
			eatAngle = -90;
			subAngle = 90;
			HeadAngle = -45;
		}
	}

	if (anim3)
	{
		scoopAngle -= float(20 * dt);
		scoopAngle2 -= float(20 * dt);
		if (scoopAngle < -20 && scoopAngle2 < -35)
		{
			anim3 = false;
			eatAngle = 0;
			subAngle = 0;
			HeadAngle = 0;
		}

	}

	if (Application::IsKeyPressed('X'))
	{
		if (not anim2)
		{
			anim2 = true;
			twirlOffset = 0;
			twirlAngle = 0;
			acceleration = 1.2;
			dir = 1;
		}
	}
	
	if (anim2)
	{
		if (armAngle >= -90)
		{
			armAngle -= float(900 * dt);
		}
		if (twirlOffset > maxHeight)
		{
			maxHeight = twirlOffset;
		}
		acceleration -= (9.81 / 600);
		twirlAngle += float(900 * dt);
		twirlOffset += acceleration;
		if (twirlOffset <= 0)
		{
			twirlOffset = 0;
			armAngle = 0;
			twirlAngle = 0;
			std::cout << "Max Height: " << std::fixed << std::setprecision(1) << maxHeight << "m\n";
			anim2 = false;
		}
	}

	if (Application::IsKeyPressed('Z'))
	{
		if (not anim1)
		{
		anim1 = true;
		anim1time = 0;
		dir = 1;
		}
		
	}
	
	if (anim1)
	{
		if (waveAngle >= -90)
		{
			waveAngle += float(-90 * dt*1.5);
		}
		if (waveAngle < -90)
		{
			anim1time += 1;
			waveAngle2 += float((90*dir) * dt);
			if (waveAngle2 >= 15)
			{
				dir = -1;
			}
			if (waveAngle2 <= -15)
			{
				dir = 1;
			}
		}
		if (anim1time > 120)
		{
			anim1 = false;
			waveAngle = 0;
			waveAngle2 = 0;
		}
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
	if (Application::IsKeyPressed('R'))
		light[0].position.Set(0, 20, 10);

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
}

void Assignment::RenderMesh(Mesh* mesh, bool enableLight)
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

	mesh->Render();
}

void Assignment::Render()
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

	modelStack.PushMatrix();
	//to do: transformation code here
	//test:modelStack.Rotate(rotateAngle, 0, 1, 0);
		modelStack.Scale(2, 2, 2);
		RenderMesh(meshList[GEO_SPHERE], true); //Body
		modelStack.PushMatrix();
			//to do: transformation code here
			modelStack.Translate(0, 1.25, 0);
			modelStack.Rotate(HeadAngle, 0, 1, 0);
			modelStack.Scale(0.5, 0.5, 0.5);
			RenderMesh(meshList[GEO_SPHERE], true); //Head
			modelStack.PushMatrix();
			//to do: transformation code here
			modelStack.Translate(-0.75, 0.7, 0);
			modelStack.Rotate(-90, 1, 0, 0);
			modelStack.Rotate(-45, 0, 1, 0);
			modelStack.Scale(.4, .4, .4);
			RenderMesh(meshList[GEO_CYLINDER], true); //Left Ear
			modelStack.PopMatrix();
			modelStack.PushMatrix();
			//to do: transformation code here
			modelStack.Translate(0.75, 0.7, 0);
			modelStack.Rotate(-90, 1, 0, 0);
			modelStack.Rotate(45, 0, 1, 0);
			modelStack.Scale(.4, .4, .4);
			RenderMesh(meshList[GEO_CYLINDER], true); //Right Ear
			modelStack.PopMatrix();
			modelStack.PushMatrix();
			//to do: transformation code here
			modelStack.Translate(.52, 0.7, .44);
			modelStack.Rotate(-90, 1, 0, 0);
			modelStack.Rotate(45,0,1,0);
			modelStack.Scale(0.1, 0.1, 0.1);
			RenderMesh(meshList[GEO_HEMISPHERE], true); //Left Eyebrow
			modelStack.PopMatrix();
			modelStack.PushMatrix();
			//to do: transformation code here
			modelStack.Translate(-.52, 0.7, .44);
			modelStack.Rotate(-90, 1, 0, 0);
			modelStack.Rotate(-45, 0, 1, 0);
			modelStack.Scale(0.1, 0.1, 0.1);
			RenderMesh(meshList[GEO_HEMISPHERE], true); //Right Eyebrow
			modelStack.PopMatrix();
			modelStack.PushMatrix();
			//to do: transformation code here
			modelStack.Translate(-.3, 0.45, 0.8);
			modelStack.Rotate(0, 0, 1, 0);
			modelStack.Scale(0.08, 0.08, 0.08);
			RenderMesh(meshList[GEO_HEMISPHERE], true); //Left Eye
			modelStack.PopMatrix();
			modelStack.PushMatrix();
			//to do: transformation code here
			modelStack.Translate(.3, 0.45, 0.8);
			modelStack.Rotate(0, 0, 1, 0);
			modelStack.Scale(0.08, 0.08, 0.08);
			RenderMesh(meshList[GEO_HEMISPHERE], true); //Right Eye
			modelStack.PopMatrix();
			modelStack.PushMatrix();
			//to do: transformation code here
			modelStack.Translate(0, 0.2, 1);
			modelStack.Rotate(0, 0, 1, 0);
			modelStack.Scale(0.12, 0.12, 0.12);
			RenderMesh(meshList[GEO_BLACK_SPHERE], true); //Nose
			modelStack.PopMatrix();
			modelStack.PushMatrix();
			//to do: transformation code here
			modelStack.Translate(0,-0.270, 0.865);
			modelStack.Rotate(-90, 1, 0, 0);
			modelStack.Rotate(180, 0, 1, 0);
			modelStack.Scale(0.4, 0.1, 0.2);
			RenderMesh(meshList[GEO_HEMISPHERE], true); //Mouth
			modelStack.PopMatrix();
			modelStack.PopMatrix();
			modelStack.PushMatrix();
			//to do: transformation code here
			modelStack.Translate(-.88, 0.5, 0);
			modelStack.Rotate(waveAngle, 1, 0, 0);
			modelStack.Rotate(waveAngle2, 0, 1, 0);
			modelStack.Scale(0.3, 0.3, 0.3);
			RenderMesh(meshList[GEO_SPHERE], true); //Left Joint
			modelStack.PushMatrix();
			//to do: transformation code here
			modelStack.Translate(0, 0, 2.25);
			modelStack.Rotate(90, 1, 0, 0);
			modelStack.Scale(0.45/ 2 / 0.3, 15 / 2 / 0.3, 0.45 / 2 / 0.3);
			RenderMesh(meshList[GEO_CYLINDER], true); //Left Hand
			modelStack.PopMatrix();
			modelStack.PopMatrix();
			modelStack.PushMatrix();
			//to do: transformation code here
			modelStack.Translate(.88, 0.5, 0);
			modelStack.Rotate(armAngle, 1, 0 ,0);
			modelStack.Rotate(scoopAngle,0, 1 ,0);
			modelStack.Rotate(scoopAngle2, 1, 0, 0);
			modelStack.Scale(0.3, 0.3, 0.3);
			RenderMesh(meshList[GEO_SPHERE], true); //Right Joint
			modelStack.PushMatrix();
			//to do: transformation code here
			modelStack.Translate(0, 0, 2.25);
			modelStack.Rotate(90, 1, 0, 0);
			modelStack.Scale(0.45 / 2 / 0.3, 15 / 2 / 0.3, 0.45 / 2 / 0.3);
			RenderMesh(meshList[GEO_CYLINDER], true); //Right Hand
			modelStack.PopMatrix();
			modelStack.PopMatrix();
			modelStack.PushMatrix();
			//to do: transformation code here
			modelStack.Translate(-0.5,-0.5,0.6);
			modelStack.Rotate(90, 1, 0, 0);
			modelStack.Scale(0.35, 10/2, 0.7/2);
			RenderMesh(meshList[GEO_CYLINDER], true); //Left Foot
			modelStack.PopMatrix();
			modelStack.PushMatrix();
			//to do: transformation code here
			modelStack.Translate(.5, -0.5, 0.6);
			modelStack.Rotate(90, 1, 0, 0);
			modelStack.Scale(0.35, 5, 0.35);
			RenderMesh(meshList[GEO_CYLINDER], true); //Right Foot
			modelStack.PopMatrix();
	modelStack.PopMatrix();

	for (int trees = 0; trees < 10; trees++) //generate trees
	{
		modelStack.PushMatrix();
		//to do: transformation code here
		modelStack.Translate(-15, 3, -50 + (10*trees+1));
		modelStack.Scale(1, 1, 1);
		RenderMesh(meshList[GEO_BARK], true); //Tree
		modelStack.PushMatrix();
		//to do: transformation code here
		modelStack.Translate(0, 5, 0);
		modelStack.Scale(3, 3, 3);
		RenderMesh(meshList[GEO_LEAF], true); //Leaf
		modelStack.PopMatrix();
		modelStack.PopMatrix();
	}
	for (int trees = 0; trees < 10; trees++) //generate trees
	{
		modelStack.PushMatrix();
		//to do: transformation code here
		modelStack.Translate(15, 3, -50 + (10 * trees + 1));
		modelStack.Scale(1, 1, 1);
		RenderMesh(meshList[GEO_BARK], true); //Tree
		modelStack.PushMatrix();
		//to do: transformation code here
		modelStack.Translate(0, 5, 0);
		modelStack.Scale(3, 3, 3);
		RenderMesh(meshList[GEO_LEAF], true); //Leaf
		modelStack.PopMatrix();
		modelStack.PopMatrix();
	}

	modelStack.PushMatrix();
	modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
	RenderMesh(meshList[GEO_LIGHTBALL], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	//to do: transformation code here
	modelStack.Translate(0, -2, 0);
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Scale(100, 100, 100);
	RenderMesh(meshList[GEO_QUAD], true);
	modelStack.PopMatrix();

	// Render VBO here
}

void Assignment::Exit()
{
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
	// Cleanup VBO here
}
