#include "TexturedCube.h"


TexturedCube::TexturedCube(float xIn, float yIn, float zIn, char* names[])
{
	x = xIn;
	y = yIn;
	z = zIn;

	sides[0] = TexturedQuad(x, y, names[0], names[1]);
	sides[1] = TexturedQuad(x, y, names[2], names[3]);
	sides[2] = TexturedQuad(x, z, names[4], names[5]);
	sides[3] = TexturedQuad(x, z, names[6], names[7]);
	sides[4] = TexturedQuad(z, y, names[8], names[9]);
	sides[5] = TexturedQuad(z, y, names[10], names[11]);

	sides[0].translate(0.0f, 0.0f, z/2);
	sides[1].translate(0.0f, 0.0f, -z/2);
	sides[2].translate(0.0f, y/2, 0.0f);
	sides[3].translate(0.0f, -y/2, 0.0f);
	sides[4].translate(x/2, 0.0f, 0.0f);
	sides[5].translate(-x/2, 0.0f, 0.0f);
	
	sides[1].rotateY(3.14159f);
	sides[2].rotateX(-3.14159f/2);
	sides[3].rotateX(3.14159f/2);
	sides[4].rotateY(3.14159f/2);
	sides[5].rotateY(-3.14159f/2);

	getWorldMatrix();
	Matrix4f* p = getWorldMatrixPtr();
	for (int i = 0; i < 6; i++)
	{
		sides[i].setParentMatrix(p);
		models.push_back(&sides[i]);
	}
}


TexturedCube::~TexturedCube(void)
{
}

vector<RenderObject*> TexturedCube::getModels() { return models; }

void TexturedCube::initVertices()
{
}

void TexturedCube::advance()
{
	rotateY(0.0001f);
}

void TexturedCube::flip()
{
	sides[0].translate(0.0f, 0.0f, -z);
	sides[1].translate(0.0f, 0.0f, z);
	sides[2].translate(0.0f, -y, 0.0f);
	sides[3].translate(0.0f, y, 0.0f);
	sides[4].translate(-x, 0.0f, 0.0f);
	sides[5].translate(x, 0.0f, 0.0f);
}
