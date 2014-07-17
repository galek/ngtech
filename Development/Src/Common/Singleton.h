#pragma once

namespace NGTech
{
	template <class T>
	class Singleton
	{
	public:
		typedef Singleton<T> Base;

		Singleton()
		{
			msInstance = static_cast<T*>(this);
		}

		virtual ~Singleton()
		{
			msInstance = nullptr;
		}

		static T& getInstance()
		{
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
		static const char* mClassTypeName;
	};

} // namespace NGTech