#include "RenderPrivate.h"
#include "GLSystem.h"

extern "C"
{
	void __declspec(dllexport) dllStartPlugin(void) throw(){
		using namespace NGTech;
		Debug("Loading dllStartPlugin");
		GetEngine()->SetRender(new GLSystem());
	}
}