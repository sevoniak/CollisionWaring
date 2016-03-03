#include "RenderObject.h"


RenderObject::RenderObject(void)
{
	rotationMatrix = Matrix4f::identity();
	translationMatrix = Matrix4f::identity();
	scaleMatrix = Matrix4f::identity();
	parentMatrix = new Matrix4f();
	*parentMatrix = Matrix4f::identity();

	scaleFactors = Vector3f(6.0, 1.0, 1.0);

	fmat = new float[16];
}


RenderObject::~RenderObject(void)
{
	if (vertices) delete[] vertices;
}

vertex* RenderObject::getVertices() { return vertices; }

unsigned int* RenderObject::getIndices() { return indices; }

int RenderObject::getNumVert() { return numVert; }

int RenderObject::getNumInds() { return numInds; }

void RenderObject::setParentMatrix(Matrix4f* mat) { parentMatrix = mat; }

Matrix4f RenderObject::getWorldMatrix()
{
	worldMatrix = *parentMatrix * translationMatrix * rotationMatrix;
	return worldMatrix * scaleMatrix;
}

Matrix4f* RenderObject::getWorldMatrixPtr() { return &worldMatrix; }

void RenderObject::scale(float sx, float sy, float sz)
{
	Matrix4f xform = Matrix4f::identity();
	xform.vm[0].x = sx;
	xform.vm[1].y = sy;
	xform.vm[2].z = sz;
	scaleMatrix = xform * scaleMatrix;
}

void RenderObject::translate(float dx, float dy, float dz)
{
	Matrix4f xform = Matrix4f::identity();
	xform.vm[0].w = dx;
	xform.vm[1].w = dy;
	xform.vm[2].w = dz;
	translationMatrix = xform * translationMatrix;
}

void RenderObject::rotateX(float angle)
{
	Matrix4f xform = Matrix4f::identity();
	xform.vm[1].y = cos(angle);
	xform.vm[1].z = -sin(angle);
	xform.vm[2].y = sin(angle);
	xform.vm[2].z = cos(angle);
	rotationMatrix = xform * rotationMatrix;
}

void RenderObject::rotateY(float angle)
{
	Matrix4f xform = Matrix4f::identity();
	xform.vm[0].x = cos(angle);
	xform.vm[0].z = sin(angle);
	xform.vm[2].x = -sin(angle);
	xform.vm[2].z = cos(angle);
	rotationMatrix = xform * rotationMatrix;
}

void RenderObject::rotateZ(float angle)
{
	Matrix4f xform = Matrix4f::identity();
	xform.vm[0].x = cos(angle);
	xform.vm[0].y = -sin(angle);
	xform.vm[1].x = sin(angle);
	xform.vm[1].y = cos(angle);
	rotationMatrix = xform * rotationMatrix;
}

void RenderObject::initBuffers()
{
	glGenBuffers(1, &vtx_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vtx_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(struct vertex) * numVert, vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &ind_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ind_vbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numInds, indices, GL_STATIC_DRAW);
}

void RenderObject::render(GLuint shaderProg)
{
	glBindBuffer(GL_ARRAY_BUFFER, vtx_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ind_vbo);
	
	GLuint loc = glGetUniformLocation(shaderProg, "worldMat");
	glUniformMatrix4fv(loc, 1, GL_FALSE, convMat4fToArr(getWorldMatrix()));

	loc = glGetAttribLocation(shaderProg, "vPos");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex), 0);

	loc = glGetAttribLocation(shaderProg, "vNorm");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, sizeof(struct vertex), (void*)((int)&vertices[0].norm - (int)vertices));

	loc = glGetAttribLocation(shaderProg, "vTex");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, sizeof(struct vertex), (void*)((int)&vertices[0].tex - (int)vertices));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texBuf);
	loc = glGetUniformLocation(shaderProg, "texSamp");
	glUniform1i(loc, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texBuf2);
	loc = glGetUniformLocation(shaderProg, "texSampTwo");
	glUniform1i(loc, 1);

	glDrawElements(GL_TRIANGLES, numInds, GL_UNSIGNED_INT, 0);
}

//transpose and convert a Matrix4f into an array of floats.
float* RenderObject::convMat4fToArr(Matrix4f mat)
{
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

void RenderObject::modScaleFactors(float sx, float sy, float sz)
{
	scaleFactors = scaleFactors + Vector3f(sx, sy, sz);
	Matrix4f xform = Matrix4f::identity();
	xform.vm[0].x = scaleFactors.x;
	xform.vm[1].y = scaleFactors.y;
	xform.vm[2].z = scaleFactors.z;
	scaleMatrix = xform;
}

void RenderObject::resetScale()
{
	scaleFactors = Vector3f(1.0f, 1.0f, 1.0f);
	Matrix4f xform = Matrix4f::identity();
	xform.vm[0].x = 1.0f;
	xform.vm[1].y = 1.0f;
	xform.vm[2].z = 1.0f;
	scaleMatrix = xform;
}
