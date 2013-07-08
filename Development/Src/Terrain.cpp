/***************************************************************************
*   Copyright (C) 2006 by AST   *
*   tsyplyaev@gmail.com   *
*   ICQ: 279-533-134                          *
*   This is a part of work done by AST.       *
*   If you want to use it, please contact me. *
***************************************************************************/

#include "EnginePrivate.h"
//************************************
#include "Engine.h"
#include "Terrain.h"
#include "Frustum.h"
//************************************

namespace NGEngine {

	Terrain::Terrain(const String &path, float step, float height, int nodeSize) {
		ILImage *hmap = ILImage::create2d(path);

		int W = hmap->getWidth();
		int H = hmap->getHeight();

		numVertices = W * H;

		vertices = new Vertex[numVertices];

		int NW = (W - 1)/nodeSize;
		int NH = (H - 1)/nodeSize;

		numNodes = NW*NH;
		nodes = new Node[numNodes];

		for(int h = 0; h < H; h++) {
			for(int w = 0; w < W; w++) {
				vertices[h*W + w].position.x = (w*2 - W)*step;
				vertices[h*W + w].position.z = (h*2 - H)*step;
				vertices[h*W + w].position.y = height * ((float)hmap->getData()[(h*W + w)*hmap->getBPP()] / 255.0f);

				vertices[h*W + w].texcoord.x = (float)w/(float)W;
				vertices[h*W + w].texcoord.y = (float)h/(float)H; 
			}
		}

		for(int nw = 0; nw < NW; nw++) {
			for(int nh = 0; nh < NH; nh++) {
				Node &nd = nodes[nh * NW + nw];

				nd.numIndices[0] = nodeSize * nodeSize * 6;
				nd.numIndices[1] = nodeSize * nodeSize * 6 / 4;
				nd.numIndices[2] = nodeSize * nodeSize * 6 / 16;
				nd.numIndices[3] = nodeSize * nodeSize * 6 / 64;

				nd.indices[0] = new unsigned int[nd.numIndices[0]];
				nd.indices[1] = new unsigned int[nd.numIndices[1]];
				nd.indices[2] = new unsigned int[nd.numIndices[2]];
				nd.indices[3] = new unsigned int[nd.numIndices[3]];

				int l = 0;
				for(int w = nw*nodeSize; w < nodeSize + nw*nodeSize; w++) {
					for(int h = nh*nodeSize; h < nodeSize + nh*nodeSize; h++) {
						nd.indices[0][l*3+0] = h*W + w;
						nd.indices[0][l*3+1] = (h+1)*W + w;
						nd.indices[0][l*3+2] = h*W + w + 1;

						nd.indices[0][l*3+3] = h*W + w+1;
						nd.indices[0][l*3+4] = (h+1)*W + w;
						nd.indices[0][l*3+5] = (h+1)*W + w + 1;
						l+=2;
					}
				}

				l = 0;
				for(int w = nw*nodeSize; w < nodeSize + nw*nodeSize; w+=2) {
					for(int h = nh*nodeSize; h < nodeSize + nh*nodeSize; h+=2) {
						nd.indices[1][l*3+0] = h*W + w;
						nd.indices[1][l*3+1] = (h+2)*W + w;
						nd.indices[1][l*3+2] = h*W + w + 2;

						nd.indices[1][l*3+3] = h*W + w + 2;
						nd.indices[1][l*3+4] = (h+2)*W + w;
						nd.indices[1][l*3+5] = (h+2)*W + w + 2;
						l+=2;
					}
				}

				l = 0;
				for(int w = nw*nodeSize; w < nodeSize + nw*nodeSize; w+=4) {
					for(int h = nh*nodeSize; h < nodeSize + nh*nodeSize; h+=4) {
						nd.indices[2][l*3+0] = h*W + w;
						nd.indices[2][l*3+1] = (h+4)*W + w;
						nd.indices[2][l*3+2] = h*W + w + 4;

						nd.indices[2][l*3+3] = h*W + w + 4;
						nd.indices[2][l*3+4] = (h+4)*W + w;
						nd.indices[2][l*3+5] = (h+4)*W + w + 4;
						l+=2;
					}
				}

				l = 0;
				for(int w = nw*nodeSize; w < nodeSize + nw*nodeSize; w+=8) {
					for(int h = nh*nodeSize; h < nodeSize + nh*nodeSize; h+=8) {
						nd.indices[3][l*3+0] = h*W + w;
						nd.indices[3][l*3+1] = (h+8)*W + w;
						nd.indices[3][l*3+2] = h*W + w + 8;

						nd.indices[3][l*3+3] = h*W + w + 8;
						nd.indices[3][l*3+4] = (h+8)*W + w;
						nd.indices[3][l*3+5] = (h+8)*W + w + 8;
						l+=2;
					}
				}

				Vec3 min = vertices[nd.indices[0][0]].position;
				Vec3 max = vertices[nd.indices[0][0]].position;

				for(int i = 1; i < nd.numIndices[0]; i++) {
					if(min.x > vertices[nd.indices[0][i]].position.x) min.x = vertices[nd.indices[0][i]].position.x;
					if(min.y > vertices[nd.indices[0][i]].position.y) min.y = vertices[nd.indices[0][i]].position.y;
					if(min.z > vertices[nd.indices[0][i]].position.z) min.z = vertices[nd.indices[0][i]].position.z;

					if(max.x < vertices[nd.indices[0][i]].position.x) max.x = vertices[nd.indices[0][i]].position.x;
					if(max.y < vertices[nd.indices[0][i]].position.y) max.y = vertices[nd.indices[0][i]].position.y;
					if(max.z < vertices[nd.indices[0][i]].position.z) max.z = vertices[nd.indices[0][i]].position.z;
				}

				nd.min = min;
				nd.max = max;
				nd.center = (max + min) * 0.5;

				nd.radius = 0; 

				for(int i = 1; i < nd.numIndices[0]; i++) {
					float length = (nd.center - vertices[nd.indices[0][i]].position).length();
					if(length > nd.radius) 
						nd.radius = length;
				}
			}
		}

		computeTBN();

		vertBuff = GLVBO::createVBO(vertices, numVertices, sizeof(Vertex), GLVBO::FLOAT);
	}

	Terrain::~Terrain() {
		delete vertBuff;
		delete [] vertices;
		for(int i = 0; i < numNodes; i++) {
			delete [] nodes[i].indices;
		}
	}

	void Terrain::computeTBN() {
		for(int i = 0; i < numNodes; i++) {
			for(int iLoop = 0; iLoop < nodes[i].numIndices[0]/3; iLoop++) {
				int ind0 = nodes[i].indices[0][iLoop*3 + 0];
				int ind1 = nodes[i].indices[0][iLoop*3 + 1];
				int ind2 = nodes[i].indices[0][iLoop*3 + 2];

				Vec3 n;
				TBNComputer::computeN(n, vertices[ind0].position,
					vertices[ind1].position,
					vertices[ind2].position);

				vertices[ind0].normal += n;
				vertices[ind1].normal += n;
				vertices[ind2].normal += n;
			}
		}

		for(int vLoop = 0; vLoop < numVertices; vLoop++) {
			vertices[vLoop].normal = Vec3::normalize(vertices[vLoop].normal);
		}
	}

	void Terrain::setPhysics() {
		int numPos = 0;
		for(int i = 0; i < numNodes; i++) {
			Node &nd = nodes[i];
			numPos += nd.numIndices[2];
		}
		Vec3 *pos = new Vec3[numPos];

		int k = 0;
		for(int i = 0; i < numNodes; i++) {
			Node &nd = nodes[i];

			for(int v = 0; v < nd.numIndices[2]/3; v++) {
				pos[k*3+0] = vertices[nd.indices[2][v*3+0]].position;
				pos[k*3+1] = vertices[nd.indices[2][v*3+1]].position;
				pos[k*3+2] = vertices[nd.indices[2][v*3+2]].position;
				k++;
			}
		}

		pBody = PhysBody::createStaticObjectMesh(pos, numPos, false);

		delete [] pos;
	}

	void Terrain::drawNode(int n, const Vec3 &cameraPos) {
		Vec3 pos = nodes[n].center;
		int lod = 0;
		if((pos - cameraPos).length() > lods.x) lod = 1;
		if((pos - cameraPos).length() > lods.y) lod = 2;
		if((pos - cameraPos).length() > lods.z) lod = 3;

		vertBuff->set(); 
		vertBuff->setTexCoordSource(0, 2, sizeof(Vertex), sizeof(Vec3));
		vertBuff->setNormalSource(sizeof(Vertex), sizeof(Vec3) + sizeof(Vec2));
		vertBuff->setVertexSource(3, sizeof(Vertex), 0);

		engine.iRender->drawIndexedGeometry(nodes[n].indices[lod], nodes[n].numIndices[lod]);

		vertBuff->unset();
		vertBuff->unsetVertexSource();
		vertBuff->unsetTexCoordSource(0);
		vertBuff->unsetNormalSource();
	}

	const Vec3 &Terrain::getMin(int n) {
		return nodes[n].min; 
	}

	const Vec3 &Terrain::getMax(int n) {
		return nodes[n].max; 
	}

	const Vec3 &Terrain::getCenter(int n) {
		return nodes[n].center; 
	}

	float Terrain::getRadius(int n) {
		return nodes[n].radius; 
	}

}