[GLSL_VERTEX_SHADER]

varying vec3 Normal;
uniform mat4 u_mvp;
uniform mat4 u_world;

void main(void)
{
	Normal = normalize(u_world * vec4(gl_Normal.xyz, 0));
	gl_Position = u_mvp * gl_Vertex;
}

[GLSL_FRAGMENT_SHADER]
#version 330 core
//OUT
layout(location = 0) out vec4 OutColor;

varying vec3 Normal;

void main (void)
{
	float Edge = 0.5;
	float Phong = 0.9;
	vec3 DiffuseColor = vec3(0.0,0.0,0.6);
	vec3 PhongColor = vec3(1.0,1.0,1.0);
	vec3 color = DiffuseColor;
	
	float f = dot(vec3(0,0,1),Normal);
	if (abs(f) < Edge)
		color = vec3(0);
	if (f > Phong)
		color = PhongColor;

	OutColor = vec4(color, 1);
}
