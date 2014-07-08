// This is file is implementation of Core Functions

//Nick:TODO:Сделай интерфейс,нечто IEngine,что бы GetEngine возвращало интерфейс
#include "Engine.h"

namespace NGTech {

	FileSystem*	GetVFSPtr(){
		return GetEngine()->vfs;
	}
}