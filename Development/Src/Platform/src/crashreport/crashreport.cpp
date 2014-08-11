#include "crashreport/windows/handler/exception_handler.h"
#include "crashreport/windows/crash_generation/client_info.h"
#include "crashreport/windows/crash_generation/crash_generation_server.h"

namespace NGTech
{
	static ExceptionHandler* handler = NULL;
	static CrashGenerationServer* crash_server = NULL;
	const wchar_t kPipeName[] = L"/./NGTechCrashService";

	void CrashServerStart() {
		// Do not create another instance of the server.
		if (crash_server) {
			return;
		}

		std::wstring dump_path = L"../logs/";

		_wmkdir(dump_path.c_str());

		crash_server = new CrashGenerationServer(kPipeName,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			true,
			&dump_path);

		if (!crash_server->Start()) {
			delete crash_server;
			crash_server = NULL;
		}
	}

	void CrashServerStop() {
		delete crash_server;
		crash_server = NULL;
	}

	bool ShowDumpResults(const wchar_t* dump_path,
		const wchar_t* minidump_id,
		void* context,
		EXCEPTION_POINTERS* exinfo,
		MDRawAssertionInfo* assertion,
		bool succeeded)
	{
		std::string ex = "General Protection Fault!";
#ifdef _WIN32
		MessageBoxA(NULL, ex.c_str(), "Critical Error", MB_OK);
#endif
#ifdef _NDEBUG
		exit(0);
#endif
		return succeeded;
	}

	bool InitMiniDump()
	{
#ifndef _DEBUG
		CrashServerStart();
#ifdef _WIN32
		// This is needed for CRT to not show dialog for invalid param
		// failures and instead let the code handle it.
		_CrtSetReportMode(_CRT_ASSERT, 0);

		handler = new ExceptionHandler(L"../logs/",
			NULL,
			ShowDumpResults,
			NULL,
			ExceptionHandler::HANDLER_ALL,
			MiniDumpNormal,
			kPipeName,
			NULL);
		if (!handler)
			return false;
#endif
#endif
		return true;
	}
}