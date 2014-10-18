#include "CorePrivate.h"
//***************************************************************************
#include <vector>
//***************************************************************************

namespace NGTech {

	bool CORE_API saveBinary
		(const String & _file,
		unsigned int const & Format,
		std::vector<unsigned char> const & Data,
		int const & Size
		)
	{
		FILE* File = fopen(_file.c_str(), "wb");

		if (File)
		{
			fwrite(&Format, sizeof(unsigned int const), 1, File);
			fwrite(&Size, sizeof(Size), 1, File);
			fwrite(&Data[0], Size, 1, File);
			fclose(File);
			return true;
		}
		return false;
	}

	bool CORE_API loadBinary
		(const std::string & _file,
		unsigned int & Format,
		std::vector<unsigned char> & Data,
		int & Size)
	{
		FILE* File = fopen(_file.c_str(), "rb");

		if (File)
		{
			fread(&Format, sizeof(unsigned int), 1, File);
			fread(&Size, sizeof(Size), 1, File);
			Data.resize(Size);
			fread(&Data[0], Size, 1, File);
			fclose(File);
			return true;
		}
		return false;
	}
}