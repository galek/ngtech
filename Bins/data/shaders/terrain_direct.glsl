[GLSL_VERTEX_SHADER]

uniform mat4 u_mvp;
uniform vec3 u_light_dir;

varying vec2 v_tex_coord;
varying vec3 v_light_vec;
varying vec3 v_normal;

void main() {
	gl_Position = u_mvp * gl_Vertex; 
	v_tex_coord = gl_MultiTexCoord0.xy;

	v_normal = normalize(gl_Normal.xyz); 

	v_light_vec = -u_light_dir;
}



[GLSL_FRAGMENT_SHADER]
//OUT
layout(location = 0) out vec4 OutColor;

varying vec2 v_tex_coord;
varying vec3 v_light_vec;
varying vec3 v_normal;

uniform vec3 u_light_color;
uniform sampler2D u_texture_0;
uniform sampler2D u_detail_map;
uniform float u_detail_map_scale;

void main() {

	vec3 lVec = normalize(v_light_vec);

	vec4 baseColor = texture2D(u_texture_0, v_tex_coord);
	vec4 detColor = texture2D(u_detail_map, v_tex_coord * u_detail_map_scale);
		
    float diffuse = (clamp(dot(v_normal, lVec), 0.0, 1.0) - 0.5) * 2.0;

	OutColor = baseColor * detColor * diffuse * vec4(u_light_color, 1.0);
}
