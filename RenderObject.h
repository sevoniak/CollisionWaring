#pragma once
#include "vertexType.h"
#include "nuss_matrix.h"
#include <glew.h>

// Abstract class that defines a renderable object.
// All basic shape objects should inherit from this.
class RenderObject
{
private:
	GLuint vtx_vbo;
	GLuint ind_vbo;
	
	Matrix4f worldMatrix;
	Matrix4f translationMatrix;
	Matrix4f rotationMatrix;
	Matrix4f scaleMatrix;
	Matrix4f* parentMatrix;

	Vector3f scaleFactors;
	float* fmat;
	float* convMat4fToArr(Matrix4f mat);

protected:
	int numInds;
	int numVert;
	vertex* vertices;
	unsigned int* indices;
	GLuint texBuf;
	GLuint texBuf2;

	void initBuffers();
	virtual void initVertices() = 0;	//derived classes should implement this

public:
	RenderObject(void);
	virtual ~RenderObject(void);

	vertex* getVertices();
	unsigned int* getIndices();
	int getNumVert();
	int getNumInds();
	Matrix4f getWorldMatrix();
	Matrix4f* getWorldMatrixPtr();
	void setParentMatrix(Matrix4f*);	//for hierarchical models

	//transformation functions
	void modScaleFactors(float, float, float);
	void resetScale();
	void scale(float, float, float);
	void translate(float, float, float);
	void rotateX(float);
	void rotateY(float);
	void rotateZ(float);

	void render(GLuint);
};

