#include "EnginePrivate.h"

namespace NGTech
{
	/**
	*/
	void R_ScreenshotFilename(int &lastNumber, const char *base, std::string &fileName) {
		int	a, b, c, d, e;

		char buff[256];

		lastNumber++;
		if (lastNumber > 99999) {
			lastNumber = 99999;
		}
		for (; lastNumber < 99999; lastNumber++) {
			int	frac = lastNumber;

			a = frac / 10000;
			frac -= a * 10000;
			b = frac / 1000;
			frac -= b * 1000;
			c = frac / 100;
			frac -= c * 100;
			d = frac / 10;
			frac -= d * 10;
			e = frac;



			sprintf(buff, "%s%i%i%i%i%i.tga", base, a, b, c, d, e);
			fileName = buff;
			if (lastNumber == 99999) {
				break;
			}
			
			bool len = VFile::IsDataExist(fileName.c_str(),false);
			if (!len) {
				break;
			}
			// check again...
		}
	}

	/**
	*/
#define	MAX_BLENDS	256	// to keep the accumulation in shorts
	ENGINE_API void API_Make_ScreenShot() {
		static int lastNumber = 0;
		std::string checkname;

		int width = GetCvars()->r_width;
		int height = GetCvars()->r_height;
		int	blends = 1;

		R_ScreenshotFilename(lastNumber, "../userData/screenshots/shot", checkname);

		GetRender()->WriteScreenshot(checkname.c_str());

		LogPrintf("Wrote %s\n", checkname.c_str());
	}
}