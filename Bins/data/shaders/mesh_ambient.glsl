[GLSL_VERTEX_SHADER]

uniform mat4 u_world;
uniform vec3 u_view_pos;
uniform mat4 u_mvp;

varying vec3 v_view_vec;
varying vec2 v_tex_coord;

void main() {
	gl_Position = u_mvp * gl_Vertex;
	v_tex_coord = gl_MultiTexCoord0.xy;
	
	vec3 tangent  = normalize((u_world * vec4(gl_MultiTexCoord1.xyz, 0)).xyz); 
	vec3 binormal = normalize((u_world * vec4(gl_MultiTexCoord2.xyz, 0)).xyz); 
	vec3 normal   = normalize((u_world * vec4(gl_Normal.xyz, 0)).xyz); 
	
	vec4 worldPos = u_world * gl_Vertex;
	 
	vec3 vVec = u_view_pos - worldPos.xyz;
		
	v_view_vec.x = dot(tangent, vVec); 
	v_view_vec.y = dot(binormal, vVec);
	v_view_vec.z = dot(normal , vVec);
}


[GLSL_FRAGMENT_SHADER]

varying vec2 v_tex_coord;
varying vec3 v_view_vec;

uniform vec3 u_light_color;
uniform sampler2D u_texture_0;
uniform sampler2D u_texture_1;

uniform vec4 u_material_param_0;

void main() {
	vec3 vVec = normalize(v_view_vec);

	vec2 texCoord = v_tex_coord;
#ifdef PARALLAX
	float height = texture2D(u_texture_1, v_tex_coord).w;
	float offset = height * 0.02 - 0.01;
	texCoord = texCoord + offset * vVec.xy * u_material_param_0.z;
#endif

	vec4 baseColor = texture2D(u_texture_0, texCoord);
	gl_FragColor = baseColor * vec4(u_light_color, 1.0);
}