#pragma once

#include "DataStream.h"

namespace NGTech
{
	class CORE_API DataFileStream :
		public DataStream
	{
	public:
		DataFileStream();
		DataFileStream(std::ifstream* _stream);
		virtual ~DataFileStream();

	private:
		std::ifstream* mFileStream;
	};

} // namespace NGTech