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
	vec4 base = texture2D(u_viewport_map, v_tex_coord);
	
	vec3 color;
	color.x = clamp(base.x - 0.6, 0.0, 0.6) * 1.6;
	color.y = clamp(base.y - 0.6, 0.0, 0.6) * 1.6;
	color.z = clamp(base.z - 0.6, 0.0, 0.6) * 1.6;

#if NEW_GL
	OutColor = vec4(color, 1);
#else
	gl_FragColor = vec4(color, 1);
#endif
}
