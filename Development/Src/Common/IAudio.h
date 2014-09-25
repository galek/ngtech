/* Copyright (C) 2006-2013, NG Games Ltd. All rights reserved.
*
* File:    IAudio.h
* Desc:    Audio interface.
* Version: 1.0
* Author:  Nick Galko <galek@nggames.com>
*
* This file is part of the NGTech (http://nggames.com/).
*
* Your use and or redistribution of this software in source and / or
* binary form, with or without modification, is subject to: (i) your
* ongoing acceptance of and compliance with the terms and conditions of
* the NGTech License Agreement; and (ii) your inclusion of this notice
* in any version of this software that you use or redistribute.
* A copy of the NGTech License Agreement is available by contacting
* NG Games Ltd. at http://nggames.com/
*/

#pragma once

//***************************************************************************

namespace NGTech {
	class Vec3;
	//---------------------------------------------------------------------------
	//Desc: Engine`s main sound system. Created one time
	//---------------------------------------------------------------------------
	struct I_Audio {
	public:
		virtual void initialise() = 0;
		virtual String getVendor() = 0;
		virtual String getRenderer() = 0;
		virtual String getVersion() = 0;
		virtual String getExtensions() = 0;

		virtual void setListener(const Vec3 &pos, const Vec3 &dir) = 0;
	};

};