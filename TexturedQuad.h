#pragma once
#include "RenderObject.h"
#include "SOIL.h"
#include <vector>
using namespace std;

class TexturedQuad : public RenderObject
{
public:
	TexturedQuad(void);
	TexturedQuad(float xLength, float yLength, char* filename, char* filename2);
	virtual ~TexturedQuad(void);
	virtual void shutdown();
	virtual void advance();

private:
	float xLength;
	float yLength;
	char* filename;
	char* filename2;

	virtual void initVertices();
	void loadImage();
};

