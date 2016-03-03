#pragma once
#include <ios>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class ShaderLoader
{
private:
	string vShader_fName;
	string fShader_fName;

	string loadFile(string fName);

public:
	ShaderLoader(void);
	ShaderLoader(string, string);
	~ShaderLoader(void);

	string loadVertexShader();
	string loadFragmentShader();
};

