//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2002 by Denton Woods
// Last modified: 05/26/2002 <--Y2K Compliant! =]
//
// Filename: src-IL/src/il_pxr.c
//
// Description: Reads from a Pxrar (.pxr) file.
//
//-----------------------------------------------------------------------------


#include "../include/il_internal.h"
#ifndef IL_NO_PXR
#include "../include/il_manip.h"
#include "../include/il_endian.h"


#ifdef _MSC_VER
#pragma pack(push, pxr_struct, 1)
#endif
typedef struct PIXHEAD
{
	ILushort	Signature;
	ILubyte		Reserved1[413];
	ILushort	Height;
	ILushort	Width;
	ILubyte		Reserved2[4];
	ILubyte		BppInfo;
	ILubyte		Reserved3[598];
} IL_PACKSTRUCT PIXHEAD;
#ifdef _MSC_VER
#pragma pack(pop, pxr_struct)
#endif



// Internal function used to load the Pxr.
ILboolean iLoadPxrInternal()
{
	ILushort	Width, Height;
	ILubyte		Bpp;

	Width = sizeof(PIXHEAD);

	iCurImage->io.devil_seek(iCurImage->io.handle, 416, IL_SEEK_SET);
	Height = GetLittleUShort(&iCurImage->io);
	Width = GetLittleUShort(&iCurImage->io);
	iCurImage->io.devil_seek(iCurImage->io.handle, 424, IL_SEEK_SET);
	Bpp = (ILubyte)iCurImage->io.devil_getc(iCurImage->io.handle);

	switch (Bpp)
	{
		case 0x08:
			ilTexImage(Width, Height, 1, 1, IL_LUMINANCE, IL_UNSIGNED_BYTE, NULL);
			break;
		case 0x0E:
			ilTexImage(Width, Height, 1, 3, IL_RGB, IL_UNSIGNED_BYTE, NULL);
			break;
		case 0x0F:
			ilTexImage(Width, Height, 1, 4, IL_RGBA, IL_UNSIGNED_BYTE, NULL);
			break;
		default:
			ilSetError(IL_INVALID_FILE_HEADER);
			return IL_FALSE;
	}

	iCurImage->io.devil_seek(iCurImage->io.handle, 1024, IL_SEEK_SET);
	iCurImage->io.read(iCurImage->io.handle, iCurImage->Data, 1, iCurImage->SizeOfData);
	iCurImage->Origin = IL_ORIGIN_UPPER_LEFT;

	return IL_TRUE;
}


#endif//IL_NO_PXR
