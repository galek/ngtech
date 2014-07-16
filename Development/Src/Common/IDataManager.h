#pragma once

#include "Types.h"
#include "Singleton.h"

namespace NGTech
{
	class IDataStream
	{
	public:
		virtual ~IDataStream() { }

		virtual bool eof() = 0;
		virtual size_t size() = 0;
		virtual void readline(std::string& _source, unsigned int _delim = '\n') = 0;
		virtual size_t read(void* _buf, size_t _count) = 0;
	};

	class DataManager :public Singleton<DataManager>
	{
	public:

		/** Get data stream from specified resource name.
		@param _name Resource name (usually file name).
		*/
		virtual IDataStream* getData(const std::string& _name) = 0;

		/** Free data stream.
		@param _data Data stream.
		*/
		virtual void freeData(IDataStream* _data) = 0;

		/** Is data with specified name exist.
		@param _name Resource name.
		*/
		virtual bool isDataExist(const std::string& _name) = 0;

		/** Get all data names with names that matches pattern.
		@param _pattern Pattern to match (for example "*.layout").
		*/
		virtual const VectorString& getDataListNames(const std::string& _pattern) = 0;

		/** Get full path to data.
		@param _name Resource name.
		@return Return full path to specified data.
		For example getDataPath("My.layout") might return "C:\path\to\project\data\My.layout"
		*/
		virtual const std::string& getDataPath(const std::string& _name) = 0;
	};
	template <> DataManager* Singleton<DataManager>::msInstance = nullptr;
	template <> const char* Singleton<DataManager>::mClassTypeName = "DataManager";
} // namespace Common
