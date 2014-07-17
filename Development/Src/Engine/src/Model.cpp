#include "EnginePrivate.h"
//**************************************
#include "Engine.h"
#include "Model.h"
#include "File.h"
#include "Log.h"
#include "Frustum.h"
#include "MathLib.h"
#include "Error.h"
//**************************************

namespace NGTech {
	/**
	*/
	Model::Model(const String &path) {
		if (FileHelper::getFileExt(path) == "amdl") {
			loadAMDL(path);
		}

		return;
	}
	/**
	*/
	void Model::loadAMDL(const String &path) {
		//begin loading
		FILE *mFile = fopen(path.c_str(), "r");

		//Check if exist
		if (!mFile) {
			Error::showAndExit("Model::Model() error: object file '" + path + "' not found");
			return;
		}

		char buf[1024];
		//number of subsets

		fscanf(mFile, "%s", buf); //#AST_ENGINE_MESH

		int numSubsets;
		fscanf(mFile, "\nnum_subsets %i", &numSubsets); //num_subsets

		//process subsets
		for (int s = 0; s < numSubsets; s++) {
			Subset *st = new Subset();

			fscanf(mFile, "\nsubset %s", st->name);

			//number of vertices
			fscanf(mFile, "\nnum_vertices %i", &st->numVertices);
			st->vertices = new Vertex[st->numVertices];

			//process vertices
			for (int v = 0; v < st->numVertices; v++) {
				fscanf(mFile, "%f %f %f %f %f %f %f %f",
					&st->vertices[v].position.x,
					&st->vertices[v].position.y,
					&st->vertices[v].position.z,
					&st->vertices[v].normal.x,
					&st->vertices[v].normal.y,
					&st->vertices[v].normal.z,
					&st->vertices[v].texcoord.x,
					&st->vertices[v].texcoord.y);
			}

			//number of faces

			fscanf(mFile, "\nnum_faces %i", &st->numIndices);
			st->numIndices *= 3;
			st->indices = new unsigned int[st->numIndices];

			//process faces
			for (int i = 0; i < st->numIndices / 3; i++) {
				fscanf(mFile, "%i %i %i",
					&st->indices[i * 3 + 0],
					&st->indices[i * 3 + 1],
					&st->indices[i * 3 + 2]);
			}

			subsets.push_back(st);
		}

		fclose(mFile);

		calculateTBN();
		createVBO();
		calcBoundings();
	}
	/**
	*/
	Model::~Model() {
		for (int s = 0; s < subsets.size(); s++) {
			Subset *st = subsets[s];

			delete st->vertBuff;

			delete[] st->vertices;
			delete[] st->indices;
		}
	}
	/**
	*/
	void Model::save(const String &path) {
		FILE *file;
		file = fopen(path.c_str(), "wt");

		//number of subsets

		fprintf(file, "#AST_ENGINE_MODEL\n\n"); //#AST_ENGINE_MESH

		fprintf(file, "num_subsets %i\n\n", subsets.size()); //num_subsets

		//process subsets
		for (int s = 0; s < subsets.size(); s++) {
			Subset *st = subsets[s];
			fprintf(file, "subset %s\n", st->name);

			fprintf(file, "num_vertices %i\n", st->numVertices);

			//process vertices
			for (int v = 0; v < st->numVertices; v++) {
				fprintf(file, "%f %f %f %f %f %f %f %f\n",
					st->vertices[v].position.x,
					st->vertices[v].position.y,
					st->vertices[v].position.z,
					st->vertices[v].normal.x,
					st->vertices[v].normal.y,
					st->vertices[v].normal.z,
					st->vertices[v].texcoord.x,
					st->vertices[v].texcoord.y);
			}

			//number of faces

			fprintf(file, "num_faces %i\n", st->numIndices);
			st->numIndices *= 3;
			st->indices = new unsigned int[st->numIndices];

			//process faces
			for (int i = 0; i < st->numIndices / 3; i++) {
				fprintf(file, "%i %i %i\n",
					st->indices[i * 3 + 0],
					st->indices[i * 3 + 1],
					st->indices[i * 3 + 2]);
			}
			fprintf(file, "\n");
		}

		fclose(file);
	}
	/**
	*/
	void Model::drawSubset(int s) {
		Subset *st = subsets[s];

		st->vertBuff->set();
		st->vertBuff->setTexCoordSource(0, 2, sizeof(Vertex), sizeof(Vec3));
		st->vertBuff->setNormalSource(sizeof(Vertex), sizeof(Vec3) + sizeof(Vec2));
		st->vertBuff->setTexCoordSource(1, 3, sizeof(Vertex), 2 * sizeof(Vec3) + sizeof(Vec2));
		st->vertBuff->setTexCoordSource(2, 3, sizeof(Vertex), 3 * sizeof(Vec3) + sizeof(Vec2));
		st->vertBuff->setVertexSource(3, sizeof(Vertex), 0);

		GetRender()->drawIndexedGeometry(st->indices, st->numIndices);

		st->vertBuff->unset();
		st->vertBuff->unsetVertexSource();
		st->vertBuff->unsetTexCoordSource(0);
		st->vertBuff->unsetNormalSource();
		st->vertBuff->unsetTexCoordSource(1);
		st->vertBuff->unsetTexCoordSource(2);
	}
	/**
	*/
	int Model::getSubset(String name) {
		for (int s = 0; s < subsets.size(); s++) {
			if (subsets[s]->name == name)
				return s;
		}
		return 0;
	}
	/**
	*/
	void Model::calculateTBN() {
		for (int s = 0; s < subsets.size(); s++) {
			Subset *st = subsets[s];

			for (int iLoop = 0; iLoop < st->numIndices / 3; iLoop++) {
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

			for (int vLoop = 0; vLoop < st->numVertices; vLoop++) {
				st->vertices[vLoop].normal = Vec3::normalize(st->vertices[vLoop].normal);
			}

			for (int iLoop = 0; iLoop < st->numIndices / 3; iLoop++) {
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

			for (int vLoop = 0; vLoop < st->numVertices; vLoop++)	{
				st->vertices[vLoop].tangent = Vec3::normalize(st->vertices[vLoop].tangent);
				st->vertices[vLoop].binormal = Vec3::normalize(st->vertices[vLoop].binormal);
			}
		}
	}
	/**
	*/
	void Model::calcBoundings() {
		for (int s = 0; s < subsets.size(); s++) {
			Subset *st = subsets[s];

			for (int v = 0; v < st->numVertices; v++) {
				//need to do so because of MSVC 2005 bug
				if (v == 0) {
					st->min = st->vertices[0].position;
					st->max = st->vertices[0].position;
				}
				st->max = st->vertices[0].position;
				st->min.x = min(st->min.x, st->vertices[v].position.x);
				st->min.y = min(st->min.y, st->vertices[v].position.y);
				st->min.z = min(st->min.z, st->vertices[v].position.z);

				st->max.x = max(st->max.x, st->vertices[v].position.x);
				st->max.y = max(st->max.y, st->vertices[v].position.y);
				st->max.z = max(st->max.z, st->vertices[v].position.z);
			}

			st->radius = 0;
			st->center = (st->max + st->min) * 0.5;

			for (int v = 0; v < st->numVertices; v++) {
				st->radius = max(st->radius, (st->vertices[v].position - st->center).length());
			}
		}

		min = subsets[0]->min;
		max = subsets[0]->max;

		for (int s = 1; s < subsets.size(); s++) {
			Subset *st = subsets[s];

			min.x = min(min.x, st->min.x);
			min.y = min(min.y, st->min.y);
			min.z = min(min.z, st->min.z);

			max.x = max(max.x, st->max.x);
			max.y = max(max.y, st->max.y);
			max.z = max(max.z, st->max.z);
		}

		center = (min + max) * 0.5;
		radius = 0;

		for (int s = 0; s < subsets.size(); s++) {
			Subset *st = subsets[s];
			for (int v = 0; v < st->numVertices; v++) {
				radius = max(radius, (st->vertices[v].position - center).length());
			}
		}
	}
	/**
	*/
	void Model::createVBO() {
		for (int s = 0; s < subsets.size(); s++) {
			Subset *st = subsets[s];
			st->vertBuff = GetRender()->CreateVBO(st->vertices, st->numVertices, sizeof(Vertex), I_VBManager::FLOAT);
		}
	}
	/**
	*/
}