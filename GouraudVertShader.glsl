uniform mat4 worldMat;
uniform mat4 viewMat;
uniform mat4 projMat;

uniform vec4 lightPos;
uniform vec3 lightAmb;
uniform vec3 lightDiff;
uniform vec3 lightSpec;
uniform vec3 matAmb;
uniform vec3 matDiff;
uniform vec3 matSpec;
uniform vec3 lightBools;
uniform int shine;

attribute vec3 vPos;
attribute vec3 vCol;
attribute vec3 vNorm;

varying vec4 outCol;


void main()
{	
	vec4 outPos = viewMat * worldMat * vec4(vPos, 1.0);
	vec4 outNorm = vec4(transpose(inverse(mat3(viewMat * worldMat))) * vNorm, 0.0f);

	vec3 L, N, V, R;
	
	N = normalize(outNorm);
	V = vec3(-outPos);
	V = normalize(V);
	L = (vec3)(lightPos - outPos);
	L = normalize(L);

	//Light source
	float Id = clamp(dot(N,L), 0.0, 1.0);
	vec3 KdLd = matDiff * lightDiff;

	R = 2*dot(L,N)*N - L;
	float Is = clamp(dot(V,R), 0.0, 1.0);
	Is = pow(Is, shine);
	vec3 KsLs = matSpec * lightSpec;
	
	float Ia = 0.1f;
	vec3 KaLa = matAmb * lightAmb;

	if (lightBools.x < 0.5f)
		Id = 0.0f;
	if (lightBools.y < 0.5f)
		Is = 0.0f;
	if (lightBools.z < 0.5f)
		Ia = 0.0f;
	vec3 I = Id*KdLd + Is*KsLs + Ia*KaLa;

	outCol = vec4(I, 1.0);

	gl_Position = projMat * viewMat * worldMat * vec4(vPos, 1.0);
}