//**************************************
#include "CorePrivate.h"
//**************************************
#include "Model.h"
#include "MeshFormatXSMSH.h"
//**************************************

namespace NGTech
{

#define NAME_SIZE 64

	static void ReadName(char *buffer, std::string &name, VFile *file)
	{
		memset(buffer, 0, NAME_SIZE);
		file->Read(buffer, NAME_SIZE, 1);
		name = buffer;
		LogPrintf((String("Loading material: ") + buffer).c_str());
	}

	/**
	Mesh
	*/
#define MESH_HEADER ('x' | 's' << 8 | 'm' << 16 | 's' << 24)
#define MESH_HEADER_UNWRAPPED ('x' | 's' << 8 | 'm' << 16 | 'u' << 24)

	/**
	*/
	MeshFormatXSMSH::MeshFormatXSMSH()
	{
	}

	/**
	*/
	MeshFormatXSMSH::~MeshFormatXSMSH()
	{
	}

	/**
	*/
	void MeshFormatXSMSH::Load(const std::string &path, Model *mesh)
	{
		//begin loading
		VFile file(path.c_str());
		//buffer
		char nameBuffer[NAME_SIZE];

		//header
		unsigned int header;
		file.Read(&header, sizeof(unsigned int), 1);

		if (header != MESH_HEADER && header != MESH_HEADER_UNWRAPPED)
		{
			Error(std::string("MeshLoader::Load() error: mesh file '" + path + "' has invalid header"), false);
			return;
		}

		mesh->lmTexcoords = false;

		if (header == MESH_HEADER_UNWRAPPED)
		{
			Debug("MESH_HEADER_UNWRAPPED");
			mesh->lmTexcoords = true;
		}

		//num_subsets
		file.Read(&mesh->numSubsets, sizeof(unsigned int), 1);

		mesh->subsets = new Model::Subset*[mesh->numSubsets];

		//process subsets
		for (int s = 0; s < mesh->numSubsets; s++)
		{
			mesh->subsets[s] = new Model::Subset();
			Model::Subset *st = mesh->subsets[s];

			//read the surface name
			ReadName(nameBuffer, st->name, &file);

			//number of vertices
			file.Read(&st->numVertices, sizeof(unsigned int), 1);
			st->vertices = new Model::Vertex[st->numVertices];

			//process vertices
			for (unsigned int v = 0; v < st->numVertices; v++)
			{
				Model::Vertex &vert = st->vertices[v];
				file.Read(&vert.position.x, sizeof(float), 1);
				file.Read(&vert.position.y, sizeof(float), 1);
				file.Read(&vert.position.z, sizeof(float), 1);
				file.Read(&vert.normal.x, sizeof(float), 1);
				file.Read(&vert.normal.y, sizeof(float), 1);
				file.Read(&vert.normal.z, sizeof(float), 1);
				file.Read(&vert.texcoord.x, sizeof(float), 1);
				file.Read(&vert.texcoord.y, sizeof(float), 1);
				if (mesh->lmTexcoords)
				{
					file.Read(&vert.lmTexcoord.x, sizeof(float), 1);
					file.Read(&vert.lmTexcoord.y, sizeof(float), 1);
				}
			}

			//indices
			file.Read(&st->numIndices, sizeof(unsigned int), 1);
			st->indices = new unsigned int[st->numIndices];

			//process faces
			for (unsigned int i = 0; i < st->numIndices; i++)
			{
				file.Read(&st->indices[i], sizeof(unsigned int), 1);
			}
		}
	}

	/**
	*/
	void MeshFormatXSMSH::Save(const String &path, Model *mesh)
	{
	}

}