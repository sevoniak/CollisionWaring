uniform mat4 worldMat;
uniform mat4 viewMat;
uniform mat4 projMat;
uniform vec4 lightPos;

attribute vec3 vPos;
attribute vec3 vNorm;
attribute vec2 vTex;

varying vec4 outPos;
varying vec4 outNorm;
varying vec2 outTex;


void main()
{	
	outPos = viewMat * worldMat * vec4(vPos, 1.0);
	outNorm = viewMat * worldMat * vec4(vNorm, 0.0f);
	outTex = vTex;

	gl_Position = projMat * viewMat * worldMat * vec4(vPos, 1.0);
}