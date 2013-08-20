#include "WrappedScriptFunctions.h"
#include "log.h"

namespace VEGA
{
        /**
        */
        void ScriptLogPrintf(std::string str){
                LogPrintf(str.c_str());
        }
        /**
        */
        void ScriptDebug(std::string str){
 //Nick:TODO               LogPrintf(str.c_str());
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