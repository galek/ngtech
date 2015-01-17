//
// Drake3D engine version ${DK_ENGINE_VERSION}
// Copyright (C) 2011-2014 Kirill Bazhenov
//
// This copy is licensed to the following parties:
//
//       User:            ${REGISTERED_USER_NAME}
//       License:         ${REGISTERED_USER_LICENSE}
//       Number of users: ${REGISTERED_USER_MAX}
//
// License granted under the terms of the license agreement
// entered by the registered party.
//
// Unauthorized redistribution of this source code is
// strictly prohibited. Violators will be prosecuted.
//
#include "RenderPrivate.h"
//***************************************************************************
#include "GLSystem.h"
//***************************************************************************
#include "meshobject.hh"

namespace NGTech
{

	MeshObject::MeshObject()
	{
		glGenVertexArrays(1, &_id);
		GetIndexBuffer().SetType(GLVBO::BUF_INDEX);
		GetVertexBuffer().SetType(GLVBO::BUF_VERTEX);
	}

	MeshObject::~MeshObject()
	{
		glDeleteVertexArrays(1, &_id);
	}

	void MeshObject::Bind() const
	{
		glBindVertexArray(GetID());
	}

	void MeshObject::Unbind() const
	{
		glBindVertexArray(0);
	}

	void MeshObject::SetVertexLayout(const VertexLayout& layout)
	{
		glBindVertexArray(_id);

		glBindBuffer(static_cast<GLenum>(GetIndexBuffer().GetType()), GetIndexBuffer().GetID());
		glBindBuffer(static_cast<GLenum>(GetVertexBuffer().GetType()), GetVertexBuffer().GetID());

		for (size_t i = 0; i != layout.attributes.size(); ++i) {
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(
				i,
				layout.attributes[i].size,
				static_cast<GLenum>(layout.attributes[i].dataType),
				layout.attributes[i].normalized,
				layout.stride,
				((char*)0) + layout.attributes[i].offset
				);
		}

		_vertexLayout = layout;
		glBindVertexArray(0);
	}

}