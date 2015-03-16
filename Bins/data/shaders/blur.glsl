[GLSL_VERTEX_SHADER]


void main() {
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}

[GLSL_FRAGMENT_SHADER]
uniform sampler2D u_viewport_map;

void main() {
	vec2 dx   = vec2 ( 1.0 / 512.0, 0.0 );
    vec2 dy   = vec2 ( 0.0, 1.0 / 512.0 );
	vec4 base = texture2D(u_viewport_map, gl_TexCoord[0].xy);
	
	base += texture2D(u_viewport_map, gl_TexCoord[0].xy + dx );
	base += texture2D(u_viewport_map, gl_TexCoord[0].xy + dy);
	base += texture2D(u_viewport_map, gl_TexCoord[0].xy + dx + dy);

	base *= 0.25;

	if ( length ( base.rgb ) < 0.8 )
        base.rgb = vec3 ( 0.0 );
	
	gl_FragColor = base;
}
