#include "EnginePrivate.h"
//**************************************
#include "Object.h"
#include "Log.h"
#include "Error.h"
#include "Cache.h"
#include "Scene.h"
//**************************************

namespace NGTech 
{
	ObjectMesh::ObjectMesh(const String &path)
		:Object(), pBody(nullptr), bMultipart(false)
	{
		auto cache = GetCache();
		if (cache){
			mesh = cache->loadModel("meshes/" + path);
			materials.resize(mesh->getNumSubsets());
			for (size_t i = 0; i < materials.size(); i++)
				materials[i] = nullptr;
		}
	}

	ObjectMesh::~ObjectMesh() {
		Warning(__FUNCTION__);
		auto cache = GetCache();
		if (cache){
			cache->deleteModel(mesh);
			for (size_t i = 0; i < materials.size(); i++) {
				cache->deleteMaterial(materials[i]);
			}
			materials.clear();
			cache->deleteSound(impactSound);
		}

		if (GetScene())
			GetScene()->deleteObjectMesh(this);

		if (bMultipart) {
			SAFE_DELETE_ARRAY(pBody);
		}
		else
			SAFE_DELETE(pBody);
	}

	void ObjectMesh::DrawSubset(size_t subset) {
		//BBox bbox = GetTransformedBBox(subset);
		//BSphere bsphere = GetTransformedBSphere(subset);

		//Camera *camera = GetScene()->GetCurrentCamera();
		//Light *light = GetScene()->GetCurrentLight();

		////frustum visibility
		//if (!camera)
		//	return;
		//	if (!camera->GetFrustum()->IsBSphereInside(bsphere))
		//		return;
		//Этого не было
		//	if (!camera->GetFrustum()->IsBBoxInside(bbox))
		//		return;

		//light visibility
		//if (!light)
		//	return;

		//BSphere lightSphere = BSphere(light->getPosition(), light->getRadius());
		//if (!lightSphere.IntersectsSphere(bsphere))
		//	return;

		//draw
		this->mesh->drawSubset(subset);

	}

	Material *ObjectMesh::GetMaterial(size_t s) {
		return materials[s];
	}

	void ObjectMesh::setMaterial(const String &path, const String &name) {
		auto cache = GetCache();
		if (cache)
		{
			Material *material = cache->loadMaterial(path);
			if (name == "*")
				for (size_t s = 0; s < mesh->getNumSubsets(); s++)
					materials[s] = material;
			materials[mesh->getSubset(name)] = material;
		}
	}

	void ObjectMesh::setMaterialList(const String &path) {
		VFile mFile(path.c_str(), VFile::READ_TEXT);

		//Check if exist
		if (!&mFile) {
			Error::showAndExit("ObjectMesh::setMaterialList() error: material list file '" + path + "' not found");
			return;
		}

		while (!mFile.IsEof()) {
			String line = mFile.GetLine();
			setMaterial(StringHelper::getWord(line, 1), StringHelper::getWord(line, 3));
		}
	}

	void ObjectMesh::SetTransform(const Mat4 &trans) {
		if (pBody)
			pBody->SetTransform(trans);
		else
			transform = trans;
	}

	const Mat4 ObjectMesh::GetTransform() {
		if (pBody == NULL)
			return transform;
		else
			return pBody->GetTransform();
	}

	void ObjectMesh::setPhysicsBox(const Vec3 &size, float mass) {
		pBody = PhysBody::CreateBox(size, &transform, mass);
	}

	void ObjectMesh::setPhysicsSphere(const Vec3 &size, float mass) {
		pBody = PhysBody::CreateSphere(size.y, &transform, mass);//Nick:TODO:заменить на getRadius
	}

	void ObjectMesh::setPhysicsCylinder(float radius, float width, float mass) {
		pBody = PhysBody::CreateCylinder(radius, width, &transform, mass);
	}

	void ObjectMesh::setPhysicsCapsule(float radius, float height, float mass)  {
		pBody = PhysBody::CreateCapsule(radius, height, &transform, mass);
	}

	void ObjectMesh::setPhysicsConvexHull(float mass) {
		int numVertices = 0;
		for (size_t i = 0; i < mesh->getNumSubsets(); i++) {
			numVertices += mesh->subsets[i]->numVertices;
		}

		PhysBody*pb = new PhysBody[mesh->getNumSubsets()];
		for (size_t i = 0; i < mesh->getNumSubsets(); i++)
			pb[i] = *PhysBody::CreateConvexHull(mesh->subsets[i]->numVertices, mesh->subsets[i]->numIndices, &transform, mesh->subsets[i]->vertices, mesh->subsets[i]->indices, mass);

		pBody = pb;
		bMultipart = true;
	}

	void ObjectMesh::setPhysicsStaticMesh() {
		int numIndices = 0;
		int numVertices = 0;
		for (size_t i = 0; i < mesh->getNumSubsets(); i++) {
			numIndices += mesh->subsets[i]->numIndices;
			numVertices += mesh->subsets[i]->numVertices;
		}

		auto pb = new PhysBody[mesh->getNumSubsets()];

		for (size_t i = 0; i < mesh->getNumSubsets(); i++)
			pb[i] = *PhysBody::CreateStaticMesh(mesh->subsets[i]->numVertices, mesh->subsets[i]->numIndices, &transform, mesh->subsets[i]->vertices, mesh->subsets[i]->indices);

		//Nick:BUG:Меш собирается в реалтайме,и часть тел пролетает,если сначала создать физику,и потом задать коллизию
		pBody = pb;
		bMultipart = true;
	}

	void ObjectMesh::setPhysicsCloth() {

	}

	void ObjectMesh::setImpactSound(const String &path) {
		auto cache = GetCache();
		if (cache)
		{
			impactSound = cache->loadSound(path);
			if (pBody)
				pBody->SetImpactSound(impactSound);
		}
	}
}