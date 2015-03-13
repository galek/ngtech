//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2009 by Denton Woods
// Last modified: 03/07/2009
//
// Filename: src-IL/src/il_raw.c
//
// Description: "Raw" file functions
//
//-----------------------------------------------------------------------------


#include "../include/il_internal.h"
#ifndef IL_NO_RAW


ILboolean iLoadRawInternal(void);
ILboolean iSaveRawInternal(void);


// Internal function to load a raw image
ILboolean iLoadRawInternal()
{
	if (iCurImage == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}


	iCurImage->Width = GetLittleUInt(&iCurImage->io);

	iCurImage->Height = GetLittleUInt(&iCurImage->io);

	iCurImage->Depth = GetLittleUInt(&iCurImage->io);

	iCurImage->Bpp = (ILubyte)iCurImage->io.devil_getc(iCurImage->io.handle);

	if (iCurImage->io.read(iCurImage->io.handle, &iCurImage->Bpc, 1, 1) != 1)
		return IL_FALSE;

	if (!ilTexImage(iCurImage->Width, iCurImage->Height, iCurImage->Depth, iCurImage->Bpp, 0, ilGetTypeBpc(iCurImage->Bpc), NULL)) {
		return IL_FALSE;
	}
	iCurImage->Origin = IL_ORIGIN_LOWER_LEFT;

	// Tries to read the correct amount of data
	if (iCurImage->io.read(iCurImage->io.handle, iCurImage->Data, 1, iCurImage->SizeOfData) < iCurImage->SizeOfData)
		return IL_FALSE;

	if (ilIsEnabled(IL_ORIGIN_SET)) {
		iCurImage->Origin = ilGetInteger(IL_ORIGIN_MODE);
	}
	else {
		iCurImage->Origin = IL_ORIGIN_UPPER_LEFT;
	}

	if (iCurImage->Bpp == 1)
		iCurImage->Format = IL_LUMINANCE;
	else if (iCurImage->Bpp == 3)
		iCurImage->Format = IL_RGB;
	else  // 4
		iCurImage->Format = IL_RGBA;

	return ilFixImage();
}


// Internal function used to load the raw data.
ILboolean iSaveRawInternal()
{
	if (iCurImage == NULL) {
		ilSetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	SaveLittleUInt(&iCurImage->io, iCurImage->Width);
	SaveLittleUInt(&iCurImage->io, iCurImage->Height);
	SaveLittleUInt(&iCurImage->io, iCurImage->Depth);
	iCurImage->io.devil_putc(iCurImage->Bpp, iCurImage->io.handle);
	iCurImage->io.devil_putc(iCurImage->Bpc, iCurImage->io.handle);
	iCurImage->io.write(iCurImage->Data, 1, iCurImage->SizeOfData, iCurImage->io.handle);

	return IL_TRUE;
}


#endif//IL_NO_RAW
