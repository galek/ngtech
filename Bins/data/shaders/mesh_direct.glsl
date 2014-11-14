[GLSL_VERTEX_SHADER]

uniform mat4 u_mvp;
uniform mat4 u_world;
uniform vec3 u_light_dir;
uniform vec3 u_view_pos;

varying vec2 v_tex_coord;
varying vec3 v_light_vec;
varying vec3 v_view_vec;

void main() {
	gl_Position = u_mvp * gl_Vertex; 
	v_tex_coord = gl_MultiTexCoord0.xy;

	vec3 tangent  = normalize((u_world * vec4(gl_MultiTexCoord1.xyz, 0)).xyz); 
	vec3 binormal = normalize((u_world * vec4(gl_MultiTexCoord2.xyz, 0)).xyz); 
	vec3 normal   = normalize((u_world * vec4(gl_Normal.xyz, 0)).xyz); 
	
    vec4 worldPos = u_world * gl_Vertex;

	vec3 lVec = -u_light_dir;
	
	v_light_vec.x = dot(tangent, lVec); 
	v_light_vec.y = dot(binormal, lVec);
	v_light_vec.z = dot(normal, lVec);
	
	vec3 vVec = u_view_pos - worldPos.xyz;
		
	v_view_vec.x = dot(tangent, vVec); 
	v_view_vec.y = dot(binormal, vVec);
	v_view_vec.z = dot(normal , vVec);
}



[GLSL_FRAGMENT_SHADER]
//OUT
layout(location = 0) out vec4 OutColor;

varying vec2 v_tex_coord;
varying vec3 v_light_vec;
varying vec3 v_view_vec;

uniform vec3 u_light_color;
uniform vec4 u_material_param_0;

uniform sampler2D u_texture_0;
uniform sampler2D u_texture_1;

void main() {

	vec3 lVec = normalize(v_light_vec);
	vec3 vVec = normalize(v_view_vec);
	
	vec2 texCoord = v_tex_coord;
#ifdef PARALLAX
	float height = texture2D(u_texture_1, v_tex_coord).w;
	float offset = height * 0.02 - 0.01;
	texCoord = texCoord + offset * vVec.xy * u_material_param_0.z;
#endif

	vec4 baseColor = texture2D(u_texture_0, texCoord);
	vec3 normal = 2.0 * texture2D(u_texture_1, texCoord).xyz - 1.0;
	
    float diffuse = clamp(dot(normal, lVec), 0.0, 1.0);

	float specular = 0.0;
#ifdef SPECULAR
	specular = pow(clamp(dot(reflect(-vVec, normal), lVec), 0.0, 1.0), u_material_param_0.y) * u_material_param_0.x;
#endif
	
	OutColor = (baseColor * diffuse + specular) * vec4(u_light_color, 1.0);
}
