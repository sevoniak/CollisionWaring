#pragma once
#include "TexturedQuad.h"
#include <vector>
using namespace std;

class TexturedCube : public RenderObject
{
private:
	TexturedQuad sides[6];
	vector<RenderObject*> models;
	float x, y, z;

	void initVertices();

public:
	TexturedCube(float x, float y, float z, char* names[]);
	~TexturedCube(void);

	vector<RenderObject*> getModels();
	void advance();
	void flip();
};

