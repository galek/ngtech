[GLSL_VERTEX_SHADER]

uniform mat4 u_mvp;
uniform vec3 u_light_pos;
uniform float u_inv_radius;
uniform mat4 u_spot_proj_transform;

varying vec2 v_tex_coord;
varying vec3 v_light_vec;
varying vec3 v_s_vec;
varying vec3 v_normal;
varying vec4 v_spot_proj_coord;

void main() {
	gl_Position = u_mvp * gl_Vertex; 
	v_tex_coord = gl_MultiTexCoord0.xy;

	v_normal = gl_Normal.xyz; 

	v_light_vec = (u_light_pos - gl_Vertex.xyz) * u_inv_radius;
	v_s_vec = -v_light_vec;
	
	v_spot_proj_coord = u_spot_proj_transform * gl_Vertex;
}



[GLSL_FRAGMENT_SHADER]
#version 330 core
//OUT
layout(location = 0) out vec4 OutColor;

varying vec2 v_tex_coord;
varying vec3 v_light_vec;
varying vec3 v_s_vec;
varying vec3 v_normal;
varying vec4 v_spot_proj_coord;

uniform vec3 u_light_dir;
uniform vec3 u_light_color;
uniform sampler2D u_texture_0;
uniform sampler2D u_shadow_map;
uniform sampler2D u_spot_map;
uniform sampler2D u_detail_map;
uniform float u_detail_map_scale;

void main() {
	float atten = 1.0;
	atten = clamp(1.0 - dot(v_light_vec, v_light_vec), 0.0, 1.0);

	float shadow = 1.0;
	
#ifdef SM_SHADOWS
	float distance = texture2DProj(u_shadow_map, v_spot_proj_coord).r + 0.01;
	shadow = float(dot(v_s_vec, v_s_vec) < distance * distance);
#endif

#ifdef SM_SHADOWS_PCF_2
	float distance = texture2DProj(u_shadow_map, v_spot_proj_coord).r + 0.01;
	shadow = float(dot(v_s_vec, v_s_vec) < distance * distance);
	
	vec4 svec;
	
	svec = v_spot_proj_coord + vec4(1.0, -1.0,  0.0, 0.0) / 4.0;
	distance = texture2DProj(u_shadow_map, svec).r + 0.01;
	shadow += float(dot(v_s_vec, v_s_vec) < distance * distance);
	
	svec = v_spot_proj_coord + vec4(1.0, 1.0, 0.0, 0.0) / 4.0;
	distance = texture2DProj(u_shadow_map, svec).r + 0.01;
	shadow += float(dot(v_s_vec, v_s_vec) < distance * distance);
	
	svec = v_spot_proj_coord + vec4(-1.0, 1.0, 0.0, 0.0) / 4.0;
	distance = texture2DProj(u_shadow_map, svec).r + 0.01;
	shadow += float(dot(v_s_vec, v_s_vec) < distance * distance);	
	
	shadow *= 0.25;
#endif

#ifdef SM_SHADOWS_PCF_3
	float distance = texture2DProj(u_shadow_map, v_spot_proj_coord).r + 0.01;
	shadow = float(dot(v_s_vec, v_s_vec) < distance * distance);
	
	vec4 svec;
	
	svec = v_spot_proj_coord + vec4(1.0, -1.0,  0.0, 0.0) / 4.0;
	distance = texture2DProj(u_shadow_map, svec).r + 0.01;
	shadow += float(dot(v_s_vec, v_s_vec) < distance * distance);
	
	svec = v_spot_proj_coord + vec4(1.0, 1.0, 0.0, 0.0) / 4.0;
	distance = texture2DProj(u_shadow_map, svec).r + 0.01;
	shadow += float(dot(v_s_vec, v_s_vec) < distance * distance);
	
	svec = v_spot_proj_coord + vec4(-1.0, 1.0, 0.0, 0.0) / 4.0;
	distance = texture2DProj(u_shadow_map, svec).r + 0.01;
	shadow += float(dot(v_s_vec, v_s_vec) < distance * distance);	
	
	
	svec = v_spot_proj_coord + vec4(-1.0, -1.0,  0.0, 0.0) / 4.0;
	distance = texture2DProj(u_shadow_map, svec).r + 0.01;
	shadow += float(dot(v_s_vec, v_s_vec) < distance * distance);
	
	svec = v_spot_proj_coord + vec4(2.0, 1.0, 0.0, 0.0) / 4.0;
	distance = texture2DProj(u_shadow_map, svec).r + 0.01;
	shadow += float(dot(v_s_vec, v_s_vec) < distance * distance);
	
	svec = v_spot_proj_coord + vec4(-1.0, 2.0, 0.0, 0.0) / 4.0;
	distance = texture2DProj(u_shadow_map, svec).r + 0.01;
	shadow += float(dot(v_s_vec, v_s_vec) < distance * distance);	
	
	shadow *= 0.142857;
#endif

	if(dot(v_s_vec, u_light_dir) < 0.05) shadow = 0.0;
	
	vec4 baseColor = texture2D(u_texture_0, v_tex_coord);
	vec4 detColor = texture2D(u_detail_map, v_tex_coord * u_detail_map_scale);

	vec3 lVec = normalize(v_light_vec);
	float diffuse = (clamp(dot(v_normal, lVec), 0.0, 1.0) - 0.5) * 2.0;

	OutColor = baseColor * detColor * diffuse * vec4(u_light_color, 1.0) * atten * shadow * texture2DProj(u_spot_map, v_spot_proj_coord);
}
