/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "CorePrivate.h"

#include <fstream>

#include "DataStream.h"

namespace NGTech
{

	DataStream::DataStream() :
		mStream(nullptr),
		mSize((size_t)-1)
	{
	}

	DataStream::DataStream(std::ifstream* _stream) :
		mStream(_stream),
		mSize((size_t)-1)
	{
	}

	DataStream::~DataStream()
	{
	}

	size_t DataStream::size()
	{
		if (mStream == nullptr) return 0;
		if (mSize == (size_t)-1)
		{
			mStream->seekg(0, std::ios::end);
			mSize = (size_t)mStream->tellg();
			mStream->seekg(0, std::ios::beg);
		}
		return mSize;
	}

	bool DataStream::eof()
	{
		return mStream == nullptr ? true : mStream->eof();
	}

	void DataStream::readline(std::string& _source, unsigned int _delim)
	{
		if (mStream == nullptr) return;
		std::getline(*mStream, _source, (char)_delim);
	}

	size_t DataStream::read(void* _buf, size_t _count)
	{
		if (mStream == nullptr) return 0;
#if PLATFORM_OS == PLATFORM_OS_WINDOWS
		size_t count = min(size(), _count);
#else
		size_t count = std::min(size(), _count);
#endif
		mStream->read((char*)_buf, count);
		return count;
	}

} // namespace NGTech