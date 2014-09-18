[GLSL_VERTEX_SHADER]

uniform mat4 u_mvp;
uniform mat4 u_proj_transform;

varying vec2 v_tex_coord;
varying vec4 v_proj_coord;

void main() {
	gl_Position = u_mvp * gl_Vertex; 
	v_tex_coord = gl_MultiTexCoord0.xy;
	
	v_proj_coord = u_proj_transform * gl_Vertex;
}


[GLSL_FRAGMENT_SHADER]

varying vec2 v_tex_coord;
varying vec4 v_proj_coord;

uniform sampler2D u_viewport_map;
uniform sampler2D u_dudv_map;

uniform float u_time;

void main() {
	vec3 noise = texture2D(u_dudv_map, v_tex_coord * 20.0 + vec2(u_time, u_time) * 0.00005).xyz * 0.5;
	vec4 refrColor = texture2DProj(u_viewport_map, v_proj_coord + vec4(noise, 0.0));
	gl_FragColor = refrColor;
}