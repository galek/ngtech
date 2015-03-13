/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef __MYGUI_DATA_STREAM_HOLDER_H__
#define __MYGUI_DATA_STREAM_HOLDER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_DataManager.h"

#include "../../../Src/Core/inc/VFS.h"
#include "../../../Src/Core/inc/CoreManager.h"

namespace MyGUI
{

	class DataStreamHolder
	{
	public:
		DataStreamHolder(IDataStream* _data) :
			mData(_data)
		{
		}

		~DataStreamHolder()
		{
			NGTech::GetVFS()->freeData(mData);
		}

		IDataStream* getData()
		{
			return mData;
		}

	private:
		IDataStream* mData;
	};

} // namespace MyGUI

#endif // __MYGUI_DATA_STREAM_HOLDER_H__
