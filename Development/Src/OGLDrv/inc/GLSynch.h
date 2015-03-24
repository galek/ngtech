#pragma once

#include "GLExtensions.h"

namespace NGTech {

	struct GLSynch
	{
		/*
		*/
		static void waitSync(GLsync &sync);

		static void waitSync(GLsync sync[3]);

		static void waitSync(GLsync sync[3], int &offset, int flush, int size);

		static void fenceSync(GLsync sync[3], int offset, int flush, int size);
	};
}