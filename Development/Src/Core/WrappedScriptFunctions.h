#pragma once
#include <string>
namespace VEGA
{
        void ScriptLogPrintf(std::string str);
        void ScriptDebug(std::string str);
        void ScriptWarning(std::string str);
        void ScriptError(std::string str,bool _status);		
}