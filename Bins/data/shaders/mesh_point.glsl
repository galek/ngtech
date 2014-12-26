[GLSL_VERTEX_SHADER]

uniform mat4 u_mvp;
uniform mat4 u_world;
uniform vec3 u_light_pos;
uniform vec3 u_view_pos;
uniform float u_inv_radius;

varying vec2 v_tex_coord;
varying vec3 v_light_vec;
varying vec3 v_view_vec;
varying vec3 v_s_vec;

void main() {
	gl_Position = u_mvp * gl_Vertex; 
	v_tex_coord = gl_MultiTexCoord0.xy;

	vec3 tangent  = normalize((u_world * vec4(gl_MultiTexCoord1.xyz, 0)).xyz); 
	vec3 binormal = normalize((u_world * vec4(gl_MultiTexCoord2.xyz, 0)).xyz); 
	vec3 normal   = normalize((u_world * vec4(gl_Normal.xyz, 0)).xyz); 
	
    vec4 worldPos = u_world * gl_Vertex;

	vec3 lVec = (u_light_pos - worldPos.xyz) * u_inv_radius;
	v_s_vec = -lVec;
	
	v_light_vec.x = dot(tangent, lVec); 
	v_light_vec.y = dot(binormal, lVec);
	v_light_vec.z = dot(normal, lVec);
	
	vec3 vVec = u_view_pos - worldPos.xyz;
		
	v_view_vec.x = dot(tangent, vVec); 
	v_view_vec.y = dot(binormal, vVec);
	v_view_vec.z = dot(normal , vVec);
}



[GLSL_FRAGMENT_SHADER]
#version 330 core
//OUT
layout(location = 0) out vec4 OutColor;

varying vec2 v_tex_coord;
varying vec3 v_light_vec;
varying vec3 v_view_vec;
varying vec3 v_s_vec;

uniform vec3 u_light_color;
uniform vec4 u_material_param_0;

uniform sampler2D u_texture_0;
uniform sampler2D u_texture_1;
uniform samplerCube u_shadow_map;

void main() {

	float atten = clamp(1.0 - dot(v_light_vec, v_light_vec), 0.0, 1.0);

	float shadow = 1.0;
	
#ifdef SM_SHADOWS
	float distance = textureCube(u_shadow_map, v_s_vec).r + 0.01;
	shadow = float(dot(v_s_vec, v_s_vec) < distance * distance);
#endif
	
#ifdef SM_SHADOWS_PCF_2
	float distance = textureCube(u_shadow_map, v_s_vec).r + 0.01;
	shadow = float(dot(v_s_vec, v_s_vec) < distance * distance);
	
	vec3 svec;
	
	svec = v_s_vec + vec3(2.0, -1.0,  1.0) / 1024.0;
	distance = textureCube(u_shadow_map, svec).r + 0.01;
	shadow += float(dot(v_s_vec, v_s_vec) < distance * distance);
	
	svec = v_s_vec + vec3(1.0, 2.0, -1.0) / 1024.0;
	distance = textureCube(u_shadow_map, svec).r + 0.01;
	shadow += float(dot(v_s_vec, v_s_vec) < distance * distance);
	
	svec = v_s_vec + vec3(-1.0, 1.0, 2.0) / 1024.0;
	distance = textureCube(u_shadow_map, svec).r + 0.01;
	shadow += float(dot(v_s_vec, v_s_vec) < distance * distance);	
	
	shadow *= 0.25;
#endif

#ifdef SM_SHADOWS_PCF_3
	float distance = textureCube(u_shadow_map, v_s_vec).r + 0.01;
	shadow = float(dot(v_s_vec, v_s_vec) < distance * distance);
	
	vec3 svec;
	
	svec = v_s_vec + vec3(2.0, -1.0,  1.0) / 1024.0;
	distance = textureCube(u_shadow_map, svec).r + 0.01;
	shadow += float(dot(svec, svec) < distance * distance);
	
	svec = v_s_vec + vec3(1.0, 2.0, -1.0) / 1024.0;
	distance = textureCube(u_shadow_map, svec).r + 0.01;
	shadow += float(dot(svec, svec) < distance * distance);
	
	svec = v_s_vec + vec3(-1.0, 1.0, 2.0) / 1024.0;
	distance = textureCube(u_shadow_map, svec).r + 0.01;
	shadow += float(dot(svec, svec) < distance * distance);	
	
	
	svec = v_s_vec + vec3(2.0, 1.0,  1.0) / 1024.0;
	distance = textureCube(u_shadow_map, svec).r + 0.01;
	shadow += float(dot(v_s_vec, v_s_vec) < distance * distance);
	
	svec = v_s_vec + vec3(-1.0, 2.0, 1.0) / 1024.0;
	distance = textureCube(u_shadow_map, svec).r + 0.01;
	shadow += float(dot(v_s_vec, v_s_vec) < distance * distance);
	
	svec = v_s_vec + vec3(1.0, -1.0, 2.0) / 1024.0;
	distance = textureCube(u_shadow_map, svec).r + 0.01;
	shadow += float(dot(v_s_vec, v_s_vec) < distance * distance);		
	
	
	shadow *= 0.142857;
#endif

#ifdef VSM_SHADOWS
	float mom = textureCube(u_shadow_map, v_s_vec).r;

	float variance = 0.001;
	float distance = length(v_s_vec);
	float sigma = distance - mom;
	shadow = (sigma > 0.0) ? variance / (variance + sigma * sigma) : 1.0;
#endif
	
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
	
	OutColor = (baseColor * diffuse + specular) * vec4(u_light_color, 1.0) * atten * shadow;
}
