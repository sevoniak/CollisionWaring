// test1.cpp : Defines the entry point for the console application.
//  Created by nuss on 2014-07-29.
//  Copyright (c) 2014 Dorn Nussbaum . All rights reserved.
//
//  	This program is for education purposes

// Modified for COMP 4002 Project by
// Sebastian Evoniak
// 100726326

/*
	GNU LICENSE AGREEMENT 

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

	Copyright and License agreement must remain if software is used.

    This program is distributed	AS IS in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

/************************************************************************/
// INCLDES 
#include <stdio.h>
#include <stdlib.h>
#if defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <glew.h>
#include <glut.h>

#include <string>
#include "ShaderLoader.h"
#include "InputClass.h"
#include "camera.h"
#include "Sphere.h"
#include "TexturedQuad.h"
#include "TexturedCube.h"
#include <vector>
using namespace std;

#endif
/************************************************************************/
//FORWARD DELCARATIONS
void initShaders();
float* convMat4fToArr(Matrix4f mat);
void normalKeyDown(unsigned char key, int x, int y);
void specialKeyDown(int key, int x, int y);
void normalKeyUp(unsigned char key, int x, int y);
void specialKeyUp(int key, int x, int y);
void handleInput();
void createCubes();
void updateCubes();


// GLOBALS
int wid;				
int widthWin = 1280;
int heightWin = 720; 
GLuint shaderProg;

float* viewMatrix = 0;
float* projMatrix = 0;

#define NUM_BUILDINGS 20

camera*					cam;
InputClass*				input;
vector<RenderObject*>	objects;
vector<RenderObject*>   skyboxObjs;
Sphere*					sphere;
TexturedCube*			skybox;
TexturedCube*			cubes[NUM_BUILDINGS];
Vector3f				lightPos;
Vector3f				lightBools;
Vector3f				lightAmb, lightDiff, lightSpec;

/*****************************************************************************/

void initApp(void)
{
	input = new InputClass();
	cam = new camera();
	cam->changeAbsPoition(0.0f, 4.0f, 10.0f);

	//define the projection matrix
	if (projMatrix) 
		delete projMatrix;
	float fovY = (float)(50 * M_PI/180.0f);
	float aspect = (float)widthWin/heightWin;
	projMatrix = convMat4fToArr(Matrix4f::symmetricPerspectiveProjectionMatrix(fovY, aspect, 0.01f, 500.0f));
	
	//create objects
	createCubes();

	lightPos = Vector3f(0.0f, 20.0f, 12.0f);
}

void initShaders()
{
//load both vertex and pixel shaders for Phong shading
	ShaderLoader* loader = new ShaderLoader("PhongVertShader.glsl", "PhongFragShader.glsl");

	string vsStr = loader->loadVertexShader();
	string fsStr = loader->loadFragmentShader();

	shaderProg = glCreateProgram();

	//create, compile, and bind vertex shader
	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	const char *vsSrc = vsStr.c_str();
	GLint vlen = vsStr.length();
	glShaderSource(vShader, 1, &vsSrc, &vlen);
	glCompileShader(vShader);
	glAttachShader(shaderProg, vShader);

	//create, compile, and bind pixel shader
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char *fsSrc = fsStr.c_str();
	GLint flen = fsStr.length();
	glShaderSource(fShader, 1, &fsSrc, &flen);
	glCompileShader(fShader);
	glAttachShader(shaderProg, fShader);

	//link shader program
	glLinkProgram(shaderProg);

	glUseProgram(shaderProg);
	GLenum err = glGetError();
	const GLubyte* msg = gluErrorString(err);
	if (err != GLEW_OK)
		printf("Shader error");
}

/* Render function of window */
void renderWin(void)
{
    // Add your code here
	// clear the window and fill it with black colour
    glClearColor(0.15f, 0.15f, 0.15f, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProg);

	//set backface culling options
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	//handle keyboard input
	handleInput();

	//update models
	updateCubes();

	//update the view matrix
	Matrix4f camViewMatrix = cam->getViewMatrix();
	if (viewMatrix) 
		delete viewMatrix;
	viewMatrix = convMat4fToArr(camViewMatrix);

	//send updated transformation matrices to the shaders
	GLuint loc;
	loc = glGetUniformLocation(shaderProg, "viewMat");
	glUniformMatrix4fv(loc, 1, GL_FALSE, viewMatrix);
	loc = glGetUniformLocation(shaderProg, "projMat");
	glUniformMatrix4fv(loc, 1, GL_FALSE, projMatrix);

	Vector4f camSpaceLightPos = camViewMatrix * Vector4f(lightPos, 1.0);
	loc = glGetUniformLocation(shaderProg, "lightPos");
	glUniform4f(loc, camSpaceLightPos.x, camSpaceLightPos.y, camSpaceLightPos.z, 1);

	loc = glGetUniformLocation(shaderProg, "light");
	glUniform1i(loc, 0);

	//render models
	for (unsigned int i = 0; i < skyboxObjs.size(); i++)
		skyboxObjs.at(i)->render(shaderProg);

	glUniform1i(loc, 1);

	for (unsigned int i = 0; i < objects.size(); i++)
		objects.at(i)->render(shaderProg);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glutSwapBuffers();
}


/*****************************************************************************/

/* Called when the corresponsing window is created or resized. */
// the input is the new window size: h (height), w (width)
void resizeWin(int w, int h)
{
    widthWin =  w;
    heightWin =  h;
    glViewport(0, 0, (GLsizei) widthWin, (GLsizei) heightWin);
}

/*****************************************************************************/

int main(int argc, char *argv[])
{
    //* general  initialization which are private to the application
	// This is not related to OpenGL

    glutInit(&argc, argv);
    
    glutInitWindowPosition(100, 100);
	
    //  instruct openGL to use a single display buffer and use 4 values for a colour RGB and Alpha
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    
    // instuct openGL what window size to use
    glutInitWindowSize((int) widthWin, (int) heightWin);
    
	//create window and register callback functions 
    wid = glutCreateWindow("COMP4002 - Project");
    glutReshapeFunc(resizeWin);
    glutDisplayFunc(renderWin);
	glutIdleFunc(renderWin);
	glutKeyboardFunc(normalKeyDown);
	glutSpecialFunc(specialKeyDown);
	glutKeyboardUpFunc(normalKeyUp);
	glutSpecialUpFunc(specialKeyUp);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDepthRange(0.0f, 1.0f);

	// initialize shaders and buffers
	glewInit();
	initApp();
	initShaders();

	// get information about OpenGL
	printf("Rendering GPU: %s\n",  glGetString (GL_RENDERER));
	printf("OpenGL version: %s\n", glGetString (GL_VERSION));
	printf("OpenGL vendor: %s \n", glGetString (GL_VENDOR));
 
    /* start the main loop */
    glutMainLoop();
    
    return 0;
}

void createCubes()
{
	char* skyboxNames[] = {"hit.png", "sky.png", "hit.png", "sky.png", "hit.png", "grass.png", "hit.png", "sky.png", "hit.png", "sky.png", "hit.png", "sky.png"};
	char* names[] = {"hit.png", "build.png", "hit.png", "build.png", "hit.png", "roof.png", "hit.png", "build.png", "hit.png", "build.png", "hit.png", "build.png"};
	char* names2[] = {"hit.png", "build2.png", "hit.png", "build2.png", "hit.png", "roof2.png", "hit.png", "build2.png", "hit.png", "build2.png", "hit.png", "build2.png"};
	char* names3[] = {"hit.png", "build3.png", "hit.png", "build3.png", "hit.png", "roof3.png", "hit.png", "build3.png", "hit.png", "build3.png", "hit.png", "build3.png"};
	char* names4[] = {"hit.png", "build4.png", "hit.png", "build4.png", "hit.png", "roof4.png", "hit.png", "build4.png", "hit.png", "build4.png", "hit.png", "build4.png"};
	vector<RenderObject*> temp;

	skybox = new TexturedCube(30.0f, 16.0f, 30.0f, skyboxNames);
	skyboxObjs = skybox->getModels();
//	objects.insert(objects.end(), temp.begin(), temp.end());
	skybox->flip();
	skybox->translate(0.0f, 7.0f, 0.0f);

	cubes[0] = new TexturedCube(1.0f, 2.0f, 1.0f, names);
	cubes[0]->translate(0.0f, 0.0f, 1.0f);

	cubes[1] = new TexturedCube(1.0f, 2.0f, 1.0f, names2);
	cubes[1]->translate(3.0f, 0.0f, 1.0f);

	cubes[2] = new TexturedCube(1.0f, 2.0f, 1.0f, names3);
	cubes[2]->translate(7.0f, 0.0f, 1.0f);

	cubes[3] = new TexturedCube(1.0f, 2.0f, 1.0f, names4);
	cubes[3]->translate(-3.5f, 0.0f, 1.0f);

	cubes[4] = new TexturedCube(1.0f, 2.0f, 1.0f, names);
	cubes[4]->translate(3.5f, 0.0f, -6.0f);

	cubes[5] = new TexturedCube(1.0f, 2.0f, 1.0f, names2);
	cubes[5]->translate(-3.0f, 0.0f, -6.0f);

	cubes[6] = new TexturedCube(1.0f, 2.0f, 1.0f, names3);
	cubes[6]->translate(6.5f, 0.0f, -6.0f);

	cubes[7] = new TexturedCube(1.0f, 2.0f, 1.0f, names4);
	cubes[7]->translate(0.5f, 0.0f, -6.0f);

	cubes[8] = new TexturedCube(1.0f, 2.0f, 1.0f, names);
	cubes[8]->translate(8.0f, 0.0f, -3.0f);

	cubes[9] = new TexturedCube(1.0f, 2.0f, 1.0f, names3);
	cubes[9]->translate(-3.5f, 0.0f, -2.0f);

	cubes[10] = new TexturedCube(1.0f, 2.0f, 1.0f, names);
	cubes[10]->translate(0.0f, 0.0f, -11.0f);

	cubes[11] = new TexturedCube(1.0f, 2.0f, 1.0f, names2);
	cubes[11]->translate(3.0f, 0.0f, -11.0f);

	cubes[12] = new TexturedCube(1.0f, 2.0f, 1.0f, names4);
	cubes[12]->translate(7.0f, 0.0f, -11.0f);

	cubes[13] = new TexturedCube(1.0f, 2.0f, 1.0f, names3);
	cubes[13]->translate(-3.5f, 0.0f, -11.0f);

	cubes[14] = new TexturedCube(1.0f, 2.0f, 1.0f, names);
	cubes[14]->translate(3.5f, 0.0f, 6.0f);

	cubes[15] = new TexturedCube(1.0f, 2.0f, 1.0f, names2);
	cubes[15]->translate(-3.0f, 0.0f, 6.0f);

	cubes[16] = new TexturedCube(1.0f, 2.0f, 1.0f, names2);
	cubes[16]->translate(6.5f, 0.0f, 6.0f);

	cubes[17] = new TexturedCube(1.0f, 2.0f, 1.0f, names4);
	cubes[17]->translate(0.5f, 0.0f, 6.0f);

	cubes[18] = new TexturedCube(1.0f, 2.0f, 1.0f, names);
	cubes[18]->translate(-7.5f, 0.0f, -4.0f);

	cubes[19] = new TexturedCube(1.0f, 2.0f, 1.0f, names2);
	cubes[19]->translate(11.0f, 0.0f, -5.0f);

	for (int i = 0; i < NUM_BUILDINGS; i++)
	{
		temp = cubes[i]->getModels();
		objects.insert(objects.end(), temp.begin(), temp.end());
	}
}

void updateCubes()
{
	for (int i = 0; i < NUM_BUILDINGS; i++)
		cubes[i]->getWorldMatrix();
	skybox->getWorldMatrix();
}

//transpose and convert a Matrix4f into an array of floats.
float* convMat4fToArr(Matrix4f mat)
{
	float * fmat = new float[16];
	fmat[0] = mat.vm[0].x;
	fmat[1] = mat.vm[1].x;
	fmat[2] = mat.vm[2].x;
	fmat[3] = mat.vm[3].x;
	fmat[4] = mat.vm[0].y;
	fmat[5] = mat.vm[1].y;
	fmat[6] = mat.vm[2].y;
	fmat[7] = mat.vm[3].y;
	fmat[8] = mat.vm[0].z;
	fmat[9] = mat.vm[1].z;
	fmat[10] = mat.vm[2].z;
	fmat[11] = mat.vm[3].z;
	fmat[12] = mat.vm[0].w;
	fmat[13] = mat.vm[1].w;
	fmat[14] = mat.vm[2].w;
	fmat[15] = mat.vm[3].w;
	return fmat;
}

//handle keyboard input
void normalKeyDown(unsigned char key, int x, int y)
{
	input->keyDown(key);
}

void specialKeyDown(int key, int x, int y)
{
	input->keyDown(key+OFFSET);
}

void normalKeyUp(unsigned char key, int x, int y)
{
	input->keyUp(key);
}

void specialKeyUp(int key, int x, int y)
{
	input->keyUp(key+OFFSET);
}

void handleInput()
{
	// camera control keys - translations
	if(input->isKeyDown('a'))
		cam->strafe(-0.002f);
	if(input->isKeyDown('d'))
		cam->strafe(0.002f);
	if(input->isKeyDown('w'))
		cam->moveForward(0.002f);
	if(input->isKeyDown('s'))
		cam->moveForward(-0.002f);
	if(input->isKeyDown('r'))
		cam->crane(0.002f);
	if(input->isKeyDown('f'))
		cam->crane(-0.002f);

	// camera control keys - rotations
	if(input->isKeyDown('q'))
		cam->roll(-0.001f);
	if(input->isKeyDown('e'))
		cam->roll(0.001f);
	if(input->isKeyDown(GLUT_KEY_UP+OFFSET))
		cam->pitch(0.0006f);
	if(input->isKeyDown(GLUT_KEY_DOWN+OFFSET))
		cam->pitch(-0.0006f);
	if(input->isKeyDown(GLUT_KEY_RIGHT+OFFSET))
		cam->yaw(-0.0006f);
	if(input->isKeyDown(GLUT_KEY_LEFT+OFFSET))
		cam->yaw(0.0006f);
}