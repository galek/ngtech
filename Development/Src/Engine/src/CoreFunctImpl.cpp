// This is file is implementation of Core Functions

//Nick:TODO:������ ���������,����� IEngine,��� �� GetEngine ���������� ���������
#include "Engine.h"

namespace NGTech {

	FileSystem*	GetVFSPtr(){
		return GetEngine()->vfs;
	}
}