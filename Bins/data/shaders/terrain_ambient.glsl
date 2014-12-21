[GLSL_VERTEX_SHADER]

uniform mat4 u_mvp;
varying vec2 v_tex_coord;

void main() {
	gl_Position = u_mvp * gl_Vertex; 
	v_tex_coord = gl_MultiTexCoord0.xy;
}


[GLSL_FRAGMENT_SHADER]
#version 330
//OUT
layout(location = 0) out vec4 OutColor;

varying vec2 v_tex_coord;

uniform vec3 u_light_color;
uniform sampler2D u_texture_0;
uniform sampler2D u_detail_map;
uniform float u_detail_map_scale;

void main() {
	vec4 baseColor = texture2D(u_texture_0, v_tex_coord);
	vec4 detColor = texture2D(u_detail_map, v_tex_coord * u_detail_map_scale);
	
	OutColor = baseColor * detColor * vec4(u_light_color, 1.0);
}