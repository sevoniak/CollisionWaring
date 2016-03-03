#include "TexturedQuad.h"


TexturedQuad::TexturedQuad()
{
	xLength = 1.0f;
	yLength = 1.0f;
	filename = 0;

	initVertices();
	initBuffers();
}

TexturedQuad::TexturedQuad(float xLengthIn, float yLengthIn, char* filenameIn, char* filename2In)
{
	xLength = xLengthIn;
	yLength = yLengthIn;
	filename = filenameIn;
	filename2 = filename2In;

	initVertices();
	loadImage();
	initBuffers();
}

TexturedQuad::~TexturedQuad(void)
{
}

void TexturedQuad::shutdown()
{
}

void TexturedQuad::initVertices()
{
	numVert = 4;
	numInds = 6;

	//Indicies
	indices = new unsigned int[numInds];

	//Front
	indices[0] = 0; indices[1] = 2; indices[2] = 1;
	indices[3] = 0; indices[4] = 3; indices[5] = 2;

	vertices = new vertex[numVert];
	
	//Vertices
	vertices[0].pos = Vector3f(-xLength/2,  yLength/2, 0); 
	vertices[0].tex = Vector2f(0.0f, 0.0f);
	vertices[0].norm = Vector3f(0.0f, 0.0f, 1.0f);

	vertices[1].pos = Vector3f( xLength/2,  yLength/2, 0); 
	vertices[1].tex = Vector2f(1.0f, 0.0f);
	vertices[1].norm = Vector3f(0.0f, 0.0f, 1.0f);

	vertices[2].pos = Vector3f( xLength/2, -yLength/2, 0); 
	vertices[2].tex = Vector2f(1.0f, 1.0f);
	vertices[2].norm = Vector3f(0.0f, 0.0f, 1.0f);

	vertices[3].pos = Vector3f(-xLength/2, -yLength/2, 0); 
	vertices[3].tex = Vector2f(0.0f, 1.0f);
	vertices[3].norm = Vector3f(0.0f, 0.0f, 1.0f);
}

void TexturedQuad::advance()
{
	rotateY(0.01f);
}	

void TexturedQuad::loadImage()
{
	int width=1, height=1;
    unsigned char* image;
	int channels;

	// Create texture object
	glGenTextures(1, &texBuf);
	glGenTextures(1, &texBuf2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texBuf);
	image = SOIL_load_image(filename, &width, &height, &channels, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texBuf2);
	image = SOIL_load_image(filename2, &width, &height, &channels, SOIL_LOAD_RGBA);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
		
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP);
}