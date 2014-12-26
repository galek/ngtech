[GLSL_VERTEX_SHADER]

uniform mat4 u_mvp;
uniform mat4 u_world;
uniform vec3 u_light_pos;
uniform float u_inv_radius;

varying vec3 v_light_vec;

void main() {
	gl_Position = u_mvp * gl_Vertex; 
	
	vec4 worldPos = u_world * gl_Vertex;
	v_light_vec = u_inv_radius * (u_light_pos - worldPos.xyz);
}


[GLSL_FRAGMENT_SHADER]
#version 330 core
//OUT
layout(location = 0) out vec4 OutColor;

varying vec3 v_light_vec;

void main() {
	float depth = length(v_light_vec);
	
	OutColor = vec4(depth, 0, 0, 0);
}
