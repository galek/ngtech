#include "WrappedScriptFunctions.h"
#include "log.h"

namespace NGTech
{
        /**
        */
        void ScriptLogPrintf(std::string str){
                LogPrintf(str.c_str());
        }
        /**
        */
        void ScriptDebug(std::string str){
                DebugF(str.c_str(),"is not avalible in script",0);
        }
        /**
        */
        void ScriptWarning(std::string str){
                Warning(str.c_str());
        }
        /**
        */
        void ScriptError(std::string str,bool _status){
                Error(str.c_str(),_status);
        }
		
}