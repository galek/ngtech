#include "CorePrivate.h"

namespace NGTech
{
	bool DemoStartCheck()
	{
#ifdef _DEMO_BUILD
#if PLATFORM_OS == PLATFORM_OS_WINDOWS
		SYSTEMTIME st;
		GetSystemTime(&st);

		if (st.wYear > EXPIRES_YEAR || (st.wYear == EXPIRES_YEAR && st.wMonth > EXPIRES_MONTH))
		{
			MessageBox(0, "This evaluation period has expired, go to www.ngtech.com to get a newer one!", "EVALUATION PERIOD HAS EXPIRED", MB_OK);
			return false;
		}
#else
		time_t t = time(0);   // get time now
		struct tm  * st = localtime(&t);

		if (st->tm_year > EXPIRES_YEAR || (st->tm_year == EXPIRES_YEAR && st->tm_mon > EXPIRES_MONTH))
		{
			printf("This evaluation period has expired, go to www.ngtech.com to get a newer one!");
			return false;
		}
#endif
#endif

		return true;
	}
}