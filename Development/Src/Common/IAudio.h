/* Copyright (C) 2006-2013, Dream In Gate Ltd. All rights reserved.
*
* File:    IAudio.h
* Desc:    Audio interface.
* Version: 1.0
* Author:  Nick Galko <galek@vegaengine.com>
*
* This file is part of the NGTech (http://vegaengine.com/).
*
* Your use and or redistribution of this software in source and / or
* binary form, with or without modification, is subject to: (i) your
* ongoing acceptance of and compliance with the terms and conditions of
* the VEGA License Agreement; and (ii) your inclusion of this notice
* in any version of this software that you use or redistribute.
* A copy of the VEGA License Agreement is available by contacting
* Dream In Gate Ltd. at http://vegaengine.com/
*/

#pragma once

//***************************************************************************

namespace VEGA {
	class Vec3;
	//---------------------------------------------------------------------------
	//Desc: Engine`s main sound system. Created one time
	//---------------------------------------------------------------------------
	class IAudio {
	public:
		virtual void initialise()=0;
		virtual std::string getVendor() = 0;
		virtual std::string getRenderer() = 0;
		virtual std::string getVersion() = 0;
		virtual std::string getExtensions() = 0;

		virtual void setListener(const Vec3 &pos, const Vec3 &dir) = 0;
	};

};