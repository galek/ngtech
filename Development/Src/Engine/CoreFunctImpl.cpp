// This is file is implementation of Core Functions

//Nick:TODO:������ ���������,����� IEngine,��� �� GetEngine ���������� ���������
#include "..\Engine.h"

namespace VEGA {

	FileSystem*	GetVFSPtr(){
		return GetEngine()->vfs;
	}
}