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

#pragma once

#include "CommonDefs.h"

#include <vector>

namespace NGTech
{

	class MeshObject
	{
	private:

		MeshObject(const MeshObject& other) = delete;
		MeshObject& operator=(const MeshObject& other) = delete;

		unsigned int         _id;
		GLVBO IndB;
		GLVBO VerB;
		VertexLayout _vertexLayout;

		MeshPrimitiveType _primitiveType = MeshPrimitiveType::Triangles;

	public:

		MeshObject();
		~MeshObject();

		void Bind() const;
		void Unbind() const;

		inline unsigned int GetID() const { return _id; }

		inline GLVBO & GetIndexBuffer() { return IndB; }
		inline const GLVBO & GetIndexBuffer() const { return IndB; }

		inline GLVBO& GetVertexBuffer() { return VerB; }
		inline const GLVBO& GetVertexBuffer() const { return VerB; }

		void SetVertexLayout(const VertexLayout& layout);
		inline const VertexLayout& GetVertexLayout() const { return _vertexLayout; }

		inline void SetPrimitiveType(MeshPrimitiveType newType) { _primitiveType = newType; }
		inline MeshPrimitiveType GetPrimitiveType() const { return _primitiveType; }
	};

}
