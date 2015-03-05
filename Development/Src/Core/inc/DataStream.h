#pragma once

#include "coredll.h"
#include "../../Common/IDataManager.h"

namespace NGTech
{
	class CORE_API DataStream :
		public IDataStream
	{
	public:
		DataStream();
		DataStream(std::ifstream* _stream);
		virtual ~DataStream();

		virtual bool eof();
		virtual size_t size();
		virtual void readline(std::string& _source, unsigned int _delim);
		virtual size_t read(void* _buf, size_t _count);

	protected:
		std::ifstream* mStream;
		size_t mSize;
	};

} // namespace NGTech