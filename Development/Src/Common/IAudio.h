/***************************************************************************
*   Copyright (C) 2006 by AST   *
*   tsyplyaev@gmail.com   *
*   ICQ: 279-533-134                          *
*   This is a part of work done by AST.       *
*   If you want to use it, please contact me. *
***************************************************************************/

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