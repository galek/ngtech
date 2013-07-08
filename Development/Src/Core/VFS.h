/*!
	@file
	@author		George Evmenov
	@date		07/2009
*/

#ifndef __DATA_MANAGER_H__
#define __DATA_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "..\Common\IDataManager.h"

namespace NGEngine
{

	class FileSystem :
		public Common::DataManager
	{
	public:
		FileSystem();

		void initialise();
		void shutdown();

		static FileSystem& getInstance()
		{
			return *getInstancePtr();
		}
		static FileSystem* getInstancePtr()
		{
			return static_cast<FileSystem*>(DataManager::getInstancePtr());
		}

		/** @see DataManager::getData(const std::string& _name) */
		virtual Common::IDataStream* getData(const std::string& _name);

		/** @see DataManager::isDataExist(const std::string& _name) */
		virtual bool isDataExist(const std::string& _name);

		/** @see DataManager::getDataListNames(const std::string& _pattern) */
		virtual const VectorString& getDataListNames(const std::string& _pattern);

		/** @see DataManager::getDataPath(const std::string& _name) */
		virtual const std::string& getDataPath(const std::string& _name);

	/*internal:*/
		void addResourceLocation(const std::string& _name, bool _recursive);

	private:
		struct ArhivInfo
		{
			std::wstring name;
			bool recursive;
		};
		typedef std::vector<ArhivInfo> VectorArhivInfo;
		VectorArhivInfo mPaths;

		bool mIsInitialise;
	};

} // namespace MyGUI

#endif // __MYGUI_OPENGL_DATA_MANAGER_H__
