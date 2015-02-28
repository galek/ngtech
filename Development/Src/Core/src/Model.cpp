#include "CorePrivate.h"
//**************************************
#include "Model.h"
#include "MeshLoader.h"
//**************************************

namespace NGTech {
	/**
	*/
	Model::Model(const String &path) {
		_load(path);
	}

	/**
	*/
	void Model::_load(const String &path) {
		GetMeshLoader()->Load(path, this);

		calculateTBN();
		createVBO();
		CalculateBoundings();
	}

	/**
	*/
	Model::~Model() {
		for (unsigned int s = 0; s < numSubsets; s++) {
			Subset *st = subsets[s];

			delete st->vertBuff;

			delete[] st->vertices;
			delete[] st->indices;
			delete st;
		}
	}

	/**
	*/
	void Model::save(const String &path) {
	}

	/**
	*/
	void Model::drawSubset(size_t s) {
		Subset *st = subsets[s];

		st->vertBuff->Bind();
		st->vertBuff->setTexCoordSource(0, 2, sizeof(Vertex), sizeof(Vec3));
		st->vertBuff->setNormalSource(sizeof(Vertex), sizeof(Vec3) + sizeof(Vec2));
		st->vertBuff->setTexCoordSource(1, 3, sizeof(Vertex), 2 * sizeof(Vec3) + sizeof(Vec2));
		st->vertBuff->setTexCoordSource(2, 3, sizeof(Vertex), 3 * sizeof(Vec3) + sizeof(Vec2));
		st->vertBuff->setVertexSource(3, sizeof(Vertex), 0);

		GetRender()->DrawElements(st->indices, st->numIndices);

		st->vertBuff->UnBind();
		st->vertBuff->unsetVertexSource();
		st->vertBuff->unsetTexCoordSource(0);
		st->vertBuff->unsetNormalSource();
		st->vertBuff->unsetTexCoordSource(1);
		st->vertBuff->unsetTexCoordSource(2);
	}

	/**
	*/
	int Model::getSubset(String name) {
		for (unsigned int s = 0; s < numSubsets; s++) {
			if (subsets[s]->name == name)
				return s;
		}
		return 0;
	}

	/**
	*/
	void Model::calculateTBN() {
		for (int s = 0; s < numSubsets; s++)
		{
			Subset *st = subsets[s];

			for (unsigned int iLoop = 0; iLoop < st->numIndices / 3; iLoop++)
			{
				int ind0 = st->indices[iLoop * 3 + 0];
				int ind1 = st->indices[iLoop * 3 + 1];
				int ind2 = st->indices[iLoop * 3 + 2];

				Vec3 n;
				TBNComputer::computeN(n, st->vertices[ind0].position,
					st->vertices[ind1].position,
					st->vertices[ind2].position);

				st->vertices[ind0].normal += n;
				st->vertices[ind1].normal += n;
				st->vertices[ind2].normal += n;
			}

			for (unsigned int vLoop = 0; vLoop < st->numVertices; vLoop++)
			{
				st->vertices[vLoop].normal = Vec3::normalize(st->vertices[vLoop].normal);
			}

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

				st->vertices[ind0].tangent += t[0];
				st->vertices[ind1].tangent += t[1];
				st->vertices[ind2].tangent += t[2];

				st->vertices[ind0].binormal += b[0];
				st->vertices[ind1].binormal += b[1];
				st->vertices[ind2].binormal += b[2];
			}

			for (unsigned int vLoop = 0; vLoop < st->numVertices; vLoop++)
			{
				st->vertices[vLoop].tangent = Vec3::normalize(st->vertices[vLoop].tangent);
				st->vertices[vLoop].binormal = Vec3::normalize(st->vertices[vLoop].binormal);
			}
		}
	}

	/**
	*/
	void Model::CalculateBoundings()
	{
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
	void Model::createVBO() {
		for (unsigned int s = 0; s < numSubsets; s++) {
			Subset *st = subsets[s];
			st->vertBuff = GetRender()->CreateVBO(st->vertices, st->numVertices, sizeof(Vertex), I_VBManager::FLOAT, I_VBManager::STATIC);
		}
	}
}