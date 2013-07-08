/*!
	@file
	@author		George Evmenov
	@date		02/2010
*/
/*
	This file is part of MyGUI.

	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __SINGLETON_H__
#define __SINGLETON_H__


namespace Common
{

#if MYGUI_COMPILER == MYGUI_COMPILER_MSVC || MYGUI_PLATFORM == MYGUI_PLATFORM_APPLE
	template <class T>
	class Singleton
#else
	template <class T>
	class MYGUI_EXPORT Singleton
#endif
	{
	public:
		typedef Singleton<T> Base;

		Singleton()
		{
//			assert(nullptr == msInstance, "Singleton instance " << getClassTypeName() << " already exsist");
			msInstance = static_cast<T*>(this);
		}

		virtual ~Singleton()
		{
//			assert(nullptr != msInstance, "Destroying Singleton instance " << getClassTypeName() << " before constructing it.");
			msInstance = nullptr;
		}

		static T& getInstance()
		{
//			assert(nullptr != getInstancePtr(), "Singleton instance " << getClassTypeName() << " was not created");
			return (*getInstancePtr());
		}

		static T* getInstancePtr()
		{
			return msInstance;
		}

		static const char* getClassTypeName()
		{
			return mClassTypeName;
		}

	private:
		static T* msInstance;

	protected:
		static const char* mClassTypeName;
	};

} // namespace MyGUI

#endif // __MYGUI_SINGLETON_H__
