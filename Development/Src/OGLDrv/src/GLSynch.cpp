#include "RenderPrivate.h"
//***************************************************************************
#include <algorithm>
//***************************************************************************
#include "GLSystem.h"
#include "GLVBO.h"
#include "GLSynch.h"
//***************************************************************************

namespace NGTech {

	void GLSynch::waitSync(GLsync &sync) {
		if (sync == NULL) return;
		int ret = glClientWaitSync(sync, 0, 0);
		while (ret != GL_ALREADY_SIGNALED && ret != GL_CONDITION_SATISFIED) {
			ret = glClientWaitSync(sync, GL_SYNC_FLUSH_COMMANDS_BIT, 1000000000);
			assert(ret != GL_WAIT_FAILED && "GLExt::waitSync(): waiting failed");
		}
		glDeleteSync(sync);
		sync = NULL;
	}

	void GLSynch::waitSync(GLsync sync[3]) {
		if (sync[0]) waitSync(sync[0]);
		if (sync[1]) waitSync(sync[1]);
		if (sync[2]) waitSync(sync[2]);
	}

	void GLSynch::waitSync(GLsync sync[3], int &offset, int flush, int size) {
		int size_3 = size / 3;
		int size_32 = size_3 * 2;
		if (offset + flush > size) {
			waitSync(sync[0]);
			offset = 0;
		}
		else if (offset <= size_3 && offset + flush > size_3) {
			waitSync(sync[1]);
			offset = size_3;
		}
		else if (offset <= size_32 && offset + flush > size_32) {
			waitSync(sync[2]);
			offset = size_32;
		}
	}

	void GLSynch::fenceSync(GLsync sync[3], int offset, int flush, int size) {
		int size_3 = size / 3;
		int size_32 = size_3 * 2;
		if (offset == 0) {
			if (sync[2]) glDeleteSync(sync[2]);
			sync[2] = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
		}
		else if (offset <= size_3 && offset + flush > size_3) {
			if (sync[0]) glDeleteSync(sync[0]);
			sync[0] = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
		}
		else if (offset <= size_32 && offset + flush > size_32) {
			if (sync[1]) glDeleteSync(sync[1]);
			sync[1] = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
		}
	}
}