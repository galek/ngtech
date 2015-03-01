//**************************************
#include "CorePrivate.h"
//**************************************
#include "MeshFormatNGGFSkinned.h"
#include "SkinnedMesh.h"
//**************************************

namespace NGTech
{
	/**
	*/
#define NAME_SIZE 64

	/**
	*/
	void ReadName(char *buffer, std::string &name, VFile *file);

	/*
	SkinnedMesh
	*/
#define SKINNED_MESH_HEADER ('x' | 's' << 8 | 's' << 16 | 'm' << 24)

	/**
	*/
	MeshFormatNGGFSkinned::MeshFormatNGGFSkinned()
	{
	}

	/**
	*/
	MeshFormatNGGFSkinned::~MeshFormatNGGFSkinned()
	{
	}

	/**
	*/
	bool MeshFormatNGGFSkinned::Load(const String &path, SkinnedMesh *mesh)
	{
		//begin loading
		VFile file(path.c_str());

		if (!file.IsValid())
		{
			Warning("File is not valid");
			return false;
		}

		//buffer
		char nameBuffer[NAME_SIZE];

		//header
		unsigned int header;
		file.Read(&header, sizeof(unsigned int), 1);

		if (header != SKINNED_MESH_HEADER)
			Error(std::string("MeshLoader::LoadXSSMSH() error: mesh file '" + path + "' has invalid header"), true);

		//num_bones
		file.Read(&mesh->numBones, sizeof(unsigned int), 1);
		mesh->bones = new SkinnedMesh::Bone[mesh->numBones];

		for (int i = 0; i < mesh->numBones; i++)
		{
			ReadName(nameBuffer, mesh->bones[i].name, &file);
			file.Read(&mesh->bones[i].parent, sizeof(unsigned int), 1);
		}

		//num_subsets
		int numSubsets = 0;
		file.Read(&numSubsets, sizeof(unsigned int), 1);
		mesh->subsets = new SkinnedMesh::Subset*[numSubsets];
		mesh->SetNumSubsets(numSubsets);

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
		return true;
	}

	/**
	*/
	void MeshFormatNGGFSkinned::Save(const String &path, SkinnedMesh *mesh)
	{
	}

}