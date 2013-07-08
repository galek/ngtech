/***************************************************************************
*   Copyright (C) 2006 by AST   *
*   tsyplyaev@gmail.com   *
*   ICQ: 279-533-134                          *
*   This is a part of work done by AST.       *
*   If you want to use it, please contact me. *
***************************************************************************/

#include "EnginePrivate.h"
//**************************************
#include "SkinnedModel.h"
#include "File.h"
#include "Log.h"
#include "Frustum.h"
#include "MathLib.h"
#include "Error.h"
//**************************************

namespace NGEngine {

	//---------------------------------------------------------------------------
	//Desc:    creates new SkinnedSkinnedModel from file
	//Params:  path - object file path
	//Returns: -
	//---------------------------------------------------------------------------
	SkinnedModel::SkinnedModel(const String &path) {
		if(FileHelper::getFileExt(path) == "asmdl") {
			loadASMDL(path);
		}
		return;
	}
	/*
	void read_string(FILE *file,char *string) {
	char c;
	char *s = string;
	int quoted = -1;
	while(fread(&c,sizeof(char),1,file) == 1) {
	if(quoted == -1) {
	if(c == '"') quoted = 1;
	else if(!strchr(" \t\n\r",c)) {
	*s++ = c;
	quoted = 0;
	}
	continue;
	}
	if(quoted == 1 && c == '"') break;
	if(quoted == 0 && strchr(" \t\n\r",c)) break;
	*s++ = c;
	}
	*s = '\0';
	}*/

	//---------------------------------------------------------------------------
	//Desc:    creates new SkinnedSkinnedModel from ASMDL file
	//Params:  path - object file path
	//Returns: -
	//---------------------------------------------------------------------------
	void SkinnedModel::loadASMDL(const String &path) {
		/*FILE *file = fopen(path.c_str(),"r");
		if(!file) {
		Log::write(StringHelper::printf("SkinnedMesh::load_ascii(): error open \"%s\" file\n", path.c_str()));
		return;
		}
		char buf[1024];
		while(fscanf(file,"%s",buf) != EOF) {
		if(!strcmp(buf,"bones")) {

		fscanf(file,"%d %s",&numBones,buf);
		bones = new Bone[numBones];
		for(int i = 0; i < numBones; i++) {
		memset(bones[i].name,0,sizeof(bones[i].name));
		read_string(file,bones[i].name);
		fscanf(file,"%d",&bones[i].parent);
		}
		fscanf(file,"%s",buf);
		}
		else if(!strcmp(buf,"surface")) {
		Subset *s = new Subset();
		char name[64];
		memset(name, 0, sizeof(name));
		read_string(file,name);
		s->name = name;
		fscanf(file,"%s",buf);

		while(fscanf(file,"%s",buf) != EOF) {
		if(!strcmp(buf,"vertex")) {
		fscanf(file,"%d %s",&s->numVertices,buf);
		s->vertices = new Vertex[s->numVertices];
		for(int i = 0; i < s->numVertices; i++) {
		Vertex *v = &s->vertices[i];
		fscanf(file,"%f %f",&v->texcoord.x,&v->texcoord.y);
		v->texcoord = v->texcoord * 0.005;
		fscanf(file,"%s %d",buf,&v->numWeights);
		fscanf(file,"%s",buf);
		v->weights = new Weight[v->numWeights];
		for(int j = 0; j < v->numWeights; j++) {
		Weight *w = &v->weights[j];
		fscanf(file,"%d %f",&w->bone,&w->weight);
		fscanf(file,"%f %f %f",&w->position.x,&w->position.z,&w->position.y);
		w->position = w->position * 0.05;
		fscanf(file,"%f %f %f",&w->normal.x,&w->normal.z,&w->normal.y);
		}
		fscanf(file,"%s",buf);
		}
		fscanf(file,"%s",buf);
		}
		else if(!strcmp(buf,"triangles")) {
		fscanf(file,"%d %s",&s->numIndices,buf);
		s->numIndices *= 3;
		s->indices = new unsigned int[s->numIndices];
		for(int i = 0; i < s->numIndices/3; i++) {
		fscanf(file,"%d %d %d",
		&s->indices[i * 3 + 0],
		&s->indices[i * 3 + 2],
		&s->indices[i * 3 + 1]);

		}
		fscanf(file,"%s",buf);
		}
		else if(!strcmp(buf,"}")) {
		break;
		}
		else {
		Log::write(StringHelper::printf("SkinnedMesh::load_ascii(): unknown token \"%s\"\n",buf));
		fclose(file);
		return;
		}
		}
		subsets.push_back(s);
		}
		else if(!strcmp(buf,"animation")) {
		fscanf(file,"%d %s",&numFrames,buf);
		frames = new Frame*[numFrames];
		for(int i = 0; i < numFrames; i++) {
		frames[i] = new Frame[numBones];
		for(int j = 0; j < numBones; j++) {
		Frame *f = &frames[i][j];
		fscanf(file,"%f %f %f",&f->position.x,&f->position.z,&f->position.y);
		f->position = f->position * 0.05;
		fscanf(file,"%f %f %f %f",&f->rotation.x,&f->rotation.z,&f->rotation.y,&f->rotation.w);

		f->rotation.x = -f->rotation.x;
		f->rotation.y = -f->rotation.y;
		f->rotation.z = -f->rotation.z; 
		}
		}
		fscanf(file,"%s",buf);
		}
		else {
		Log::write(StringHelper::printf("SkinnedMesh::load_ascii(): unknown token \"%s\"\n",buf));
		fclose(file);
		return;
		}
		}
		fclose(file);*/

		//begin loading
		FILE *mFile = fopen(path.c_str(), "r");

		//Check if exist
		if(!mFile) {
			Error::showAndExit("SkinnedModel::SkinnedModel() error: object file '" + path + "' not found");
			return;
		}

		char buf[1024];
		//number of subsets

		fscanf(mFile, "%s", buf); //#AST_ENGINE_SKINNED_MESH

		fscanf(mFile, "\nnum_bones %i", &numBones); //num_subsets
		bones = new Bone[numBones];

		for(int i = 0; i < numBones; i++) { 
			fscanf(mFile, "%s %i", bones[i].name, &bones[i].parent); //num_subsets
		}

		int numSubsets;
		fscanf(mFile, "\nnum_subsets %i", &numSubsets); //num_subsets
		//Log::write(FileHelper::readString(mFile));

		//process subsets
		for(int s = 0; s < numSubsets; s++) {
			Subset *st = new Subset();

			fscanf(mFile, "\nsubset %s", st->name);

			//number of vertices
			fscanf(mFile, "\nnum_vertices %i", &st->numVertices);
			st->vertices = new Vertex[st->numVertices];

			//process vertices
			for(int v = 0; v < st->numVertices; v++) {
				fscanf(mFile, "\nnum_weights %i", &st->vertices[v].numWeights);
				st->vertices[v].weights = new Weight[st->vertices[v].numWeights];

				for(int w = 0; w < st->vertices[v].numWeights; w++) {
					fscanf(mFile, "%i %f %f %f %f %f %f %f",
						&st->vertices[v].weights[w].bone,
						&st->vertices[v].weights[w].weight,
						&st->vertices[v].weights[w].position.x,
						&st->vertices[v].weights[w].position.y,
						&st->vertices[v].weights[w].position.z,
						&st->vertices[v].weights[w].normal.x,
						&st->vertices[v].weights[w].normal.y,
						&st->vertices[v].weights[w].normal.z);
				}

				fscanf(mFile, "%f %f",
					&st->vertices[v].texcoord.x,
					&st->vertices[v].texcoord.y);
			}

			//number of faces

			fscanf(mFile, "\nnum_faces %i", &st->numIndices);
			st->numIndices *= 3;
			st->indices = new unsigned int[st->numIndices];

			//process faces
			for(int i = 0; i < st->numIndices/3; i++) {
				fscanf(mFile, "%i %i %i",
					&st->indices[i*3 + 0],
					&st->indices[i*3 + 1],
					&st->indices[i*3 + 2]);
			}

			subsets.push_back(st);
		}

		fscanf(mFile, "\nnum_frames %i", &numFrames); //num_frames
		frames = new Frame*[numFrames];

		for(int f = 0; f < numFrames; f++) {
			frames[f] = new Frame[numBones];

			for(int i = 0; i < numBones; i++) {
				fscanf(mFile, "%f %f %f %f %f %f %f",
					&frames[f][i].position.x,
					&frames[f][i].position.y,
					&frames[f][i].position.z,
					&frames[f][i].rotation.x,
					&frames[f][i].rotation.y,
					&frames[f][i].rotation.z,
					&frames[f][i].rotation.w);
			}
		}

		fclose(mFile);

		calculateTBN();
		createVBO();
		calcBoundings();
	}

	//---------------------------------------------------------------------------
	//Desc:    sets the animation frame
	//Params:  frame - frame number
	//Returns: -
	//---------------------------------------------------------------------------
	void SkinnedModel::setFrame(float frame, int from, int to) {

		if(numFrames == 0) return;

		if(from < 0) from = 0;
		if(to < 0) to = numFrames;

		int frame0 = (int)frame;
		frame -= frame0;
		frame0 += from;
		if(frame0 >= to) frame0 = (frame0 - from) % (to - from) + from;
		int frame1 = frame0 + 1;
		if(frame1 >= to) frame1 = from;

		for(int i = 0; i < numBones; i++) {	// calculate matrixes
			Mat4 translate;
			translate = Mat4::translate(frames[frame0][i].position * (1.0f - frame) + frames[frame1][i].position * frame);

			Quat rot;
			rot = Quat::slerp(frames[frame0][i].rotation,frames[frame1][i].rotation,frame);

			bones[i].rotation = Mat4(rot.toMatrix());
			bones[i].transform = translate * bones[i].rotation;
		}

		min = Vec3(1000000,1000000,1000000);	// bound box
		max = Vec3(-1000000,-1000000,-1000000);

		for(int i = 0; i < (int)subsets.size(); i++) {	// calculate vertexes
			Subset *s = subsets[i];

			s->min = Vec3(1000000,1000000,1000000);
			s->max = Vec3(-1000000,-1000000,-1000000);

			for(int j = 0; j < s->numVertices; j++) {
				Vertex *v = &s->vertices[j];
				v->position = Vec3(0,0,0);
				v->normal = Vec3(0,0,0);
				v->tangent = Vec3(0,0,0);
				v->binormal = Vec3(0,0,0);
				for(int k = 0; k < v->numWeights; k++) {
					Weight *w = &v->weights[k];
					v->position += bones[w->bone].transform * w->position * w->weight;
					v->normal += bones[w->bone].rotation * w->normal * w->weight;
					v->tangent += bones[w->bone].rotation * w->tangent * w->weight;
					v->binormal += bones[w->bone].rotation * w->binormal * w->weight;
				}
				if(s->max.x < v->position.x) s->max.x = v->position.x;
				if(s->min.x > v->position.x) s->min.x = v->position.x;
				if(s->max.y < v->position.y) s->max.y = v->position.y;
				if(s->min.y > v->position.y) s->min.y = v->position.y;
				if(s->max.z < v->position.z) s->max.z = v->position.z;
				if(s->min.z > v->position.z) s->min.z = v->position.z;
			}

			s->center = (s->min + s->max) / 2.0f;
			s->radius = (s->max - s->center).length();
			if(max.x < s->max.x) max.x = s->max.x;
			if(min.x > s->min.x) min.x = s->min.x;
			if(max.y < s->max.y) max.y = s->max.y;
			if(min.y > s->min.y) min.y = s->min.y;
			if(max.z < s->max.z) max.z = s->max.z;
			if(min.z > s->min.z) min.z = s->min.z;
		}

		center = (min + max) / 2.0f;
		radius = (max - center).length();
	}


	//---------------------------------------------------------------------------
	//Desc:    destroys object
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	SkinnedModel::~SkinnedModel() {
		for(int s = 0; s < subsets.size(); s++) {
			Subset *st = subsets[s];

			delete st->vertBuff;

			delete [] st->vertices;
			delete [] st->indices;
			delete st;
		}
	}

	//---------------------------------------------------------------------------
	//Desc:    saves model to file
	//Params:  path - file path
	//Returns: -
	//---------------------------------------------------------------------------
	void SkinnedModel::save(const String &path) {
		FILE *file;
		file = fopen(path.c_str(), "wt");

		//number of subsets

		fprintf(file, "#AST_ENGINE_SKINNED_MODEL\n\n"); //#AST_ENGINE_MESH

		fprintf(file, "num_bones %i\n", numBones); //num_bones
		for(int i = 0; i < numBones; i++) {
			fprintf(file, "%s %i\n", bones[i].name, bones[i].parent); //num_bones
		}
		fprintf(file, "\n");

		fprintf(file, "num_subsets %i\n\n", subsets.size()); //num_subsets

		//process subsets
		for(int s = 0; s < subsets.size(); s++) {
			Subset *st = subsets[s];
			fprintf(file, "subset %s\n", st->name);

			fprintf(file, "num_vertices %i\n", st->numVertices);

			//process vertices
			for(int v = 0; v < st->numVertices; v++) {
				fprintf(file, "num_weights %i\n", st->vertices[v].numWeights);
				for(int w = 0; w < st->vertices[v].numWeights; w++) {
					fprintf(file, "%i %f %f %f %f %f %f %f\n",
						st->vertices[v].weights[w].bone,
						st->vertices[v].weights[w].weight,
						st->vertices[v].weights[w].position.x,
						st->vertices[v].weights[w].position.y,
						st->vertices[v].weights[w].position.z,
						st->vertices[v].weights[w].normal.x,
						st->vertices[v].weights[w].normal.y,
						st->vertices[v].weights[w].normal.z);
				}
				fprintf(file, "%f %f\n",
					st->vertices[v].texcoord.x,
					st->vertices[v].texcoord.y);
			}

			//number of faces

			fprintf(file, "num_faces %i\n", st->numIndices/3);

			//process faces
			for(int i = 0; i < st->numIndices/3; i++) {
				fprintf(file, "%i %i %i\n",
					st->indices[i*3 + 0],
					st->indices[i*3 + 1],
					st->indices[i*3 + 2]);
			}
			fprintf(file, "\n");
		}

		fprintf(file, "num_frames %i\n", numFrames); 
		for(int i = 0; i < numFrames; i++) {
			for(int k = 0; k < numBones; k++) {
				fprintf(file, "%f %f %f %f %f %f %f\n", frames[i][k].position.x,
					frames[i][k].position.y,
					frames[i][k].position.z,
					frames[i][k].rotation.x,
					frames[i][k].rotation.y,
					frames[i][k].rotation.z,
					frames[i][k].rotation.w);
			}
		}

		fclose(file);
	}

	//---------------------------------------------------------------------------
	//Desc:    draws object subset
	//Params:  s - surface number
	//Returns: -
	//---------------------------------------------------------------------------
	void SkinnedModel::drawSubset(int s) {
		Subset *st = subsets[s];

		glClientActiveTextureARB(GL_TEXTURE0_ARB);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2,GL_FLOAT,sizeof(Vertex),st->vertices->texcoord);

		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT,sizeof(Vertex),st->vertices->normal);

		glClientActiveTextureARB(GL_TEXTURE1_ARB);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(3,GL_FLOAT,sizeof(Vertex),st->vertices->tangent);

		glClientActiveTextureARB(GL_TEXTURE2_ARB);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(3,GL_FLOAT,sizeof(Vertex),st->vertices->binormal);

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3,GL_FLOAT,sizeof(Vertex),st->vertices->position);


		glDrawElements(GL_TRIANGLES,st->numIndices, GL_UNSIGNED_INT, st->indices);


		glDisableClientState(GL_VERTEX_ARRAY);

		glClientActiveTextureARB(GL_TEXTURE0_ARB);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		glDisableClientState(GL_NORMAL_ARRAY);

		glClientActiveTextureARB(GL_TEXTURE1_ARB);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		glClientActiveTextureARB(GL_TEXTURE2_ARB);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	//---------------------------------------------------------------------------
	//Desc:    get subset number by number
	//Params:  s - subset name
	//Returns: subset number
	//---------------------------------------------------------------------------
	int SkinnedModel::getSubset(String name) {
		for(int s = 0; s < subsets.size(); s++) {
			if(subsets[s]->name == name)
				return s;
		}
		return 0;
	}


	//---------------------------------------------------------------------------
	//Desc:    calculates tangents for each surfaces
	//Params:  s - subset number
	//Returns: -
	//---------------------------------------------------------------------------
	void SkinnedModel::calculateTBN() {
		setFrame(0.0);

		for(int s = 0; s < subsets.size(); s++) {
			Subset *st = subsets[s];

			for(int iLoop = 0; iLoop < st->numIndices/3; iLoop++) {
				int ind0 = st->indices[iLoop*3 + 0];
				int ind1 = st->indices[iLoop*3 + 1];
				int ind2 = st->indices[iLoop*3 + 2];

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

				for(int w = 0; w < st->vertices[ind0].numWeights; w++) {
					st->vertices[ind0].weights[w].tangent += t[0];
					st->vertices[ind0].weights[w].binormal += b[0];
				}
				for(int w = 0; w < st->vertices[ind1].numWeights; w++) {
					st->vertices[ind1].weights[w].tangent += t[1];
					st->vertices[ind1].weights[w].binormal += b[1];
				}
				for(int w = 0; w < st->vertices[ind2].numWeights; w++) {
					st->vertices[ind2].weights[w].tangent += t[2];
					st->vertices[ind2].weights[w].binormal += b[2];
				}
			}

			for(int vLoop = 0; vLoop < st->numVertices; vLoop++) {
				for(int w = 0; w < st->vertices[vLoop].numWeights; w++) {
					st->vertices[vLoop].weights[w].tangent = Vec3::normalize(st->vertices[vLoop].weights[w].tangent);
					st->vertices[vLoop].weights[w].binormal = Vec3::normalize(st->vertices[vLoop].weights[w].binormal);
				}
			}
		}

		setFrame(0.0);
	}

	//---------------------------------------------------------------------------
	//Desc:    calculates boundings of the scene
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void SkinnedModel::calcBoundings() {
		for(int s = 0; s < subsets.size(); s++) {
			Subset *st = subsets[s];

			for(int v = 0; v < st->numVertices; v++) {
				//need to do so because of MSVC 2005 bug
				if(v == 0) {
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

			for(int v = 0; v < st->numVertices; v++) {
				st->radius = max(st->radius, (st->vertices[v].position - st->center).length());
			}
		}

		min = subsets[0]->min;
		max = subsets[0]->max;

		for(int s = 1; s < subsets.size(); s++) {
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

		for(int s = 0; s < subsets.size(); s++) {
			Subset *st = subsets[s];
			for(int v = 0; v < st->numVertices; v++) {
				radius = max(radius, (st->vertices[v].position - center).length());
			}
		}
	}


	//---------------------------------------------------------------------------
	//Desc:    creates VBO`s for geometry information
	//Params:  -
	//Returns: -
	//---------------------------------------------------------------------------
	void SkinnedModel::createVBO() {
		for(int s = 0; s < subsets.size(); s++) {
			Subset *st = subsets[s];
			st->vertBuff = GLVBO::createVBO(st->vertices, st->numVertices, sizeof(Vertex), GLVBO::FLOAT);
		}
	}

}