[GLSL_VERTEX_SHADER]

void main() {
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}

[GLSL_FRAGMENT_SHADER]
uniform sampler2D u_viewport_map;

void main() {
	vec4 base = texture2D(u_viewport_map, gl_TexCoord[0].xy);
	
	vec3 color;
	color.x = clamp(base.x - 0.6, 0.0, 0.6) * 1.6;
	color.y = clamp(base.y - 0.6, 0.0, 0.6) * 1.6;
	color.z = clamp(base.z - 0.6, 0.0, 0.6) * 1.6;

	gl_FragColor = vec4(color, 1);
}
