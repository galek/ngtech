/* Copyright (C) 2005-2014, Unigine Corp. All rights reserved.
 *
 * File:    Ffp.h
 * Desc:    Fixed function pipeline
 * Version: 1.11
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

#ifndef __FFP_H__
#define __FFP_H__

#include "MathLib.h"

namespace NGTech
{
	/*
	 */
	class I_FFP {

	public:

		I_FFP();
		virtual ~I_FFP();

		// return ffp
		static I_FFP *get();

		// ffp modes
		enum {
			MODE_DEFAULT = 0,
			MODE_MULTISAMPLE_2,
			MODE_MULTISAMPLE_4,
			MODE_MULTISAMPLE_8,
			MODE_MULTISAMPLE_16,
			MODE_SHADOW,
			MODE_SPLASH,
			MODE_SOLID,
			MODE_CUBE,
			MODE_GRAD,
			MODE_SRGB,
			MODE_YUV,
		};

		// ffp colors
		enum {
			COLOR_RED = 0xffff0000u,
			COLOR_GREEN = 0xff00ff00u,
			COLOR_BLUE = 0xff0000ffu,
			COLOR_BLACK = 0xff000000u,
			COLOR_WHITE = 0xffffffffu,
		};

		// current mode
		virtual void setMode(int mode) = 0;
		virtual int getMode() const = 0;

		// enable/disable ffp
		virtual int isEnabled() const = 0;
		virtual void enable(int mode = MODE_DEFAULT) = 0;
		virtual void disable() = 0;

		// transformation
		virtual void setOrtho(int width, int height) = 0;
		virtual void setTransform(const Mat4 &transform) = 0;
		virtual const Mat4 &getTransform() const = 0;

		// render lines
		int isLines() const;
		void beginLines();
		void endLines();

		// render triangles
		int isTriangles() const;
		void beginTriangles();
		void endTriangles();

		// render screen
		void renderScreen();

		ATTRIBUTE_ALIGNED4(struct) Vertex{
			float xyz[3];				// coordinate
			float texcoord[4];			// texture coordinates
			union {
				unsigned char color[4];	// color
				unsigned int color_vec;
			};
		};

		// vertices
		int getNumVertex() const;
		void reserveVertex(int num_vertex) const;
		void addVertex(const Vertex &vertex) const;
		void addVertexFast(const Vertex &vertex) const;
		void addVertex(const Vertex *vertex, int num_vertex) const;
		void addVertex(const float *xyz, int size) const;
		void addVertex(float x, float y, float z = 0.0f) const;
		void setTexCoord(const float *texcoord, int size) const;
		void setTexCoord(float x, float y, float z = 0.0f, float w = 1.0f) const;
		void setColor(const float *color, int size) const;
		void setColor(float r, float g, float b, float a) const;
		void setColor(const unsigned char *color, int size) const;
		void setColor(int r, int g, int b, int a) const;
		void setColor(unsigned int color) const;

		// indices
		int getNumIndices() const;
		void reserveIndices(int num_indices) const;
		void addIndex(int index) const;
		void addIndexFast(int index) const;
		void addIndices(int i0, int i1) const;
		void addIndices(int i0, int i1, int i2) const;
		void addIndices(int i0, int i1, int i2, int i3) const;
		void addIndices(const unsigned short *indices, int num_indices) const;
		void addIndices(const unsigned short *indices, int num_indices, int vertex_offset) const;
		void addLines(int num) const;
		void addTriangles(int num) const;
		void addTriangleQuads(int num) const;

	private:

		// renders
		virtual void render_lines(Vertex *vertex, int num_vertex, unsigned short *indices, int num_indices) = 0;
		virtual void render_triangles(Vertex *vertex, int num_vertex, unsigned short *indices, int num_indices) = 0;
		virtual void render_screen() = 0;
	};
}
#endif /* __FFP_H__ */