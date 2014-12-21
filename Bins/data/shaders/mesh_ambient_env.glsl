[GLSL_VERTEX_SHADER]

uniform mat4 u_mvp;
uniform mat4 u_world;
varying vec2 v_tex_coord;
varying vec3 v_env_coord;

void main() {
	gl_Position = u_mvp * gl_Vertex;
	v_env_coord = normalize(u_world * vec4(0.0, 0.0, 0.0, 0.0) - u_world * gl_Vertex).xyz;	
	v_tex_coord = gl_MultiTexCoord0.xy;
}


[GLSL_FRAGMENT_SHADER]
#version 330
//OUT
layout(location = 0) out vec4 OutColor;

varying vec2 v_tex_coord;
varying vec3 v_env_coord;

uniform vec3 u_light_color;
uniform sampler2D u_texture_1;
uniform samplerCube u_env_map;

void main() {
	vec3 normal = 2.0 * texture2D(u_texture_1, v_tex_coord).xyz - 1.0;
	OutColor = textureCube(u_env_map, v_env_coord + normal * 0.2);
}