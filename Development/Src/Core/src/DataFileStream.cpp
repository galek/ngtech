#include "CorePrivate.h"
#include "DataFileStream.h"
#include <fstream>

namespace NGTech
{

	DataFileStream::DataFileStream() :
		DataStream(),
		mFileStream(nullptr)
	{
	}

	DataFileStream::DataFileStream(std::ifstream* _stream) :
		DataStream(_stream),
		mFileStream(_stream)
	{
	}

	DataFileStream::~DataFileStream()
	{
		if (mFileStream != nullptr)
		{
			mFileStream->close();
			delete mFileStream;
			mFileStream = nullptr;
		}
	}

} // namespace NGTech