#pragma once

namespace NGTech
{
	enum class DataType
	{
		NoIndex = 0,
		Int = GL_INT,
		UInt = GL_UNSIGNED_INT,
		Byte = GL_BYTE,
		UByte = GL_UNSIGNED_BYTE,
		Float = GL_FLOAT
	};

	enum class MeshPrimitiveType
	{
		Triangles = GL_TRIANGLES,
		Lines = GL_LINES,
		Points = GL_POINTS,
		TriangleStrip = GL_TRIANGLE_STRIP,
		LineStrip = GL_LINE_STRIP,
		Patches = GL_PATCHES
	};

	enum class RenderTargetSlot
	{
		Color0 = GL_COLOR_ATTACHMENT0,
		Color1 = GL_COLOR_ATTACHMENT1,
		Color2 = GL_COLOR_ATTACHMENT2,
		Color3 = GL_COLOR_ATTACHMENT3,
		Color4 = GL_COLOR_ATTACHMENT4,
		Color5 = GL_COLOR_ATTACHMENT5,
		Color6 = GL_COLOR_ATTACHMENT6,
		Color7 = GL_COLOR_ATTACHMENT7,
		Depth = GL_DEPTH_ATTACHMENT,
		Stencil = GL_STENCIL_ATTACHMENT
	};

	enum class BufferType
	{
		Index = GL_ELEMENT_ARRAY_BUFFER,
		Vertex = GL_ARRAY_BUFFER,
		Constant = GL_UNIFORM_BUFFER
	};

	enum class BufferUsage
	{
		Static = GL_STATIC_DRAW,
		Dynamic = GL_DYNAMIC_DRAW,
		Stream = GL_STREAM_DRAW
	};

	enum class BufferMap
	{
		ReadOnly = GL_READ_ONLY,
		WriteOnly = GL_WRITE_ONLY,
		ReadWrite = GL_READ_WRITE
	};

	struct VertexLayout
	{
		struct Attribute
		{
			size_t   size;
			size_t   offset;
			bool     normalized;
			DataType dataType;

			Attribute(size_t newSize, size_t newOffset, bool newNormalized, DataType newDataType)
				: size(newSize), offset(newOffset), normalized(newNormalized), dataType(newDataType)
			{}

			Attribute(const Attribute& other)
				: size(other.size), offset(other.offset), normalized(other.normalized), dataType(other.dataType)
			{}
		};

		size_t stride;

		std::vector<Attribute> attributes;
	};
}