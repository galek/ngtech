[GLSL_VERTEX_SHADER]

uniform mat4 u_mvp;
uniform mat4 u_world;
uniform mat4 u_viewport_transform;

varying vec2 v_tex_coord;
varying vec4 v_proj_coord;


void main() {
	gl_Position = u_mvp * gl_Vertex;
	v_tex_coord = gl_MultiTexCoord0.xy;
	
	v_proj_coord = u_viewport_transform * u_world * gl_Vertex;
}


[GLSL_FRAGMENT_SHADER]
#version 330
//OUT
layout(location = 0) out vec4 OutColor;

varying vec2 v_tex_coord;
varying vec4 v_proj_coord;

uniform vec3 u_light_color;
uniform sampler2D u_texture_1;
uniform sampler2D u_viewport_map;

void main() {
	vec3 normal = 2.0 * texture2D(u_texture_1, v_tex_coord).xyz - 1.0;
	OutColor = texture2DProj(u_viewport_map, v_proj_coord + vec4(normal, 0.0));
}