/* Copyright (C) 2005-2014, Unigine Corp. All rights reserved.
 *
 * File:    GLFfp.h
 * Desc:    OpenGL fixed function pipeline
 * Version: 1.17
 * Author:  Alexander Zapryagaev <frustum@unigine.com>
 *
 * This file is part of the Unigine engine (http://unigine.com/).
 *
 * Your use and or redistribution of this software in source and / or
 * binary form, with or without modification, is subject to: (i) your
 * ongoing acceptance of and compliance with the terms and conditions of
 * the Unigine License Agreement; and (ii) your inclusion of this notice
 * in any version of this software that you use or redistribute.
 * A copy of the Unigine License Agreement is available by contacting
 * Unigine Corp. at http://unigine.com/
 */

#include "RenderPrivate.h"
#include "GLFfp.h"
#include "GLExt.h"

namespace NGTech
{
	/*
	 */
#define GL_FFP_BUFFER_SIZE	(1024 * 3)

	/*
	 */
	GLFfp::GLFfp() {

		mode = -1;
		enabled = 0;

		frame = -1;

		transform.Identity();

		default_program_id = 0;
		multisample_program_id = 0;
		shadow_program_id = 0;
		splash_program_id = 0;
		solid_program_id = 0;
		cube_program_id = 0;
		grad_program_id = 0;
		srgb_program_id = 0;
		yuv_program_id = 0;

		num_vertex = 0;
		vertex_offset = 0;
		num_frame_vertex = 0;
		vertex_vao_id = 0;
		vertex_vbo_id = 0;
//#ifdef USE_OPENGL_44
		vertex_ptr = NULL;
		vertex_sync[0] = NULL;
		vertex_sync[1] = NULL;
		vertex_sync[2] = NULL;
//#endif

		num_indices = 0;
		indices_offset = 0;
		num_frame_indices = 0;
		indices_vbo_id = 0;
//#ifdef USE_OPENGL_44
		indices_ptr = NULL;
		indices_sync[0] = NULL;
		indices_sync[1] = NULL;
		indices_sync[2] = NULL;
//#endif

		screen_vertex_vao_id = 0;
		screen_vertex_vbo_id = 0;
	}

	GLFfp::~GLFfp() {

		if (glIsProgram(default_program_id)) glDeleteProgram(default_program_id);
		if (glIsProgram(multisample_program_id)) glDeleteProgram(multisample_program_id);
		if (glIsProgram(shadow_program_id)) glDeleteProgram(shadow_program_id);
		if (glIsProgram(splash_program_id)) glDeleteProgram(splash_program_id);
		if (glIsProgram(solid_program_id)) glDeleteProgram(solid_program_id);
		if (glIsProgram(cube_program_id)) glDeleteProgram(cube_program_id);
		if (glIsProgram(grad_program_id)) glDeleteProgram(grad_program_id);
		if (glIsProgram(srgb_program_id)) glDeleteProgram(srgb_program_id);
		if (glIsProgram(yuv_program_id)) glDeleteProgram(yuv_program_id);

		if (glIsVertexArray(vertex_vao_id)) glDeleteVertexArrays(1, &vertex_vao_id);
		if (glIsBuffer(vertex_vbo_id)) glDeleteBuffers(1, &vertex_vbo_id);
//#ifdef USE_OPENGL_44
		if (glIsSync(vertex_sync[0])) glDeleteSync(vertex_sync[0]);
		if (glIsSync(vertex_sync[1])) glDeleteSync(vertex_sync[1]);
		if (glIsSync(vertex_sync[2])) glDeleteSync(vertex_sync[2]);
//#endif

		if (glIsBuffer(indices_vbo_id)) glDeleteBuffers(1, &indices_vbo_id);
//#ifdef USE_OPENGL_44
		if (glIsSync(indices_sync[0])) glDeleteSync(indices_sync[0]);
		if (glIsSync(indices_sync[1])) glDeleteSync(indices_sync[1]);
		if (glIsSync(indices_sync[2])) glDeleteSync(indices_sync[2]);
//#endif

		if (glIsVertexArray(screen_vertex_vao_id)) glDeleteVertexArrays(1, &screen_vertex_vao_id);
		if (glIsBuffer(screen_vertex_vbo_id)) glDeleteBuffers(1, &screen_vertex_vbo_id);
	}

	/*
	 */
	GLuint GLFfp::create_shader(GLuint type, const char *src) {

		GLuint shader = glCreateShader(type);
		glShaderSource(shader, 1, (const GLchar**)&src, NULL);
		glCompileShader(shader);

		GLint status = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE) {
			char info[32768];
			glGetShaderInfoLog(shader, sizeof(info), NULL, info);
			Warning("GLFfp::create_shader(): can't create shader\n%s", info);//Error
			glDeleteShader(shader);
			return 0;
		}

		return shader;
	}

	GLuint GLFfp::create_program(GLuint vertex_shader, GLuint fragment_shader) {

		GLuint program_id = glCreateProgram();

		glAttachShader(program_id, vertex_shader);
		glAttachShader(program_id, fragment_shader);

		glBindFragDataLocation(program_id, 0, "s_frag_data_0");
		glBindFragDataLocation(program_id, 1, "s_frag_data_1");

		GLint status = 0;
		glLinkProgram(program_id);
		glGetProgramiv(program_id, GL_LINK_STATUS, &status);
		if (status == GL_FALSE) {
			char error[32768];
			glGetProgramInfoLog(program_id, sizeof(error), NULL, error);
			Warning("GLFfp::create_programs(): can't link program\n%s", error);//Error
			glDeleteProgram(program_id);
			return 0;
		}

		GLint old_program_id = GLExt::getProgramID();
		GLExt::setProgramID(program_id);
		GLint location = glGetUniformLocation(program_id, "s_texture_0");
		if (location >= 0) glUniform1i(location, 0);
		GLExt::setProgramID(old_program_id);

		return program_id;
	}

	/*
	 */
	void GLFfp::create_programs() {

//#ifdef USE_OPENGL_44
		static const char vertex_shader_src[] =
			"#version 430\n"
			"layout(location = 0) in vec4 s_attribute_0;\n"
			"layout(location = 1) in vec4 s_attribute_1;\n"
			"layout(location = 2) in vec4 s_attribute_2;\n"
			"layout(location = 0) uniform mat4 s_transform;\n"
			"out vec4 s_texcoord;\n"
			"out vec4 s_color;\n"
			"void main() {\n"
			"	gl_Position = s_transform * s_attribute_0;\n"
			"	s_texcoord = s_attribute_1;\n"
			"	s_color = s_attribute_2;\n"
			"}\n";
//#else
//		static const char vertex_shader_src[] =
//			"#version 330\n"
//			"layout(location = 0) in vec4 s_attribute_0;\n"
//			"layout(location = 1) in vec4 s_attribute_1;\n"
//			"layout(location = 2) in vec4 s_attribute_2;\n"
//			"layout(location = 3) in vec4 s_attribute_3;\n"
//			"layout(location = 4) in vec4 s_attribute_4;\n"
//			"layout(location = 5) in vec4 s_attribute_5;\n"
//			"layout(location = 6) in vec4 s_attribute_6;\n"
//			"out vec4 s_texcoord;\n"
//			"out vec4 s_color;\n"
//			"void main() {\n"
//			"	gl_Position.x = dot(s_attribute_3,s_attribute_0);\n"
//			"	gl_Position.y = dot(s_attribute_4,s_attribute_0);\n"
//			"	gl_Position.z = dot(s_attribute_5,s_attribute_0);\n"
//			"	gl_Position.w = dot(s_attribute_6,s_attribute_0);\n"
//			"	s_texcoord = s_attribute_1;\n"
//			"	s_color = s_attribute_2;\n"
//			"}\n";
//#endif

		static const char fragment_shader_default_src[] =
			"#version 330\n"
			"uniform sampler2D s_texture_0;\n"
			"in vec4 s_texcoord;\n"
			"in vec4 s_color;\n"
			"out vec4 s_frag_data_0;\n"
			"out vec4 s_frag_data_1;\n"
			"void main() {\n"
			"	s_frag_data_0 = texture(s_texture_0,s_texcoord.xy) * s_color;\n"
			"	s_frag_data_1 = s_frag_data_0;\n"
			"}\n";

		static const char fragment_shader_multisample_src[] =
			"#version 330\n"
			"uniform sampler2DMS s_texture_0;\n"
			"in vec4 s_texcoord;\n"
			"in vec4 s_color;\n"
			"out vec4 s_frag_data_0;\n"
			"out vec4 s_frag_data_1;\n"
			"void main() {\n"
			"	s_frag_data_0 = texelFetch(s_texture_0,ivec2(vec2(textureSize(s_texture_0)) * s_texcoord.xy),0) * s_color;\n"
			"	s_frag_data_1 = s_frag_data_0;\n"
			"}\n";

		static const char fragment_shader_shadow_src[] =
			"#version 330\n"
			"uniform sampler2DShadow s_texture_0;\n"
			"in vec4 s_texcoord;\n"
			"in vec4 s_color;\n"
			"out vec4 s_frag_data_0;\n"
			"out vec4 s_frag_data_1;\n"
			"void main() {\n"
			"	s_frag_data_0 = texture(s_texture_0,vec3(s_texcoord.xy,0.99)) * s_color;\n"
			"	s_frag_data_1 = s_frag_data_0;\n"
			"}\n";

		static const char fragment_shader_splash_src[] =
			"#version 330\n"
			"uniform sampler2D s_texture_0;\n"
			"in vec4 s_texcoord;\n"
			"in vec4 s_color;\n"
			"out vec4 s_frag_data_0;\n"
			"out vec4 s_frag_data_1;\n"
			"void main() {\n"
			"	vec4 color_0 = texture(s_texture_0,s_texcoord.xy + vec2(0.0,0.0));\n"
			"	vec4 color_1 = texture(s_texture_0,s_texcoord.xy + vec2(0.0,0.5));\n"
			"	float weight = clamp(color_1.w * s_texcoord.z + s_texcoord.w,0.0,1.0);\n"
			"	s_frag_data_0 = vec4(mix(color_1.xyz,color_0.xyz,weight),color_0.w) * s_color;\n"
			"	s_frag_data_1 = s_frag_data_0;\n"
			"}\n";

		static const char fragment_shader_solid_src[] =
			"#version 330\n"
			"in vec4 s_color;\n"
			"out vec4 s_frag_data_0;\n"
			"out vec4 s_frag_data_1;\n"
			"void main() {\n"
			"	s_frag_data_0 = s_color;\n"
			"	s_frag_data_1 = s_color;\n"
			"}\n";

		static const char fragment_shader_cube_src[] =
			"#version 330\n"
			"uniform samplerCube s_texture_0;\n"
			"in vec4 s_texcoord;\n"
			"in vec4 s_color;\n"
			"out vec4 s_frag_data_0;\n"
			"out vec4 s_frag_data_1;\n"
			"void main() {\n"
			"	s_frag_data_0 = texture(s_texture_0,s_texcoord.xyz) * s_color;\n"
			"	s_frag_data_1 = s_frag_data_0;\n"
			"}\n";

		static const char fragment_shader_grad_src[] =
			"#version 330\n"
			"uniform sampler2D s_texture_0;\n"
			"in vec4 s_texcoord;\n"
			"in vec4 s_color;\n"
			"out vec4 s_frag_data_0;\n"
			"out vec4 s_frag_data_1;\n"
			"void main() {\n"
			"   float radius = length(s_texcoord.xy - vec2(s_texcoord.z,0.5)) * 2.0;\n"
			"   s_frag_data_0 = texture(s_texture_0,vec2(radius,0.5)) * s_color;\n"
			"   s_frag_data_1 = s_frag_data_0;\n"
			"}\n";

		static const char fragment_shader_srgb_src[] =
			"#version 330\n"
			"uniform sampler2D s_texture_0;\n"
			"in vec4 s_texcoord;\n"
			"in vec4 s_color;\n"
			"out vec4 s_frag_data_0;\n"
			"out vec4 s_frag_data_1;\n"
			"void main() {\n"
			"	vec4 color = texture(s_texture_0,s_texcoord.xy);\n"
			"	color.xyz = pow(color.xyz,vec3(1.0 / 2.2));\n"
			"	s_frag_data_0 = color * s_color;\n"
			"	s_frag_data_1 = s_frag_data_0;\n"
			"}\n";

		static const char fragment_shader_yuv_src[] =
			"#version 330\n"
			"uniform sampler2D s_texture_0;\n"
			"in vec4 s_texcoord;\n"
			"in vec4 s_color;\n"
			"out vec4 s_frag_data_0;\n"
			"out vec4 s_frag_data_1;\n"
			"void main() {\n"
			"	vec4 yuv = texture(s_texture_0,s_texcoord.xy);\n"
			"	vec3 color = vec3(1.596 * yuv.z - 0.872,0.534 - 0.813 * yuv.z - 0.392 * yuv.y,2.017 * yuv.y - 1.083) + 1.164 * yuv.x;\n"
			"	s_frag_data_0 = vec4(color,yuv.w) * s_color;\n"
			"	s_frag_data_1 = s_frag_data_0;\n"
			"}\n";

		// create shaders
		GLuint vertex_shader = create_shader(GL_VERTEX_SHADER, vertex_shader_src);
		GLuint fragment_shader_default = create_shader(GL_FRAGMENT_SHADER, fragment_shader_default_src);
		GLuint fragment_shader_multisample = create_shader(GL_FRAGMENT_SHADER, fragment_shader_multisample_src);
		GLuint fragment_shader_shadow = create_shader(GL_FRAGMENT_SHADER, fragment_shader_shadow_src);
		GLuint fragment_shader_splash = create_shader(GL_FRAGMENT_SHADER, fragment_shader_splash_src);
		GLuint fragment_shader_solid = create_shader(GL_FRAGMENT_SHADER, fragment_shader_solid_src);
		GLuint fragment_shader_cube = create_shader(GL_FRAGMENT_SHADER, fragment_shader_cube_src);
		GLuint fragment_shader_grad = create_shader(GL_FRAGMENT_SHADER, fragment_shader_grad_src);
		GLuint fragment_shader_srgb = create_shader(GL_FRAGMENT_SHADER, fragment_shader_srgb_src);
		GLuint fragment_shader_yuv = create_shader(GL_FRAGMENT_SHADER, fragment_shader_yuv_src);

		// create programs
		default_program_id = create_program(vertex_shader, fragment_shader_default);
		multisample_program_id = create_program(vertex_shader, fragment_shader_multisample);
		shadow_program_id = create_program(vertex_shader, fragment_shader_shadow);
		splash_program_id = create_program(vertex_shader, fragment_shader_splash);
		solid_program_id = create_program(vertex_shader, fragment_shader_solid);
		cube_program_id = create_program(vertex_shader, fragment_shader_cube);
		grad_program_id = create_program(vertex_shader, fragment_shader_grad);
		srgb_program_id = create_program(vertex_shader, fragment_shader_srgb);
		yuv_program_id = create_program(vertex_shader, fragment_shader_yuv);

		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader_default);
		glDeleteShader(fragment_shader_multisample);
		glDeleteShader(fragment_shader_shadow);
		glDeleteShader(fragment_shader_splash);
		glDeleteShader(fragment_shader_solid);
		glDeleteShader(fragment_shader_cube);
		glDeleteShader(fragment_shader_grad);
		glDeleteShader(fragment_shader_srgb);
		glDeleteShader(fragment_shader_yuv);
	}

	void GLFfp::create_buffers() {

		// create vertices vbo
		num_vertex = GL_FFP_BUFFER_SIZE;
		glGenBuffers(1, &vertex_vbo_id);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo_id);
//#ifdef USE_OPENGL_44
		glBufferStorage(GL_ARRAY_BUFFER, sizeof(Vertex) * num_vertex, NULL, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | GL_DYNAMIC_STORAGE_BIT);
		vertex_ptr = (unsigned char*)glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * num_vertex, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
		if (vertex_ptr == NULL) Error("GLFfp::create_buffers(): can't map vertices buffer\n", true);
//#else
//		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * num_vertex, NULL, GL_STREAM_DRAW);
//#endif

		// create indices vbo
		num_indices = GL_FFP_BUFFER_SIZE;
		glGenBuffers(1, &indices_vbo_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_vbo_id);
//#ifdef USE_OPENGL_44
		glBufferStorage(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * num_indices, NULL, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | GL_DYNAMIC_STORAGE_BIT);
		indices_ptr = (unsigned char*)glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(unsigned short) * num_indices, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
		if (indices_ptr == NULL) Error("GLFfp::create_buffers(): can't map indices buffer\n", true);
//#else
//		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * num_indices, NULL, GL_STREAM_DRAW);
//#endif

		// create vertices vao
		glGenVertexArrays(1, &vertex_vao_id);
		update_vertex_array();
		glBindVertexArray(0);

		// create screen vertices vbo
		const Vertex screen_vertex[] = {
			{ { -1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 0.0f }, { { 255, 255, 255, 255 } } },
			{ { 3.0f, -1.0f, 0.0f }, { 2.0f, 0.0f, 0.0f, 0.0f }, { { 255, 255, 255, 255 } } },
			{ { -1.0f, 3.0f, 0.0f }, { 0.0f, 2.0f, 0.0f, 0.0f }, { { 255, 255, 255, 255 } } },
		};

		glGenBuffers(1, &screen_vertex_vbo_id);
		glBindBuffer(GL_ARRAY_BUFFER, screen_vertex_vbo_id);
//#ifdef USE_OPENGL_44
		glBufferStorage(GL_ARRAY_BUFFER, sizeof(screen_vertex), screen_vertex, 0);
//#else
//		glBufferData(GL_ARRAY_BUFFER, sizeof(screen_vertex), screen_vertex, GL_STATIC_DRAW);
//#endif

		// create screen vertices vao
		glGenVertexArrays(1, &screen_vertex_vao_id);
		glBindVertexArray(screen_vertex_vao_id);
		glBindBuffer(GL_ARRAY_BUFFER, screen_vertex_vbo_id);
		glVertexAttribPointer(0, 3, GL_FLOAT, 0, sizeof(Vertex), (void*)(0));
		glVertexAttribPointer(1, 4, GL_FLOAT, 0, sizeof(Vertex), (void*)(12));
		glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, 1, sizeof(Vertex), (void*)(28));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void GLFfp::update_vertex_array() const {

		// bind buffers
		glBindVertexArray(vertex_vao_id);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_vbo_id);
		glVertexAttribPointer(0, 3, GL_FLOAT, 0, sizeof(Vertex), (void*)(0));
		glVertexAttribPointer(1, 4, GL_FLOAT, 0, sizeof(Vertex), (void*)(12));
		glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, 1, sizeof(Vertex), (void*)(28));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
	}

	/*
	 */
	void GLFfp::setMode(int m) {
		if (enabled) {
			disable();
			enable(m);
//#ifdef USE_OPENGL_44
			glUniformMatrix4fv(0, 1, GL_FALSE, transform);
//#endif
		}
		else {
			mode = m;
		}
	}

	int GLFfp::getMode() const {
		return mode;
	}

	/*
	 */
	int GLFfp::isEnabled() const {
		return enabled;
	}

	void GLFfp::enable(int m) {

		assert(enabled == 0 && "GLFfp::enable(): is already enabled");

		mode = m;
		enabled = 1;

		if (default_program_id == 0) create_programs();

		if (vertex_vao_id == 0) create_buffers();

		if (mode == MODE_DEFAULT) GLExt::setProgramID(default_program_id);
		else if (mode == MODE_MULTISAMPLE_2) GLExt::setProgramID(multisample_program_id);
		else if (mode == MODE_MULTISAMPLE_4) GLExt::setProgramID(multisample_program_id);
		else if (mode == MODE_MULTISAMPLE_8) GLExt::setProgramID(multisample_program_id);
		else if (mode == MODE_MULTISAMPLE_16) GLExt::setProgramID(multisample_program_id);
		else if (mode == MODE_SHADOW) GLExt::setProgramID(shadow_program_id);
		else if (mode == MODE_SPLASH) GLExt::setProgramID(splash_program_id);
		else if (mode == MODE_SOLID) GLExt::setProgramID(solid_program_id);
		else if (mode == MODE_CUBE) GLExt::setProgramID(cube_program_id);
		else if (mode == MODE_GRAD) GLExt::setProgramID(grad_program_id);
		else if (mode == MODE_SRGB) GLExt::setProgramID(srgb_program_id);
		else if (mode == MODE_YUV) GLExt::setProgramID(yuv_program_id);
		else assert(0 && "GLFfp::enable(): unknown mode");

		glBindVertexArray(vertex_vao_id);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_vbo_id);
	}

	void GLFfp::disable() {

		assert(enabled && "GLFfp::disable(): is not enabled");

		mode = -1;
		enabled = 0;

		glBindVertexArray(0);

		GLExt::setProgramID(0);
	}

	/*
	 */
	void GLFfp::setOrtho(int width, int height) {
		transform.Zero();
		transform.e[0] = 2.0f / width; transform.e[12] = -1.0f;
		transform.e[5] = -2.0f / height; transform.e[13] = 1.0f;
		transform.e[15] = 1.0f;
		if (enabled) {
//#ifdef USE_OPENGL_44
			glUniformMatrix4fv(0, 1, GL_FALSE, transform);
//#else
//			glVertexAttrib4f(3, transform.m00, transform.m01, transform.m02, transform.m03);
//			glVertexAttrib4f(4, transform.m10, transform.m11, transform.m12, transform.m13);
//			glVertexAttrib4f(5, transform.m20, transform.m21, transform.m22, transform.m23);
//			glVertexAttrib4f(6, transform.m30, transform.m31, transform.m32, transform.m33);
//#endif
		}
	}

	void GLFfp::setTransform(const Mat4 &t) {
		transform = t;
		if (enabled) {
//#ifdef USE_OPENGL_44
			glUniformMatrix4fv(0, 1, GL_FALSE, transform);
//#else
//			glVertexAttrib4f(3, transform.m00, transform.m01, transform.m02, transform.m03);
//			glVertexAttrib4f(4, transform.m10, transform.m11, transform.m12, transform.m13);
//			glVertexAttrib4f(5, transform.m20, transform.m21, transform.m22, transform.m23);
//			glVertexAttrib4f(6, transform.m30, transform.m31, transform.m32, transform.m33);
//#endif
		}
	}

	const Mat4 &GLFfp::getTransform() const {
		return transform;
	}

	/*
	 */
	void GLFfp::render(Vertex *vertex, int vertex_size, unsigned short *indices, int indices_size) {

		if (vertex_vao_id == 0) create_buffers();

		if (enabled == 0) {
			glBindVertexArray(vertex_vao_id);
			glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo_id);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_vbo_id);
		}
#pragma message("Расскоментируй это")
		// per-frame buffers
		/*if (frame != App::get()->getFrame()) {
			frame = App::get()->getFrame();
			num_frame_vertex = vertex_size;
			num_frame_indices = indices_size;
		}
		else*/ {
			num_frame_vertex += vertex_size;
			num_frame_indices += indices_size;
		}

		/////////////////////////////////
		// vertices buffer
		/////////////////////////////////

//#ifdef USE_OPENGL_44

		// resize vertices buffer
		if (num_vertex < num_frame_vertex * 3) {
			num_vertex = max(num_vertex * 2, num_frame_vertex * 3);

			// synchronization
			GLExt::waitSync(vertex_sync);

			// delete vertices vbo
			glDeleteBuffers(1, &vertex_vbo_id);

			// create vertices vbo
			glGenBuffers(1, &vertex_vbo_id);
			glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo_id);
			glBufferStorage(GL_ARRAY_BUFFER, sizeof(Vertex) * num_vertex, NULL, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | GL_DYNAMIC_STORAGE_BIT);
			vertex_ptr = (unsigned char*)glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * num_vertex, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
			if (vertex_ptr == NULL) Error("GLFfp::render(): can't map vertices buffer\n", true);
			vertex_offset = 0;

			// update vertices vao
			update_vertex_array();
		}

		// synchronization
		GLExt::waitSync(vertex_sync, vertex_offset, vertex_size, num_vertex);

		// copy vertices
		/*Math::*/memcpy(vertex_ptr + sizeof(Vertex) * vertex_offset, vertex, sizeof(Vertex) * vertex_size);

//#else
//
//		// resize vertices buffer
//		if (num_vertex < num_frame_vertex) {
//			num_vertex = num_frame_vertex * 2;
//			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * num_vertex, NULL, GL_STREAM_DRAW);
//			vertex_offset = 0;
//		}
//
//		// overflow of vertices
//		if (vertex_offset + vertex_size > num_vertex) {
//			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * num_vertex, NULL, GL_STREAM_DRAW);
//			vertex_offset = 0;
//		}
//
//		// update vertices buffer
//		glBufferSubData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertex_offset, sizeof(Vertex) * vertex_size, vertex);
//
//#endif

		/////////////////////////////////
		// indices buffer
		/////////////////////////////////

//#ifdef USE_OPENGL_44

		// resize indices buffer
		if (num_indices < num_frame_indices * 3) {
			num_indices = max(num_indices * 2, num_frame_indices * 3);

			// synchronization
			GLExt::waitSync(indices_sync);

			// delete indices vbo
			glDeleteBuffers(1, &indices_vbo_id);

			// create indices vbo
			glGenBuffers(1, &indices_vbo_id);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_vbo_id);
			glBufferStorage(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * num_indices, NULL, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | GL_DYNAMIC_STORAGE_BIT);
			indices_ptr = (unsigned char*)glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(unsigned short) * num_indices, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
			if (indices_ptr == NULL) Error("GLExt::render(): can't map indices buffer\n", true);
			indices_offset = 0;
		}

		// synchronization
		GLExt::waitSync(indices_sync, indices_offset, indices_size, num_indices);

		// copy indices
		/*Math::*/memcpy(indices_ptr + sizeof(unsigned short) * indices_offset, indices, sizeof(unsigned short) * indices_size);

//#else
//
//		// resize indices buffer
//		if (num_indices < num_frame_indices) {
//			num_indices = num_frame_indices * 2;
//			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * num_indices, NULL, GL_STREAM_DRAW);
//			indices_offset = 0;
//		}
//
//		// overflow of indices
//		if (indices_offset + indices_size > num_indices) {
//			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * num_indices, NULL, GL_STREAM_DRAW);
//			indices_offset = 0;
//		}
//
//		// update indices buffer
//		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * indices_offset, sizeof(unsigned short) * indices_size, indices);
//
//#endif
	}

	void GLFfp::render_lines(Vertex *vertex, int vertex_size, unsigned short *indices, int indices_size) {

		if (vertex_size == 0 || indices_size == 0) return;

		render(vertex, vertex_size, indices, indices_size);

		glDrawElementsBaseVertex(GL_LINES, indices_size, GL_UNSIGNED_SHORT, (void*)(sizeof(unsigned short) * indices_offset), vertex_offset);

//#ifdef USE_OPENGL_44
		GLExt::fenceSync(vertex_sync, vertex_offset, vertex_size, num_vertex);
		GLExt::fenceSync(indices_sync, indices_offset, indices_size, num_indices);
//#endif

		vertex_offset += vertex_size;
		indices_offset += indices_size;

		if (enabled == 0) glBindVertexArray(0);
	}

	void GLFfp::render_triangles(Vertex *vertex, int vertex_size, unsigned short *indices, int indices_size) {

		if (vertex_size == 0 || indices_size == 0) return;

		render(vertex, vertex_size, indices, indices_size);

		glDrawElementsBaseVertex(GL_TRIANGLES, indices_size, GL_UNSIGNED_SHORT, (void*)(sizeof(unsigned short) * indices_offset), vertex_offset);

//#ifdef USE_OPENGL_44
		GLExt::fenceSync(vertex_sync, vertex_offset, vertex_size, num_vertex);
		GLExt::fenceSync(indices_sync, indices_offset, indices_size, num_indices);
//#endif

		vertex_offset += vertex_size;
		indices_offset += indices_size;

		if (enabled == 0) glBindVertexArray(0);
	}

	void GLFfp::render_screen() {

		if (vertex_vao_id == 0) create_buffers();

		glBindVertexArray(screen_vertex_vao_id);
		glBindBuffer(GL_ARRAY_BUFFER, screen_vertex_vbo_id);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		if (enabled) {
			glBindVertexArray(vertex_vao_id);
			glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo_id);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_vbo_id);
		}
		else {
			glBindVertexArray(0);
		}
	}
}