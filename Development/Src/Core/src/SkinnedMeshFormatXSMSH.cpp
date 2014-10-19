//**************************************
#include "CorePrivate.h"
//**************************************
#include "SkinnedMeshFormatXSSMSH.h"
#include "SkinnedMesh.h"
//**************************************

namespace NGTech
{

#define NAME_SIZE 64

	static void ReadName(char *buffer, std::string &name, VFile *file)
	{
		memset(buffer, 0, NAME_SIZE);
		file->Read(buffer, NAME_SIZE, 1);
		name = buffer;
		Debug((String("Loading material: ") + buffer).c_str());
	}

	/*
	SkinnedMesh
	*/
#define SKINNED_MESH_HEADER ('x' | 's' << 8 | 's' << 16 | 'm' << 24)

	/*
	*/
	SkinnedMeshFormatXSSMSH::SkinnedMeshFormatXSSMSH()
	{
	}

	/*
	*/
	SkinnedMeshFormatXSSMSH::~SkinnedMeshFormatXSSMSH()
	{
	}

	/*
	*/
	void SkinnedMeshFormatXSSMSH::Load(const String &path, SkinnedMesh *mesh)
	{
		//begin loading
		VFile file(path.c_str());

		//buffer
		char nameBuffer[NAME_SIZE];

		//header
		unsigned int header;
		file.Read(&header, sizeof(unsigned int), 1);

		if (header != SKINNED_MESH_HEADER)
		{
			Error(std::string("MeshLoader::LoadXSSMSH() error: mesh file '" + path + "' has invalid header"), true);
			return;
		}

		//num_bones
		file.Read(&mesh->numBones, sizeof(unsigned int), 1);
		mesh->bones = new SkinnedMesh::Bone[mesh->numBones];

		for (int i = 0; i < mesh->numBones; i++)
		{
			ReadName(nameBuffer, mesh->bones[i].name, &file);
			file.Read(&mesh->bones[i].parent, sizeof(unsigned int), 1);
		}

		//num_subsets
		file.Read(&mesh->numSubsets, sizeof(unsigned int), 1);
		mesh->subsets = new SkinnedMesh::Subset*[mesh->numSubsets];

		//process subsets
		for (int s = 0; s < mesh->numSubsets; s++)
		{
			mesh->subsets[s] = new SkinnedMesh::Subset();
			SkinnedMesh::Subset *st = mesh->subsets[s];

			//read the surface name
			ReadName(nameBuffer, st->name, &file);

			//number of vertices
			file.Read(&st->numVertices, sizeof(unsigned int), 1);
			st->vertices = new SkinnedMesh::Vertex[st->numVertices];

			//process vertices
			for (unsigned int v = 0; v < st->numVertices; v++)
			{
				SkinnedMesh::Vertex &vert = st->vertices[v];

				file.Read(&vert.numWeights, sizeof(unsigned int), 1);
				vert.weights = new SkinnedMesh::Weight[vert.numWeights];

				for (int w = 0; w < vert.numWeights; w++)
				{
					SkinnedMesh::Weight &wt = vert.weights[w];

					file.Read(&wt.bone, sizeof(unsigned int), 1);
					file.Read(&wt.weight, sizeof(float), 1);
					file.Read(&wt.position.x, sizeof(float), 1);
					file.Read(&wt.position.y, sizeof(float), 1);
					file.Read(&wt.position.z, sizeof(float), 1);
					file.Read(&wt.normal.x, sizeof(float), 1);
					file.Read(&wt.normal.y, sizeof(float), 1);
					file.Read(&wt.normal.z, sizeof(float), 1);
				}
				file.Read(&vert.texcoord.x, sizeof(float), 1);
				file.Read(&vert.texcoord.y, sizeof(float), 1);
			}

			//number of faces
			file.Read(&st->numIndices, sizeof(unsigned int), 1);
			st->indices = new unsigned int[st->numIndices];

			//process faces
			for (unsigned int i = 0; i < st->numIndices; i++)
			{
				file.Read(&st->indices[i], sizeof(unsigned int), 1);
			}
		}

		//num frames
		file.Read(&mesh->numFrames, sizeof(unsigned int), 1);
		mesh->frames = new SkinnedMesh::Frame*[mesh->numFrames];

		//frames
		for (int i = 0; i < mesh->numFrames; i++)
		{
			mesh->frames[i] = new SkinnedMesh::Frame[mesh->numBones];

			for (int k = 0; k < mesh->numBones; k++)
			{
				file.Read(&mesh->frames[i][k].position.x, sizeof(float), 1);
				file.Read(&mesh->frames[i][k].position.y, sizeof(float), 1);
				file.Read(&mesh->frames[i][k].position.z, sizeof(float), 1);
				file.Read(&mesh->frames[i][k].rotation.x, sizeof(float), 1);
				file.Read(&mesh->frames[i][k].rotation.y, sizeof(float), 1);
				file.Read(&mesh->frames[i][k].rotation.z, sizeof(float), 1);
				file.Read(&mesh->frames[i][k].rotation.w, sizeof(float), 1);
			}
		}
	}

	/*
	*/
	void SkinnedMeshFormatXSSMSH::Save(const String &path, SkinnedMesh *mesh)
	{
	}

}