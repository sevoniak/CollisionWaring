uniform vec4 lightPos;
uniform sampler2D texSamp;
uniform sampler2D texSampTwo;
uniform int light;

varying vec4 outPos;
varying vec4 outNorm;
varying vec2 outTex;


// Phong Shading

void main()
{
	vec3 L, N, V, R;
	
	N = normalize(outNorm);
	V = vec3(-outPos);
	V = normalize(V);
	L = (vec3)(lightPos - outPos);
	L = normalize(L);

	//Light source
	float Id = clamp(dot(N,L), 0.0, 1.0);

	R = 2*dot(L,N)*N - L;
	float Is = clamp(dot(V,R), 0.0, 1.0);
	Is = pow(Is, 120);
	
	float Ia = 0.2f;

	float I = 0.7*Id + 0.7*Is + Ia;

	float factor = clamp(length(outPos) - 2.0, 0.0, 1.0);

	vec4 cr1 = texture2D(texSamp, outTex);
	vec4 cr2 = texture2D(texSampTwo, outTex);
	vec4 cr = mix(cr1, cr2, factor);

	if (light > 0)
		gl_FragColor = vec4(I*cr);
	else
		gl_FragColor = vec4(cr);
}