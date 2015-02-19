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

#ifndef __GL_FFP_H__
#define __GL_FFP_H__

#include "I_Ffp.h"

#include "GLExtensions.h"

namespace NGTech
{
	/*
	 */
	class GLFfp : public I_FFP {

	public:

		GLFfp();
		virtual ~GLFfp();

		// current mode
		virtual void setMode(int mode);
		virtual int getMode() const;

		// enable/disable ffp
		virtual int isEnabled() const;
		virtual void enable(int mode);
		virtual void disable();

		// transformation
		virtual void setOrtho(int width, int height);
		virtual void setTransform(const Mat4 &transform);
		virtual const Mat4 &getTransform() const;

	private:

		unsigned int create_shader(unsigned int type, const char *src);
		unsigned int create_program(unsigned int vertex_shader, unsigned int fragment_shader);

		void create_programs();
		void create_buffers();
		void update_vertex_array() const;

		// renders
		void render(Vertex *vertex, int num_vertex, unsigned short *indices, int num_indices);
		virtual void render_lines(Vertex *vertex, int num_vertex, unsigned short *indices, int num_indices);
		virtual void render_triangles(Vertex *vertex, int num_vertex, unsigned short *indices, int num_indices);
		virtual void render_screen();

		int mode;
		int enabled;

		int frame;

		Mat4 transform;

		unsigned int default_program_id;
		unsigned int multisample_program_id;
		unsigned int shadow_program_id;
		unsigned int splash_program_id;
		unsigned int solid_program_id;
		unsigned int cube_program_id;
		unsigned int grad_program_id;
		unsigned int srgb_program_id;
		unsigned int yuv_program_id;

		int num_vertex;
		int vertex_offset;
		int num_frame_vertex;
		unsigned int vertex_vao_id;
		unsigned int vertex_vbo_id;
//#ifdef USE_OPENGL_44
		unsigned char *vertex_ptr;
		GLsync vertex_sync[3];
//#endif

		int num_indices;
		int indices_offset;
		int num_frame_indices;
		unsigned int indices_vbo_id;
//#ifdef USE_OPENGL_44
		unsigned char *indices_ptr;
		GLsync indices_sync[3];
//#endif

		unsigned int screen_vertex_vao_id;
		unsigned int screen_vertex_vbo_id;
	};
}
#endif /* __GL_FFP_H__ */