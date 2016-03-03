#include "ShaderLoader.h"

ShaderLoader::ShaderLoader(void)
{
	vShader_fName = "";
	fShader_fName = "";
}

ShaderLoader::ShaderLoader(string vsName, string fsName)
{
	vShader_fName = vsName;
	fShader_fName = fsName;
}

ShaderLoader::~ShaderLoader(void)
{
}

string ShaderLoader::loadVertexShader()
{
	return loadFile(vShader_fName);
}

string ShaderLoader::loadFragmentShader()
{
	return loadFile(fShader_fName);
}

string ShaderLoader::loadFile(string fName)
{
	ifstream file;
	file.open(fName.c_str());
	if (!file.good())
	{
		printf("Error, can't open file.");
		return "";
	}

	string buffer, line;
	while (getline(file, line))
		buffer += line + "\n";
	file.close();
	return buffer + "\0";
}
