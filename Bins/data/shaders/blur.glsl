[GLSL_VERTEX_SHADER]

varying vec2 v_tex_coord;

void main() {
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	v_tex_coord = gl_MultiTexCoord0.xy;
}

[GLSL_FRAGMENT_SHADER]
#if NEW_GL
#version 330 core
//OUT
layout(location = 0) out vec4 OutColor;
#endif

varying vec2 v_tex_coord;
uniform sampler2D u_viewport_map;

void main() {
	float offset = 0.00390625;
	vec4 base = texture2D(u_viewport_map, v_tex_coord);
	
	base += texture2D(u_viewport_map, v_tex_coord + vec2(offset,   offset));
	base += texture2D(u_viewport_map, v_tex_coord + vec2(offset,  -offset));
	base += texture2D(u_viewport_map, v_tex_coord + vec2(-offset,  offset));
	base += texture2D(u_viewport_map, v_tex_coord + vec2(-offset, -offset));
	
	base += texture2D(u_viewport_map, v_tex_coord + vec2(0.0,   offset));
	base += texture2D(u_viewport_map, v_tex_coord + vec2(0.0,  -offset));
	base += texture2D(u_viewport_map, v_tex_coord + vec2(offset,  0.0));
	base += texture2D(u_viewport_map, v_tex_coord + vec2(-offset, 0.0));

#if NEW_GL
	OutColor = base * 0.11;
#else
	gl_FragColor = base * 0.11;
#endif
}
