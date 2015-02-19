#include "EnginePrivate.h"
//**************************************
#include "Scene.h"
//**************************************

namespace NGTech {

	Object::Object() {
		if (GetScene())
			GetScene()->addObject(this);
		transform.Identity();
	}
}