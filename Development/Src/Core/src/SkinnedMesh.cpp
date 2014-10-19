//**************************************
#include "CorePrivate.h"
//**************************************
#include "SkinnedMesh.h"
#include "SkinnedMeshLoader.h"
//**************************************

namespace NGTech
{

	/*
	*/
	SkinnedMesh::SkinnedMesh(const String &path)
	{
		Load(path);
	}

	/*
	*/
	SkinnedMesh::~SkinnedMesh()
	{
		for (int s = 0; s < numSubsets; s++)
		{
			Subset *st = subsets[s];

			delete[] st->vertices;
			delete[] st->indices;
			SAFE_DELETE(st);
		}

	}

	/*
	*/
	void SkinnedMesh::Load(const String &_path)
	{
		GetSkinnedMeshLoader()->Load(_path, this);

		path = _path;
		CalculateTBN();
		CalculateBoundings();
	}

	/*
	*/
	void SkinnedMesh::SetFrame(float frame, int from, int to)
	{
		if (numFrames == 0) return;

		if (from < 0) from = 0;
		if (to < 0) to = numFrames;

		int frame0 = (int)frame;
		frame -= frame0;
		frame0 += from;
		if (frame0 >= to) frame0 = (frame0 - from) % (to - from) + from;
		int frame1 = frame0 + 1;
		if (frame1 >= to) frame1 = from;

		for (unsigned int i = 0; i < numBones; i++)
		{
			// calculate matrixes
			Mat4 translate;
			translate = Mat4::translate(frames[frame0][i].position * (1.0f - frame) + frames[frame1][i].position * frame);

			Quat rot;
			rot = Quat::slerp(frames[frame0][i].rotation, frames[frame1][i].rotation, frame);

			bones[i].rotation = Mat4(rot.toMatrix());
			bones[i].transform = translate * bones[i].rotation;
		}

		for (unsigned int i = 0; i < numSubsets; i++)
		{
			// calculate vertexes
			Subset *st = subsets[i];

			st->bBox = BBox(st->vertices[0].position, st->vertices[0].position);

			for (unsigned int j = 0; j < st->numVertices; j++)
			{
				Vertex *v = &st->vertices[j];

				v->position = Vec3(0, 0, 0);
				v->normal = Vec3(0, 0, 0);
				v->tangent = Vec3(0, 0, 0);
				v->binormal = Vec3(0, 0, 0);

				for (int k = 0; k < v->numWeights; k++)
				{
					Weight *w = &v->weights[k];
					v->position += bones[w->bone].transform * w->position * w->weight;
					v->normal -= bones[w->bone].rotation * w->normal * w->weight;
					v->tangent += bones[w->bone].rotation * w->tangent * w->weight;
					v->binormal -= bones[w->bone].rotation * w->binormal * w->weight;
				}

				st->bBox.AddPoint(v->position);
			}

			st->bSphere = BSphere(st->bBox.GetCenter(), ((st->bBox.max - st->bBox.min) * 0.5f).length());
		}

		bBox = subsets[0]->bBox;
		for (int s = 1; s < numSubsets; s++)
		{
			Subset *st = subsets[s];
			bBox.AddBBox(st->bBox);
		}

		bSphere = BSphere(bBox.GetCenter(), ((bBox.max - bBox.min) * 0.5f).length());
	}

	/*
	TODO:This is not optimal,need rewrite on modern GL and shaders
	*/
	void SkinnedMesh::drawSubset(int subset)
	{
		CreateVBO();
		Subset *st = subsets[subset];
		st->vertBuff->map();

		st->vertBuff->set();
		st->vertBuff->setTexCoordSource(0, 2, sizeof(Vertex), sizeof(Vec3));
		st->vertBuff->setNormalSource(sizeof(Vertex), sizeof(Vec3) + sizeof(Vec2));
		st->vertBuff->setTexCoordSource(1, 3, sizeof(Vertex), 2 * sizeof(Vec3) + sizeof(Vec2));
		st->vertBuff->setTexCoordSource(2, 3, sizeof(Vertex), 3 * sizeof(Vec3) + sizeof(Vec2));
		st->vertBuff->setVertexSource(3, sizeof(Vertex), 0);
		st->vertBuff->unMap();


		GetRender()->drawIndexedGeometry(st->indices, st->numIndices);

		st->vertBuff->unset();
		st->vertBuff->unsetVertexSource();
		st->vertBuff->unsetTexCoordSource(0);
		st->vertBuff->unsetNormalSource();
		st->vertBuff->unsetTexCoordSource(1);
		st->vertBuff->unsetTexCoordSource(2);

		SAFE_DELETE(st->vertBuff);
	}

	/*
	*/
	int SkinnedMesh::GetSubset(String name)
	{
		for (unsigned int s = 0; s < numSubsets; s++)
		{
			if (subsets[s]->name == name)
				return s;
		}
		return 0;
	}


	/*
	*/
	void SkinnedMesh::CalculateTBN()
	{
		SetFrame(0.0);

		for (int s = 0; s < numSubsets; s++)
		{
			Subset *st = subsets[s];

			for (unsigned int iLoop = 0; iLoop < st->numIndices / 3; iLoop++)
			{
				int ind0 = st->indices[iLoop * 3 + 0];
				int ind1 = st->indices[iLoop * 3 + 1];
				int ind2 = st->indices[iLoop * 3 + 2];

				Vec3 t[3];
				Vec3 b[3];

				TBNComputer::computeTBN(t[0], b[0],
					st->vertices[ind0].position,
					st->vertices[ind1].position,
					st->vertices[ind2].position,
					st->vertices[ind0].texcoord,
					st->vertices[ind1].texcoord,
					st->vertices[ind2].texcoord,
					st->vertices[ind0].normal);
				TBNComputer::computeTBN(t[1], b[1],
					st->vertices[ind1].position,
					st->vertices[ind2].position,
					st->vertices[ind0].position,
					st->vertices[ind1].texcoord,
					st->vertices[ind2].texcoord,
					st->vertices[ind0].texcoord,
					st->vertices[ind1].normal);
				TBNComputer::computeTBN(t[2], b[2],
					st->vertices[ind2].position,
					st->vertices[ind0].position,
					st->vertices[ind1].position,
					st->vertices[ind2].texcoord,
					st->vertices[ind0].texcoord,
					st->vertices[ind1].texcoord,
					st->vertices[ind2].normal);

				for (int w = 0; w < st->vertices[ind0].numWeights; w++)
				{
					st->vertices[ind0].weights[w].tangent += t[0];
					st->vertices[ind0].weights[w].binormal += b[0];
				}
				for (int w = 0; w < st->vertices[ind1].numWeights; w++)
				{
					st->vertices[ind1].weights[w].tangent += t[1];
					st->vertices[ind1].weights[w].binormal += b[1];
				}
				for (int w = 0; w < st->vertices[ind2].numWeights; w++)
				{
					st->vertices[ind2].weights[w].tangent += t[2];
					st->vertices[ind2].weights[w].binormal += b[2];
				}
			}

			for (unsigned int vLoop = 0; vLoop < st->numVertices; vLoop++)
			{
				for (int w = 0; w < st->vertices[vLoop].numWeights; w++)
				{
					st->vertices[vLoop].weights[w].tangent = Vec3::normalize(st->vertices[vLoop].weights[w].tangent);
					st->vertices[vLoop].weights[w].binormal = Vec3::normalize(st->vertices[vLoop].weights[w].binormal);
				}
			}
		}

		//SetFrame(0.0);//Из 91
	}

	/*
	*/
	void SkinnedMesh::CalculateBoundings()
	{
		SetFrame(0.0);
		for (unsigned int s = 0; s < numSubsets; s++)
		{
			Subset *st = subsets[s];

			st->bBox = BBox(st->vertices[0].position, st->vertices[0].position);
			for (unsigned int v = 1; v < st->numVertices; v++)
			{
				st->bBox.AddPoint(st->vertices[v].position);
			}

			st->bSphere = BSphere(st->bBox.GetCenter(), 0);
			for (unsigned int v = 0; v < st->numVertices; v++)
			{
				st->bSphere.AddPoint(st->vertices[v].position);
			}
		}

		bBox = subsets[0]->bBox;
		for (int s = 1; s < numSubsets; s++)
		{
			Subset *st = subsets[s];
			bBox.AddBBox(st->bBox);
		}

		bSphere = BSphere(bBox.GetCenter(), ((bBox.max - bBox.min) * 0.5f).length());
	}

	/**
	*/
	void SkinnedMesh::CreateVBO() {
		for (unsigned int s = 0; s < numSubsets; s++) {
			Subset *st = subsets[s];
			st->vertBuff = GetRender()->CreateVBO(st->vertices, st->numVertices, sizeof(Vertex), I_VBManager::FLOAT, I_VBManager::STREAM);
		}
	}

}