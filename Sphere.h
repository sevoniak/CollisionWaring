#pragma once

#include "RenderObject.h"
#include <math.h>
using namespace std;

#define MAX_SHININESS 300
#define MIN_SHININESS 1

class Sphere : public RenderObject
{
private:
	void initVertices();	//implementation of method in parent class

public:
	Sphere(void);
	~Sphere(void);	

	void advance();
};

