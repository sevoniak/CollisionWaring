#include "Sphere.h"

Sphere::Sphere(void): RenderObject()
{
	initVertices();
	initBuffers();
}


Sphere::~Sphere(void)
{
}

void Sphere::initVertices()
{
	int numDiv = 24;		//horizontal divisions
	int numBands = 24;		//vertical divisions
	numVert = numDiv * numBands;
	numInds = 6 * numDiv * numBands;

	vertices = new vertex[numVert];
	indices = new unsigned int [numInds];

	//define the triangle pairs that make up each face
	int count = 0;
	for (int i = 0; i < numBands; i++)
	{
		float yPos = 0.5f - (i * 1.0f/ (numBands-1));
		float rad = sqrt(0.5f*0.5f - yPos*yPos);
		for (int j = 0; j < numDiv; j++)
		{
			float angle = (float)(j * 2*M_PI / numDiv); 
			vertices[count].pos = Vector3f(rad*cos(angle), yPos, rad*sin(angle));
//			vertices[count].col = Vector3f(1.0, 1.0, 0.0);
			vertices[count].norm = normalize(Vector3f(vertices[count].pos.x, vertices[count].pos.y, vertices[count].pos.z));
			count++;
		}
	}

	//define indices for the entire sphere
	count = 0;
	for (int i = 0; i < numBands; i++)
	{
		for (int j = 0; j < numDiv; j++)
		{
			indices[count++] = i*numDiv + j;
			indices[count++] = i*numDiv + (j+1)%numDiv;
			indices[count++] = (i+1)*numDiv + (j+1)%numDiv;

			indices[count++] = i*numDiv + j;
			indices[count++] = (i+1)*numDiv + (j+1)%numDiv;
			indices[count++] = (i+1)*numDiv + j;
		}
	}
}

void Sphere::advance()
{
	rotateY(0.00005f);
}
