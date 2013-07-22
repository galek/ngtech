//************************************************************************** 
//* Export.cpp	- Ascii File Exporter
//* 
//* By Christer Janson
//* Kinetix Development
//*
//* January 20, 1997 CCJ Initial coding
//*
//* This module contains the main export functions.
//*
//* Copyright (c) 1997, All Rights Reserved. 
//***************************************************************************

#include "asciiexp.h"

/****************************************************************************

  Global output [Scene info]
  
****************************************************************************/

// Dump some global animation information.
void AsciiExp::ExportGlobalInfo()
{
	Interval range = ip->GetAnimRange();

	struct tm *newtime;
	time_t aclock;

	time( &aclock );
	newtime = localtime(&aclock);

	TSTR today = _tasctime(newtime);	// The date string has a \n appended.
	today.remove(today.length()-1);		// Remove the \n

	// Start with a file identifier and format version
	_ftprintf(pStream, _T("%s\t%d\n"), ID_FILEID, VERSION);

	Interface14 *iface = GetCOREInterface14();
	UINT codepage  = iface-> DefaultTextSaveCodePage(true);
	TSTR versionName = GetString(IDS_VERSIONSTRING);
	const char* versionName_locale = versionName.ToCP(codepage);

	// Text token describing the above as a comment.
	_ftprintf(pStream, _T("%s \"%hs  %1.2f - %s\"\n"), ID_COMMENT, versionName_locale, VERSION / 100.0f, today);

	_ftprintf(pStream, _T("%s {\n"), ID_SCENE);

	TSTR curtFileName = FixupName(ip->GetCurFileName());
	const char* curtFileName_locale = curtFileName.ToCP(codepage);
	_ftprintf(pStream, _T("\t%s \"%hs\"\n"), ID_FILENAME, curtFileName_locale);
	_ftprintf(pStream, _T("\t%s %d\n"), ID_FIRSTFRAME, range.Start() / GetTicksPerFrame());
	_ftprintf(pStream, _T("\t%s %d\n"), ID_LASTFRAME, range.End() / GetTicksPerFrame());
	_ftprintf(pStream, _T("\t%s %d\n"), ID_FRAMESPEED, GetFrameRate());
	_ftprintf(pStream, _T("\t%s %d\n"), ID_TICKSPERFRAME, GetTicksPerFrame());
	
	Texmap* env = ip->GetEnvironmentMap();

	Control* ambLightCont;
	Control* bgColCont;
	
	if (env) {
		// Output environment texture map
		DumpTexture(env, Class_ID(0,0), 0, 1.0f, 0);
	}
	else {
		// Output background color
		bgColCont = ip->GetBackGroundController();
		if (bgColCont && bgColCont->IsAnimated()) {
			// background color is animated.
			_ftprintf(pStream, _T("\t%s {\n"), ID_ANIMBGCOLOR);
			
			DumpPoint3Keys(bgColCont, 0);
				
			_ftprintf(pStream, _T("\t}\n"));
		}
		else {
			// Background color is not animated
			Color bgCol = ip->GetBackGround(GetStaticFrame(), FOREVER);
			_ftprintf(pStream, _T("\t%s %s\n"), ID_STATICBGCOLOR, Format(bgCol));
		}
	}
	
	// Output ambient light
	ambLightCont = ip->GetAmbientController();
	if (ambLightCont && ambLightCont->IsAnimated()) {
		// Ambient light is animated.
		_ftprintf(pStream, _T("\t%s {\n"), ID_ANIMAMBIENT);
		
		DumpPoint3Keys(ambLightCont, 0);
		
		_ftprintf(pStream, _T("\t}\n"));
	}
	else {
		// Ambient light is not animated
		Color ambLight = ip->GetAmbient(GetStaticFrame(), FOREVER);
		_ftprintf(pStream, _T("\t%s %s\n"), ID_STATICAMBIENT, Format(ambLight));
	}

	_ftprintf(pStream, _T("}\n"));
}

/****************************************************************************

  GeomObject output
  
****************************************************************************/

void AsciiExp::ExportGeomObject(INode* node, int indentLevel)
{
	ObjectState os = node->EvalWorldState(GetStaticFrame());
	if (!os.obj)
		return;
	
	// Targets are actually geomobjects, but we will export them
	// from the camera and light objects, so we skip them here.
	if (os.obj->ClassID() == Class_ID(TARGET_CLASS_ID, 0))
		return;
	
	
	TSTR indent = GetIndent(indentLevel);
	
	ExportNodeHeader(node, ID_GEOMETRY, indentLevel);
	
	ExportNodeTM(node, indentLevel);
	
	if (GetIncludeMesh()) {
		ExportMesh(node, GetStaticFrame(), indentLevel);
	}

	// Node properties (only for geomobjects)
	_ftprintf(pStream, _T("%s\t%s %d\n"), indent.data(), ID_PROP_MOTIONBLUR, node->MotBlur());
	_ftprintf(pStream, _T("%s\t%s %d\n"), indent.data(), ID_PROP_CASTSHADOW, node->CastShadows());
	_ftprintf(pStream, _T("%s\t%s %d\n"), indent.data(), ID_PROP_RECVSHADOW, node->RcvShadows());

	if (GetIncludeAnim()) {
		ExportAnimKeys(node, indentLevel);
	}

	// Export the visibility track
	Control* visCont = node->GetVisController();
	if (visCont) {
		_ftprintf(pStream, _T("%s\t%s {\n"), indent.data(), ID_VISIBILITY_TRACK);
		DumpFloatKeys(visCont, indentLevel);
		_ftprintf(pStream, _T("\t}\n"));
	}

	if (GetIncludeMtl()) {
		ExportMaterial(node, indentLevel);
	}

	if (GetIncludeMeshAnim()) {
		ExportAnimMesh(node, indentLevel);
	}
	
	if (GetIncludeIKJoints()) {
		ExportIKJoints(node, indentLevel);
	}
	
	_ftprintf(pStream, _T("%s}\n"), indent.data());
}

/****************************************************************************

  Shape output
  
****************************************************************************/

void AsciiExp::ExportShapeObject(INode* node, int indentLevel)
{
	ExportNodeHeader(node, ID_SHAPE, indentLevel);
	ExportNodeTM(node, indentLevel);
	TimeValue t = GetStaticFrame();
	Matrix3 tm = node->GetObjTMAfterWSM(t);

	TSTR indent = GetIndent(indentLevel);
	
	ObjectState os = node->EvalWorldState(t);
	if (!os.obj || os.obj->SuperClassID()!=SHAPE_CLASS_ID) {
		_ftprintf(pStream, _T("%s}\n"), indent.data());
		return;
	}
	
	ShapeObject* shape = (ShapeObject*)os.obj;
	PolyShape pShape;
	int numLines;

	// We will output ahspes as a collection of polylines.
	// Each polyline contains collection of line segments.
	shape->MakePolyShape(t, pShape);
	numLines = pShape.numLines;
	
	_ftprintf(pStream, _T("%s\t%s %d\n"), indent.data(), ID_SHAPE_LINECOUNT, numLines);
	
	for(int poly = 0; poly < numLines; poly++) {
		_ftprintf(pStream, _T("%s\t%s %d {\n"), indent.data(), ID_SHAPE_LINE, poly);
		DumpPoly(&pShape.lines[poly], tm, indentLevel);
		_ftprintf(pStream, _T("%s\t}\n"), indent.data());
	}
	
	if (GetIncludeAnim()) {
		ExportAnimKeys(node, indentLevel);
	}
	
	_ftprintf(pStream, _T("%s}\n"), indent.data());
}

void AsciiExp::DumpPoly(PolyLine* line, Matrix3 tm, int indentLevel)
{
	int numVerts = line->numPts;
	
	TSTR indent = GetIndent(indentLevel);
	
	if(line->IsClosed()) {
		_ftprintf(pStream, _T("%s\t\t%s\n"), indent.data(), ID_SHAPE_CLOSED);
	}
	
	_ftprintf(pStream, _T("%s\t\t%s %d\n"), indent.data(), ID_SHAPE_VERTEXCOUNT, numVerts);
	
	// We differ between true and interpolated knots
	for (int i=0; i<numVerts; i++) {
		PolyPt* pt = &line->pts[i];
		if (pt->flags & POLYPT_KNOT) {
			_ftprintf(pStream, _T("%s\t\t%s\t%d\t%s\n"), indent.data(), ID_SHAPE_VERTEX_KNOT, i,
				Format(tm * pt->p));
		}
		else {
			_ftprintf(pStream, _T("%s\t\t%s\t%d\t%s\n"), indent.data(), ID_SHAPE_VERTEX_INTERP,
				i, Format(tm * pt->p));
		}
		
	}
}

/****************************************************************************

  Light output
  
****************************************************************************/

void AsciiExp::ExportLightObject(INode* node, int indentLevel)
{
	TimeValue t = GetStaticFrame();
	TSTR indent = GetIndent(indentLevel);

	ExportNodeHeader(node, ID_LIGHT, indentLevel);
	
	ObjectState os = node->EvalWorldState(t);
	if (!os.obj) {
		_ftprintf(pStream, _T("%s}\n"), indent.data());
		return;
	}
	
	GenLight* light = (GenLight*)os.obj;
	struct LightState ls;
	Interval valid = FOREVER;
	Interval animRange = ip->GetAnimRange();

	light->EvalLightState(t, valid, &ls);

	// This is part os the lightState, but it doesn't
	// make sense to output as an animated setting so
	// we dump it outside of ExportLightSettings()

	_ftprintf(pStream, _T("%s\t%s "), indent.data(), ID_LIGHT_TYPE);
	switch(ls.type) {
	case OMNI_LIGHT:  _ftprintf(pStream, _T("%s\n"), ID_LIGHT_TYPE_OMNI); break;
	case TSPOT_LIGHT: _ftprintf(pStream, _T("%s\n"), ID_LIGHT_TYPE_TARG);  break;
	case DIR_LIGHT:   _ftprintf(pStream, _T("%s\n"), ID_LIGHT_TYPE_DIR); break;
	case FSPOT_LIGHT: _ftprintf(pStream, _T("%s\n"), ID_LIGHT_TYPE_FREE); break;
	}

	ExportNodeTM(node, indentLevel);
	// If we have a target object, export Node TM for the target too.
	INode* target = node->GetTarget();
	if (target) {
		ExportNodeTM(target, indentLevel);
	}

	int shadowMethod = light->GetShadowMethod();
	_ftprintf(pStream, _T("%s\t%s %s\n"), indent.data(), ID_LIGHT_SHADOWS,
			shadowMethod == LIGHTSHADOW_NONE ? ID_LIGHT_SHAD_OFF :
			shadowMethod == LIGHTSHADOW_MAPPED ? ID_LIGHT_SHAD_MAP :
			ID_LIGHT_SHAD_RAY);

	
	_ftprintf(pStream, _T("%s\t%s %s\n"), indent.data(), ID_LIGHT_USELIGHT, Format(light->GetUseLight()));
	
	_ftprintf(pStream, _T("%s\t%s %s\n"), indent.data(), ID_LIGHT_SPOTSHAPE, 
		light->GetSpotShape() == RECT_LIGHT ? ID_LIGHT_SHAPE_RECT : ID_LIGHT_SHAPE_CIRC);

	_ftprintf(pStream, _T("%s\t%s %s\n"), indent.data(), ID_LIGHT_USEGLOBAL, Format(light->GetUseGlobal()));
	_ftprintf(pStream, _T("%s\t%s %s\n"), indent.data(), ID_LIGHT_ABSMAPBIAS, Format(light->GetAbsMapBias()));
	_ftprintf(pStream, _T("%s\t%s %s\n"), indent.data(), ID_LIGHT_OVERSHOOT, Format(light->GetOvershoot()));

	ExclList* el = light->GetExclList();  // DS 8/31/00 . switched to NodeIDTab from NameTab
	if (el->Count()) {
		_ftprintf(pStream, _T("%s\t%s {\n"), indent.data(), ID_LIGHT_EXCLUSIONLIST);
		_ftprintf(pStream, _T("%s\t\t%s %s\n"), indent.data(), ID_LIGHT_NUMEXCLUDED, Format(el->Count()));
		_ftprintf(pStream, _T("%s\t\t%s %s\n"), indent.data(), ID_LIGHT_EXCLINCLUDE, Format(el->TestFlag(NT_INCLUDE)));
		_ftprintf(pStream, _T("%s\t\t%s %s\n"), indent.data(), ID_LIGHT_EXCL_AFFECT_ILLUM, Format(el->TestFlag(NT_AFFECT_ILLUM)));
		_ftprintf(pStream, _T("%s\t\t%s %s\n"), indent.data(), ID_LIGHT_EXCL_AFFECT_SHAD, Format(el->TestFlag(NT_AFFECT_SHADOWCAST)));
		Interface14 *iface = GetCOREInterface14();
	UINT codepage  = iface-> DefaultTextSaveCodePage(true);
		for (int nameid = 0; nameid < el->Count(); nameid++) {
			INode *n = (*el)[nameid];	// DS 8/31/00
			if (n)
			{
				TSTR name = n->GetName();
				const char* name_locale = name.ToCP(codepage);
				_ftprintf(pStream, _T("%s\t\t%s \"%hs\"\n"), indent.data(), ID_LIGHT_EXCLUDED,name_locale);
			}
			}
		_ftprintf(pStream, _T("%s\t}\n"), indent.data());
	}

	// Export light settings for frame 0
	ExportLightSettings(&ls, light, t, indentLevel);

	// Export animated light settings
	if (!valid.InInterval(animRange) && GetIncludeCamLightAnim()) {
		_ftprintf(pStream, _T("%s\t%s {\n"), indent.data(), ID_LIGHT_ANIMATION);

		TimeValue t = animRange.Start();
		
		while (1) {
			valid = FOREVER; // Extend the validity interval so the camera can shrink it.
			light->EvalLightState(t, valid, &ls);

			t = valid.Start() < animRange.Start() ? animRange.Start() : valid.Start();
			
			// Export the light settings at this frame
			ExportLightSettings(&ls, light, t, indentLevel+1);
			
			if (valid.End() >= animRange.End()) {
				break;
			}
			else {
				t = (valid.End()/GetTicksPerFrame()+GetMeshFrameStep()) * GetTicksPerFrame();
			}
		}

		_ftprintf(pStream, _T("%s\t}\n"), indent.data());
	}

	// Export animation keys for the light node
	if (GetIncludeAnim()) {
		ExportAnimKeys(node, indentLevel);
		
		// If we have a target, export animation keys for the target too.
		if (target) {
			ExportAnimKeys(target, indentLevel);
		}
	}
	
	_ftprintf(pStream, _T("%s}\n"), indent.data());
}

void AsciiExp::ExportLightSettings(LightState* ls, GenLight* light, TimeValue t, int indentLevel)
{
	TSTR indent = GetIndent(indentLevel);

	_ftprintf(pStream, _T("%s\t%s {\n"), indent.data(), ID_LIGHT_SETTINGS);

	// Frame #
	_ftprintf(pStream, _T("%s\t\t%s %d\n"),indent.data(), ID_TIMEVALUE, t);

	_ftprintf(pStream, _T("%s\t\t%s %s\n"), indent.data(), ID_LIGHT_COLOR, Format(ls->color));
	_ftprintf(pStream, _T("%s\t\t%s %s\n"), indent.data(), ID_LIGHT_INTENS, Format(ls->intens));
	_ftprintf(pStream, _T("%s\t\t%s %s\n"), indent.data(), ID_LIGHT_ASPECT, Format(ls->aspect));
	
	if (ls->type != OMNI_LIGHT) {
		_ftprintf(pStream, _T("%s\t\t%s %s\n"), indent.data(), ID_LIGHT_HOTSPOT, Format(ls->hotsize));
		_ftprintf(pStream, _T("%s\t\t%s %s\n"), indent.data(), ID_LIGHT_FALLOFF, Format(ls->fallsize));
	}
	if (ls->type != DIR_LIGHT && ls->useAtten) {
		_ftprintf(pStream, _T("%s\t\t%s %s\n"), indent.data(), ID_LIGHT_ATTNSTART, Format(ls->attenStart));
		_ftprintf(pStream, _T("%s\t\t%s %s\n"), indent.data(), ID_LIGHT_ATTNEND,	Format(ls->attenEnd));
	}

	_ftprintf(pStream, _T("%s\t\t%s %s\n"), indent.data(), ID_LIGHT_TDIST, Format(light->GetTDist(t, FOREVER)));
	_ftprintf(pStream, _T("%s\t\t%s %s\n"), indent.data(), ID_LIGHT_MAPBIAS, Format(light->GetMapBias(t, FOREVER)));
	_ftprintf(pStream, _T("%s\t\t%s %s\n"), indent.data(), ID_LIGHT_MAPRANGE, Format(light->GetMapRange(t, FOREVER)));
	_ftprintf(pStream, _T("%s\t\t%s %s\n"), indent.data(), ID_LIGHT_MAPSIZE, Format(light->GetMapSize(t, FOREVER)));
	_ftprintf(pStream, _T("%s\t\t%s %s\n"), indent.data(), ID_LIGHT_RAYBIAS, Format(light->GetRayBias(t, FOREVER)));

	_ftprintf(pStream, _T("%s\t}\n"), indent.data());
}


/****************************************************************************

  Camera output
  
****************************************************************************/

void AsciiExp::ExportCameraObject(INode* node, int indentLevel)
{
	TSTR indent = GetIndent(indentLevel);

	ExportNodeHeader(node, ID_CAMERA, indentLevel);

	INode* target = node->GetTarget();
	if (target) {
		_ftprintf(pStream, _T("%s\t%s %s\n"), indent.data(), ID_CAMERA_TYPE, ID_CAMERATYPE_TARGET);
	}
	else {
		_ftprintf(pStream, _T("%s\t%s %s\n"), indent.data(), ID_CAMERA_TYPE, ID_CAMERATYPE_FREE);
	}


	ExportNodeTM(node, indentLevel);
	// If we have a target object, export animation keys for the target too.
	if (target) {
		ExportNodeTM(target, indentLevel);
	}
	
	CameraState cs;
	TimeValue t = GetStaticFrame();
	Interval valid = FOREVER;
	// Get animation range
	Interval animRange = ip->GetAnimRange();
	
	ObjectState os = node->EvalWorldState(t);
	CameraObject *cam = (CameraObject *)os.obj;
	
	cam->EvalCameraState(t,valid,&cs);
	
	ExportCameraSettings(&cs, cam, t, indentLevel);

	// Export animated camera settings
	if (!valid.InInterval(animRange) && GetIncludeCamLightAnim()) {

		_ftprintf(pStream, _T("%s\t%s {\n"), indent.data(), ID_CAMERA_ANIMATION);

		TimeValue t = animRange.Start();
		
		while (1) {
			valid = FOREVER; // Extend the validity interval so the camera can shrink it.
			cam->EvalCameraState(t,valid,&cs);

			t = valid.Start() < animRange.Start() ? animRange.Start() : valid.Start();
			
			// Export the camera settings at this frame
			ExportCameraSettings(&cs, cam, t, indentLevel+1);
			
			if (valid.End() >= animRange.End()) {
				break;
			}
			else {
				t = (valid.End()/GetTicksPerFrame()+GetMeshFrameStep()) * GetTicksPerFrame();
			}
		}

		_ftprintf(pStream, _T("%s\t}\n"), indent.data());
	}
	
	// Export animation keys for the light node
	if (GetIncludeAnim()) {
		ExportAnimKeys(node, indentLevel);
		
		// If we have a target, export animation keys for the target too.
		if (target) {
			ExportAnimKeys(target, indentLevel);
		}
	}
	
	_ftprintf(pStream, _T("%s}\n"), indent.data());
}

void AsciiExp::ExportCameraSettings(CameraState* cs, CameraObject* cam, TimeValue t, int indentLevel)
{
	TSTR indent = GetIndent(indentLevel);

	_ftprintf(pStream, _T("%s\t%s {\n"), indent.data(), ID_CAMERA_SETTINGS);

	// Frame #
	_ftprintf(pStream, _T("%s\t\t%s %d\n"), indent.data(), ID_TIMEVALUE, t);

	if (cs->manualClip) {
		_ftprintf(pStream, _T("%s\t\t%s %s\n"), indent.data(), ID_CAMERA_HITHER, Format(cs->hither));
		_ftprintf(pStream, _T("%s\t\t%s %s\n"), indent.data(), ID_CAMERA_YON, Format(cs->yon));
	}

	_ftprintf(pStream, _T("%s\t\t%s %s\n"), indent.data(), ID_CAMERA_NEAR, Format(cs->nearRange));
	_ftprintf(pStream, _T("%s\t\t%s %s\n"), indent.data(), ID_CAMERA_FAR, Format(cs->farRange));
	_ftprintf(pStream, _T("%s\t\t%s %s\n"), indent.data(), ID_CAMERA_FOV, Format(cs->fov));
	_ftprintf(pStream, _T("%s\t\t%s %s\n"), indent.data(), ID_CAMERA_TDIST, Format(cam->GetTDist(t)));

	_ftprintf(pStream, _T("%s\t}\n"),indent.data());
}


/****************************************************************************

  Helper object output
  
****************************************************************************/

void AsciiExp::ExportHelperObject(INode* node, int indentLevel)
{
	TSTR indent = GetIndent(indentLevel);
	ExportNodeHeader(node, ID_HELPER, indentLevel);

	// We don't really know what kind of helper this is, but by exporting
	// the Classname of the helper object, the importer has a chance to
	// identify it.
	Object* helperObj = node->EvalWorldState(0).obj;
	if (helperObj) {
		TSTR className;
		helperObj->GetClassName(className);
		Interface14 *iface = GetCOREInterface14();
		UINT codepage  = iface-> DefaultTextSaveCodePage(true);
		const char* className_locale = className.ToCP(codepage);
		_ftprintf(pStream, _T("%s\t%s \"%hs\"\n"), indent.data(), ID_HELPER_CLASS, className_locale);
	}

	ExportNodeTM(node, indentLevel);

	if (helperObj) {
		TimeValue	t = GetStaticFrame();
		Matrix3		oTM = node->GetObjectTM(t);
		Box3		bbox;

		helperObj->GetDeformBBox(t, bbox, &oTM);

		_ftprintf(pStream, _T("%s\t%s %s\n"), indent.data(), ID_BOUNDINGBOX_MIN, Format(bbox.pmin));
		_ftprintf(pStream, _T("%s\t%s %s\n"), indent.data(), ID_BOUNDINGBOX_MAX, Format(bbox.pmax));
	}

	if (GetIncludeAnim()) {
		ExportAnimKeys(node, indentLevel);
	}
	
	_ftprintf(pStream, _T("%s}\n"), indent.data());
}


/****************************************************************************

  Node Header
  
****************************************************************************/

// The Node Header consists of node type (geometry, helper, camera etc.)
// node name and parent node
void AsciiExp::ExportNodeHeader(INode* node, TCHAR* type, int indentLevel)
{
	TSTR indent = GetIndent(indentLevel);
	Interface14 *iface = GetCOREInterface14();
	UINT codepage  = iface-> DefaultTextSaveCodePage(true);
	
	// Output node header and object type 
	TSTR typeStr = type;
	const char* type_locale = typeStr.ToCP(codepage);
	_ftprintf(pStream, _T("%s%hs {\n"), indent.data(), type_locale);
	
	// Node name
	TSTR nodeStr = FixupName(node->GetName());
	const char* name_locale = nodeStr.ToCP(codepage);
	_ftprintf(pStream, _T("%s\t%s \"%hs\"\n"), indent.data(), ID_NODE_NAME, name_locale);
	
	//  If the node is linked, export parent node name
	INode* parent = node->GetParentNode();
	if (parent && !parent->IsRootNode()) {
		TSTR parentName = FixupName(parent->GetName());
		const char* parentName_locale = parentName.ToCP(codepage);
		_ftprintf(pStream, _T("%s\t%s \"%hs\"\n"), indent.data(), ID_NODE_PARENT, parentName_locale);
	}
}


/****************************************************************************

  Node Transformation
  
****************************************************************************/

void AsciiExp::ExportNodeTM(INode* node, int indentLevel)
{
	Matrix3 pivot = node->GetNodeTM(GetStaticFrame());
	TSTR indent = GetIndent(indentLevel);
	
	_ftprintf(pStream, _T("%s\t%s {\n"), indent.data(), ID_NODE_TM);
	
	// Node name
	// We export the node name together with the nodeTM, because some objects
	// (like a camera or a spotlight) has an additional node (the target).
	// In that case the nodeTM and the targetTM is exported after eachother
	// and the nodeName is how you can tell them apart.
	Interface14 *iface = GetCOREInterface14();
	UINT codepage  = iface-> DefaultTextSaveCodePage(true);
	TSTR nodeName = FixupName(node->GetName());
	const char* nodeName_locale = nodeName.ToCP(codepage);
	_ftprintf(pStream, _T("%s\t\t%s \"%hs\"\n"), indent.data(), ID_NODE_NAME, nodeName_locale);

	// Export TM inheritance flags
	DWORD iFlags = node->GetTMController()->GetInheritanceFlags();
	_ftprintf(pStream, _T("%s\t\t%s %d %d %d\n"), indent.data(), ID_INHERIT_POS,
		INHERIT_POS_X & iFlags ? 1 : 0,
		INHERIT_POS_Y & iFlags ? 1 : 0,
		INHERIT_POS_Z & iFlags ? 1 : 0);

	_ftprintf(pStream, _T("%s\t\t%s %d %d %d\n"), indent.data(), ID_INHERIT_ROT,
		INHERIT_ROT_X & iFlags ? 1 : 0,
		INHERIT_ROT_Y & iFlags ? 1 : 0,
		INHERIT_ROT_Z & iFlags ? 1 : 0);

	_ftprintf(pStream, _T("%s\t\t%s %d %d %d\n"), indent.data(), ID_INHERIT_SCL,
		INHERIT_SCL_X & iFlags ? 1 : 0,
		INHERIT_SCL_Y & iFlags ? 1 : 0,
		INHERIT_SCL_Z & iFlags ? 1 : 0);

	// Dump the full matrix
	DumpMatrix3(&pivot, indentLevel+2);
	
	_ftprintf(pStream, _T("%s\t}\n"), indent.data());
}

/****************************************************************************

  Animation output
  
****************************************************************************/

// If the object is animated, then we will output the entire mesh definition
// for every specified frame. This can result in, dare I say, rather large files.
//
// Many target systems (including MAX itself!) cannot always read back this
// information. If the objects maintains the same number of verices it can be
// imported as a morph target, but if the number of vertices are animated it
// could not be read back in withou special tricks.
// Since the target system for this exporter is unknown, it is up to the
// user (or developer) to make sure that the data conforms with the target system.

void AsciiExp::ExportAnimMesh(INode* node, int indentLevel)
{
	ObjectState os = node->EvalWorldState(GetStaticFrame());
	if (!os.obj)
		return;
	
	TSTR indent = GetIndent(indentLevel);
	
	// Get animation range
	Interval animRange = ip->GetAnimRange();
	// Get validity of the object
	Interval objRange = os.obj->ObjectValidity(GetStaticFrame());
	
	// If the animation range is not fully included in the validity
	// interval of the object, then we're animated.
	if (!objRange.InInterval(animRange)) {
		
		_ftprintf(pStream, _T("%s\t%s {\n"), indent.data(), ID_MESH_ANIMATION);
		
		TimeValue t = animRange.Start();
		
		while (1) {
			// This may seem strange, but the object in the pipeline
			// might not be valid anymore.
			os = node->EvalWorldState(t);
			objRange = os.obj->ObjectValidity(t);
			t = objRange.Start() < animRange.Start() ? animRange.Start() : objRange.Start();
			
			// Export the mesh definition at this frame
			ExportMesh(node, t, indentLevel+1);
			
			if (objRange.End() >= animRange.End()) {
				break;
			}
			else {
				t = (objRange.End()/GetTicksPerFrame()+GetMeshFrameStep()) * GetTicksPerFrame();
			}
		}
		_ftprintf(pStream, _T("%s\t}\n"), indent.data());
	}
}


/****************************************************************************

  Mesh output
  
****************************************************************************/

void AsciiExp::ExportMesh(INode* node, TimeValue t, int indentLevel)
{
	int i;
	Mtl* nodeMtl = node->GetMtl();
	Matrix3 tm = node->GetObjTMAfterWSM(t);
	BOOL negScale = TMNegParity(tm);
	int vx1, vx2, vx3;
	TSTR indent;
	
	indent = GetIndent(indentLevel+1);
	
	ObjectState os = node->EvalWorldState(t);
	if (!os.obj || os.obj->SuperClassID()!=GEOMOBJECT_CLASS_ID) {
		return; // Safety net. This shouldn't happen.
	}
	
	// Order of the vertices. Get 'em counter clockwise if the objects is
	// negatively scaled.
	if (negScale) {
		vx1 = 2;
		vx2 = 1;
		vx3 = 0;
	}
	else {
		vx1 = 0;
		vx2 = 1;
		vx3 = 2;
	}
	
	BOOL needDel;
	TriObject* tri = GetTriObjectFromNode(node, t, needDel);
	if (!tri) {
		return;
	}
	
	Mesh* mesh = &tri->GetMesh();
	
	mesh->buildNormals();
	
	_ftprintf(pStream, _T("%s%s {\n"),indent.data(),  ID_MESH);
	_ftprintf(pStream, _T("%s\t%s %d\n"),indent.data(), ID_TIMEVALUE, t);
	_ftprintf(pStream, _T("%s\t%s %d\n"),indent.data(), ID_MESH_NUMVERTEX, mesh->getNumVerts());
    _ftprintf(pStream, _T("%s\t%s %d\n"),indent.data(), ID_MESH_NUMFACES, mesh->getNumFaces());
	
	// Export the vertices
	_ftprintf(pStream, _T("%s\t%s {\n"),indent.data(), ID_MESH_VERTEX_LIST);
	for (i=0; i<mesh->getNumVerts(); i++) {
		Point3 v = tm * mesh->verts[i];
		_ftprintf(pStream, _T("%s\t\t%s %4d\t%s\n"),indent.data(), ID_MESH_VERTEX, i, Format(v));
	}
	_ftprintf(pStream, _T("%s\t}\n"),indent.data()); // End vertex list
	
	// To determine visibility of a face, get the vertices in clockwise order.
	// If the objects has a negative scaling, we must compensate for that by
	// taking the vertices counter clockwise
	_ftprintf(pStream, _T("%s\t%s {\n"),indent.data(), ID_MESH_FACE_LIST);
	for (i=0; i<mesh->getNumFaces(); i++) {
		_ftprintf(pStream, _T("%s\t\t%s %4d:    A: %4d B: %4d C: %4d AB: %4d BC: %4d CA: %4d"),
			indent.data(),
			ID_MESH_FACE, i,
			mesh->faces[i].v[vx1],
			mesh->faces[i].v[vx2],
			mesh->faces[i].v[vx3],
			mesh->faces[i].getEdgeVis(vx1) ? 1 : 0,
			mesh->faces[i].getEdgeVis(vx2) ? 1 : 0,
			mesh->faces[i].getEdgeVis(vx3) ? 1 : 0);
		_ftprintf(pStream, _T("\t %s "), ID_MESH_SMOOTHING);
		for (int j=0; j<32; j++) {
			if (mesh->faces[i].smGroup & (1<<j)) {
				if ((j<31) && (mesh->faces[i].smGroup>>(j+1))) {
					_ftprintf(pStream, _T("%d,"),j+1); // Add extra comma
				} else {
					_ftprintf(pStream, _T("%d "),j+1);
				}
			}
		}
		
		// This is the material ID for the face.
		// Note: If you use this you should make sure that the material ID
		// is not larger than the number of sub materials in the material.
		// The standard approach is to use a modulus function to bring down
		// the material ID.
		_ftprintf(pStream, _T("\t%s %d"), ID_MESH_MTLID, mesh->faces[i].getMatID());
		
		_ftprintf(pStream, _T("\n"));
	}
	_ftprintf(pStream, _T("%s\t}\n"), indent.data()); // End face list
	
	// Export face map texcoords if we have them...
	if (GetIncludeTextureCoords() && !CheckForAndExportFaceMap(nodeMtl, mesh, indentLevel+1)) {
		// If not, export standard tverts
		int numTVx = mesh->getNumTVerts();

		_ftprintf(pStream, _T("%s\t%s %d\n"),indent.data(), ID_MESH_NUMTVERTEX, numTVx);

		if (numTVx) {
			_ftprintf(pStream, _T("%s\t%s {\n"),indent.data(), ID_MESH_TVERTLIST);
			for (i=0; i<numTVx; i++) {
				UVVert tv = mesh->tVerts[i];
				_ftprintf(pStream, _T("%s\t\t%s %d\t%s\n"),indent.data(), ID_MESH_TVERT, i, Format(tv));
			}
			_ftprintf(pStream, _T("%s\t}\n"),indent.data());
			
			_ftprintf(pStream, _T("%s\t%s %d\n"),indent.data(), ID_MESH_NUMTVFACES, mesh->getNumFaces());

			_ftprintf(pStream, _T("%s\t%s {\n"),indent.data(), ID_MESH_TFACELIST);
			for (i=0; i<mesh->getNumFaces(); i++) {
				_ftprintf(pStream, _T("%s\t\t%s %d\t%d\t%d\t%d\n"),
					indent.data(),
					ID_MESH_TFACE, i,
					mesh->tvFace[i].t[vx1],
					mesh->tvFace[i].t[vx2],
					mesh->tvFace[i].t[vx3]);
			}
			_ftprintf(pStream, _T("%s\t}\n"),indent.data());
		}

		// CCJ 3/9/99
		// New for R3 - Additional mapping channels
		for (int mp = 2; mp < MAX_MESHMAPS-1; mp++) {
			if (mesh->mapSupport(mp)) {

				_ftprintf(pStream, _T("%s\t%s %d {\n"),indent.data(), ID_MESH_MAPPINGCHANNEL, mp);


				int numTVx = mesh->getNumMapVerts(mp);
				_ftprintf(pStream, _T("%s\t\t%s %d\n"),indent.data(), ID_MESH_NUMTVERTEX, numTVx);

				if (numTVx) {
					_ftprintf(pStream, _T("%s\t\t%s {\n"),indent.data(), ID_MESH_TVERTLIST);
					for (i=0; i<numTVx; i++) {
						UVVert tv = mesh->mapVerts(mp)[i];
						_ftprintf(pStream, _T("%s\t\t\t%s %d\t%s\n"),indent.data(), ID_MESH_TVERT, i, Format(tv));
					}
					_ftprintf(pStream, _T("%s\t\t}\n"),indent.data());
					
					_ftprintf(pStream, _T("%s\t\t%s %d\n"),indent.data(), ID_MESH_NUMTVFACES, mesh->getNumFaces());

					_ftprintf(pStream, _T("%s\t\t%s {\n"),indent.data(), ID_MESH_TFACELIST);
					for (i=0; i<mesh->getNumFaces(); i++) {
						_ftprintf(pStream, _T("%s\t\t\t%s %d\t%d\t%d\t%d\n"),
							indent.data(),
							ID_MESH_TFACE, i,
							mesh->mapFaces(mp)[i].t[vx1],
							mesh->mapFaces(mp)[i].t[vx2],
							mesh->mapFaces(mp)[i].t[vx3]);
					}
					_ftprintf(pStream, _T("%s\t\t}\n"),indent.data());
				}
				_ftprintf(pStream, _T("%s\t}\n"),indent.data());
			}
		}
	}

	// Export color per vertex info
	if (GetIncludeVertexColors()) {
		int numCVx = mesh->numCVerts;

		_ftprintf(pStream, _T("%s\t%s %d\n"),indent.data(), ID_MESH_NUMCVERTEX, numCVx);
		if (numCVx) {
			_ftprintf(pStream, _T("%s\t%s {\n"),indent.data(), ID_MESH_CVERTLIST);
			for (i=0; i<numCVx; i++) {
				Point3 vc = mesh->vertCol[i];
				_ftprintf(pStream, _T("%s\t\t%s %d\t%s\n"),indent.data(), ID_MESH_VERTCOL, i, Format(vc));
			}
			_ftprintf(pStream, _T("%s\t}\n"),indent.data());
			
			_ftprintf(pStream, _T("%s\t%s %d\n"),indent.data(), ID_MESH_NUMCVFACES, mesh->getNumFaces());

			_ftprintf(pStream, _T("%s\t%s {\n"),indent.data(), ID_MESH_CFACELIST);
			for (i=0; i<mesh->getNumFaces(); i++) {
				_ftprintf(pStream, _T("%s\t\t%s %d\t%d\t%d\t%d\n"),
					indent.data(),
					ID_MESH_CFACE, i,
					mesh->vcFace[i].t[vx1],
					mesh->vcFace[i].t[vx2],
					mesh->vcFace[i].t[vx3]);
			}
			_ftprintf(pStream, _T("%s\t}\n"),indent.data());
		}
	}
	
	if (GetIncludeNormals()) {
		// Export mesh (face + vertex) normals
		_ftprintf(pStream, _T("%s\t%s {\n"),indent.data(), ID_MESH_NORMALS);
		
		Point3 fn;  // Face normal
		Point3 vn;  // Vertex normal
		int  vert;
		Face* f;
		
		// Face and vertex normals.
		// In MAX a vertex can have more than one normal (but doesn't always have it).
		// This is depending on the face you are accessing the vertex through.
		// To get all information we need to export all three vertex normals
		// for every face.
		for (i=0; i<mesh->getNumFaces(); i++) {
			f = &mesh->faces[i];
			fn = mesh->getFaceNormal(i);
			_ftprintf(pStream, _T("%s\t\t%s %d\t%s\n"), indent.data(), ID_MESH_FACENORMAL, i, Format(fn));
			
			vert = f->getVert(vx1);
			vn = GetVertexNormal(mesh, i, mesh->getRVertPtr(vert));
			_ftprintf(pStream, _T("%s\t\t\t%s %d\t%s\n"),indent.data(), ID_MESH_VERTEXNORMAL, vert, Format(vn));
			
			vert = f->getVert(vx2);
			vn = GetVertexNormal(mesh, i, mesh->getRVertPtr(vert));
			_ftprintf(pStream, _T("%s\t\t\t%s %d\t%s\n"),indent.data(), ID_MESH_VERTEXNORMAL, vert, Format(vn));
			
			vert = f->getVert(vx3);
			vn = GetVertexNormal(mesh, i, mesh->getRVertPtr(vert));
			_ftprintf(pStream, _T("%s\t\t\t%s %d\t%s\n"),indent.data(), ID_MESH_VERTEXNORMAL, vert, Format(vn));
		}
		
		_ftprintf(pStream, _T("%s\t}\n"),indent.data());
	}
	
	_ftprintf(pStream, _T("%s}\n"),indent.data());
	
	if (needDel) {
		delete tri;
	}
}

Point3 AsciiExp::GetVertexNormal(Mesh* mesh, int faceNo, RVertex* rv)
{
	Face* f = &mesh->faces[faceNo];
	DWORD smGroup = f->smGroup;
	int numNormals = 0;
	Point3 vertexNormal;
	
	// Is normal specified
	// SPCIFIED is not currently used, but may be used in future versions.
	if (rv->rFlags & SPECIFIED_NORMAL) {
		vertexNormal = rv->rn.getNormal();
	}
	// If normal is not specified it's only available if the face belongs
	// to a smoothing group
	else if ((numNormals = rv->rFlags & NORCT_MASK) != 0 && smGroup) {
		// If there is only one vertex is found in the rn member.
		if (numNormals == 1) {
			vertexNormal = rv->rn.getNormal();
		}
		else {
			// If two or more vertices are there you need to step through them
			// and find the vertex with the same smoothing group as the current face.
			// You will find multiple normals in the ern member.
			for (int i = 0; i < numNormals; i++) {
				if (rv->ern[i].getSmGroup() & smGroup) {
					vertexNormal = rv->ern[i].getNormal();
				}
			}
		}
	}
	else {
		// Get the normal from the Face if no smoothing groups are there
		vertexNormal = mesh->getFaceNormal(faceNo);
	}
	
	return vertexNormal;
}

/****************************************************************************

  Inverse Kinematics (IK) Joint information
  
****************************************************************************/

void AsciiExp::ExportIKJoints(INode* node, int indentLevel)
{
	Control* cont;
	TSTR indent = GetIndent(indentLevel);

	if (node->TestAFlag(A_INODE_IK_TERMINATOR)) 
		_ftprintf(pStream, _T("%s\t%s\n"), indent.data(), ID_IKTERMINATOR);

	if(node->TestAFlag(A_INODE_IK_POS_PINNED))
		_ftprintf(pStream, _T("%s\t%s\n"), indent.data(), ID_IKPOS_PINNED);

	if(node->TestAFlag(A_INODE_IK_ROT_PINNED))
		_ftprintf(pStream, _T("%s\t%s\n"), indent.data(), ID_IKROT_PINNED);

	// Position joint
	cont = node->GetTMController()->GetPositionController();
	if (cont) {
		JointParams* joint = (JointParams*)cont->GetProperty(PROPID_JOINTPARAMS);
		if (joint && !joint->IsDefault()) {
			// Has IK Joints!!!
			_ftprintf(pStream, _T("%s\t%s {\n"), indent.data(), ID_IKJOINT);
			DumpJointParams(joint, indentLevel+1);
			_ftprintf(pStream, _T("%s\t}\n"), indent.data());
		}
	}

	// Rotational joint
	cont = node->GetTMController()->GetRotationController();
	if (cont) {
		JointParams* joint = (JointParams*)cont->GetProperty(PROPID_JOINTPARAMS);
		if (joint && !joint->IsDefault()) {
			// Has IK Joints!!!
			_ftprintf(pStream, _T("%s\t%s {\n"), indent.data(), ID_IKJOINT);
			DumpJointParams(joint, indentLevel+1);
			_ftprintf(pStream, _T("%s\t}\n"), indent.data());
		}
	}
}

void AsciiExp::DumpJointParams(JointParams* joint, int indentLevel)
{
	TSTR indent = GetIndent(indentLevel);
	float scale = joint->scale;

	_ftprintf(pStream, _T("%s\t%s %s\n"), indent.data(), ID_IKTYPE,   joint->Type() == JNT_POS ? ID_IKTYPEPOS : ID_IKTYPEROT);
	_ftprintf(pStream, _T("%s\t%s %d\n"), indent.data(), ID_IKDOF,    joint->dofs);

	_ftprintf(pStream, _T("%s\t%s %d\n"), indent.data(), ID_IKXACTIVE,  joint->flags & JNT_XACTIVE  ? 1 : 0);
	_ftprintf(pStream, _T("%s\t%s %d\n"), indent.data(), ID_IKYACTIVE,  joint->flags & JNT_YACTIVE  ? 1 : 0);
	_ftprintf(pStream, _T("%s\t%s %d\n"), indent.data(), ID_IKZACTIVE,  joint->flags & JNT_ZACTIVE  ? 1 : 0);

	_ftprintf(pStream, _T("%s\t%s %d\n"), indent.data(), ID_IKXLIMITED, joint->flags & JNT_XLIMITED ? 1 : 0);
	_ftprintf(pStream, _T("%s\t%s %d\n"), indent.data(), ID_IKYLIMITED, joint->flags & JNT_YLIMITED ? 1 : 0);
	_ftprintf(pStream, _T("%s\t%s %d\n"), indent.data(), ID_IKZLIMITED, joint->flags & JNT_ZLIMITED ? 1 : 0);

	_ftprintf(pStream, _T("%s\t%s %d\n"), indent.data(), ID_IKXEASE,    joint->flags & JNT_XEASE    ? 1 : 0);
	_ftprintf(pStream, _T("%s\t%s %d\n"), indent.data(), ID_IKYEASE,    joint->flags & JNT_YEASE    ? 1 : 0);
	_ftprintf(pStream, _T("%s\t%s %d\n"), indent.data(), ID_IKZEASE,    joint->flags & JNT_ZEASE    ? 1 : 0);

	_ftprintf(pStream, _T("%s\t%s %d\n"), indent.data(), ID_IKLIMITEXACT, joint->flags & JNT_LIMITEXACT ? 1 : 0);

	for (int i=0; i<joint->dofs; i++) {
		_ftprintf(pStream, _T("%s\t%s %d %s %s %s\n"), indent.data(), ID_IKJOINTINFO, i, Format(joint->min[i]), Format(joint->max[i]), Format(joint->damping[i]));
	}

}

/****************************************************************************

  Material and Texture Export
  
****************************************************************************/

void AsciiExp::ExportMaterialList()
{
	if (!GetIncludeMtl()) {
		return;
	}

	_ftprintf(pStream, _T("%s {\n"), ID_MATERIAL_LIST);

	int numMtls = mtlList.Count();
	_ftprintf(pStream, _T("\t%s %d\n"), ID_MATERIAL_COUNT, numMtls);

	for (int i=0; i<numMtls; i++) {
		DumpMaterial(mtlList.GetMtl(i), i, -1, 0);
	}

	_ftprintf(pStream, _T("}\n"));
}

void AsciiExp::ExportMaterial(INode* node, int indentLevel)
{
	Mtl* mtl = node->GetMtl();
	
	TSTR indent = GetIndent(indentLevel);
	
	// If the node does not have a material, export the wireframe color
	if (mtl) {
		int mtlID = mtlList.GetMtlID(mtl);
		if (mtlID >= 0) {
			_ftprintf(pStream, _T("%s\t%s %d\n"), indent.data(), ID_MATERIAL_REF, mtlID);
		}
	}
	else {
		DWORD c = node->GetWireColor();
		_ftprintf(pStream, _T("%s\t%s %s\n"), indent.data(), ID_WIRECOLOR,
			Format(Color(GetRValue(c)/255.0f, GetGValue(c)/255.0f, GetBValue(c)/255.0f)));
	}
}

void AsciiExp::DumpMaterial(Mtl* mtl, int mtlID, int subNo, int indentLevel)
{
	int i;
	TimeValue t = GetStaticFrame();
	
	if (!mtl) return;
	
	TSTR indent = GetIndent(indentLevel+1);
	
	TSTR className;
	mtl->GetClassName(className);
	
	
	if (subNo == -1) {
		// Top level material
		_ftprintf(pStream, _T("%s%s %d {\n"),indent.data(), ID_MATERIAL, mtlID);
	}
	else {
		_ftprintf(pStream, _T("%s%s %d {\n"),indent.data(), ID_SUBMATERIAL, subNo);
	}
	Interface14 *iface = GetCOREInterface14();
	UINT codepage  = iface-> DefaultTextSaveCodePage(true);
	TSTR mtlName = FixupName(mtl->GetName());
	const char* mtlName_locale = mtlName.ToCP(codepage);
	_ftprintf(pStream, _T("%s\t%s \"%hs\"\n"),indent.data(), ID_MATNAME, mtlName_locale);
	TSTR classNameStr = FixupName(className);
	const char* classname_locale = classNameStr.ToCP(codepage);
	_ftprintf(pStream, _T("%s\t%s \"%hs\"\n"),indent.data(), ID_MATCLASS,classname_locale);
	
	// We know the Standard material, so we can get some extra info
	if (mtl->ClassID() == Class_ID(DMTL_CLASS_ID, 0)) {
		StdMat* std = (StdMat*)mtl;

		_ftprintf(pStream, _T("%s\t%s %s\n"),indent.data(), ID_AMBIENT, Format(std->GetAmbient(t)));
		_ftprintf(pStream, _T("%s\t%s %s\n"),indent.data(), ID_DIFFUSE, Format(std->GetDiffuse(t)));
		_ftprintf(pStream, _T("%s\t%s %s\n"),indent.data(), ID_SPECULAR, Format(std->GetSpecular(t)));
		_ftprintf(pStream, _T("%s\t%s %s\n"),indent.data(), ID_SHINE, Format(std->GetShininess(t)));
		_ftprintf(pStream, _T("%s\t%s %s\n"),indent.data(), ID_SHINE_STRENGTH, Format(std->GetShinStr(t)));
		_ftprintf(pStream, _T("%s\t%s %s\n"),indent.data(), ID_TRANSPARENCY, Format(std->GetXParency(t)));
		_ftprintf(pStream, _T("%s\t%s %s\n"),indent.data(), ID_WIRESIZE, Format(std->WireSize(t)));

		_ftprintf(pStream, _T("%s\t%s "), indent.data(), ID_SHADING);
		switch(std->GetShading()) {
		case SHADE_CONST: _ftprintf(pStream, _T("%s\n"), ID_MAT_SHADE_CONST); break;
		case SHADE_PHONG: _ftprintf(pStream, _T("%s\n"), ID_MAT_SHADE_PHONG); break;
		case SHADE_METAL: _ftprintf(pStream, _T("%s\n"), ID_MAT_SHADE_METAL); break;
		case SHADE_BLINN: _ftprintf(pStream, _T("%s\n"), ID_MAT_SHADE_BLINN); break;
		default: _ftprintf(pStream, _T("%s\n"), ID_MAT_SHADE_OTHER); break;
		}
		
		_ftprintf(pStream, _T("%s\t%s %s\n"), indent.data(), ID_XP_FALLOFF, Format(std->GetOpacFalloff(t)));
		_ftprintf(pStream, _T("%s\t%s %s\n"), indent.data(), ID_SELFILLUM, Format(std->GetSelfIllum(t)));
		
		if (std->GetTwoSided()) {
			_ftprintf(pStream, _T("%s\t%s\n"), indent.data(), ID_TWOSIDED);
		}
		
		if (std->GetWire()) {
			_ftprintf(pStream, _T("%s\t%s\n"), indent.data(), ID_WIRE);
		}
		
		if (std->GetWireUnits()) {
			_ftprintf(pStream, _T("%s\t%s\n"), indent.data(), ID_WIREUNITS);
		}
		
		_ftprintf(pStream, _T("%s\t%s %s\n"), indent.data(), ID_FALLOFF, std->GetFalloffOut() ? ID_FALLOFF_OUT : ID_FALLOFF_IN);
		
		if (std->GetFaceMap()) {
			_ftprintf(pStream, _T("%s\t%s\n"), indent.data(), ID_FACEMAP);
		}
		
		if (std->GetSoften()) {
			_ftprintf(pStream, _T("%s\t%s\n"), indent.data(), ID_SOFTEN);
		}
		
		_ftprintf(pStream, _T("%s\t%s "), indent.data(), ID_XP_TYPE);
		switch (std->GetTransparencyType()) {
		case TRANSP_FILTER: _ftprintf(pStream, _T("%s\n"), ID_MAP_XPTYPE_FLT); break;
		case TRANSP_SUBTRACTIVE: _ftprintf(pStream, _T("%s\n"), ID_MAP_XPTYPE_SUB); break;
		case TRANSP_ADDITIVE: _ftprintf(pStream, _T("%s\n"), ID_MAP_XPTYPE_ADD); break;
		default: _ftprintf(pStream, _T("%s\n"), ID_MAP_XPTYPE_OTH); break;
		}
	}
	else {
		// Note about material colors:
		// This is only the color used by the interactive renderer in MAX.
		// To get the color used by the scanline renderer, we need to evaluate
		// the material using the mtl->Shade() method.
		// Since the materials are procedural there is no real "diffuse" color for a MAX material
		// but we can at least take the interactive color.
		
		_ftprintf(pStream, _T("%s\t%s %s\n"),indent.data(), ID_AMBIENT, Format(mtl->GetAmbient()));
		_ftprintf(pStream, _T("%s\t%s %s\n"),indent.data(), ID_DIFFUSE, Format(mtl->GetDiffuse()));
		_ftprintf(pStream, _T("%s\t%s %s\n"),indent.data(), ID_SPECULAR, Format(mtl->GetSpecular()));
		_ftprintf(pStream, _T("%s\t%s %s\n"),indent.data(), ID_SHINE, Format(mtl->GetShininess()));
		_ftprintf(pStream, _T("%s\t%s %s\n"),indent.data(), ID_SHINE_STRENGTH, Format(mtl->GetShinStr()));
		_ftprintf(pStream, _T("%s\t%s %s\n"),indent.data(), ID_TRANSPARENCY, Format(mtl->GetXParency()));
		_ftprintf(pStream, _T("%s\t%s %s\n"),indent.data(), ID_WIRESIZE, Format(mtl->WireSize()));
	}

	for (i=0; i<mtl->NumSubTexmaps(); i++) {
		Texmap* subTex = mtl->GetSubTexmap(i);
		float amt = 1.0f;
		if (subTex) {
			// If it is a standard material we can see if the map is enabled.
			if (mtl->ClassID() == Class_ID(DMTL_CLASS_ID, 0)) {
				if (!((StdMat*)mtl)->MapEnabled(i))
					continue;
				amt = ((StdMat*)mtl)->GetTexmapAmt(i, 0);
				
			}
			DumpTexture(subTex, mtl->ClassID(), i, amt, indentLevel+1);
		}
	}
	
	if (mtl->NumSubMtls() > 0)  {
		_ftprintf(pStream, _T("%s\t%s %d\n"),indent.data(), ID_NUMSUBMTLS, mtl->NumSubMtls());
		
		for (i=0; i<mtl->NumSubMtls(); i++) {
			Mtl* subMtl = mtl->GetSubMtl(i);
			if (subMtl) {
				DumpMaterial(subMtl, 0, i, indentLevel+1);
			}
		}
	}
	_ftprintf(pStream, _T("%s}\n"), indent.data());
}


// For a standard material, this will give us the meaning of a map
// givien its submap id.
TCHAR* AsciiExp::GetMapID(Class_ID cid, int subNo)
{
	static TCHAR buf[50];
	
	if (cid == Class_ID(0,0)) {
		_tcscpy(buf, ID_ENVMAP);
	}
	else if (cid == Class_ID(DMTL_CLASS_ID, 0)) {
		switch (subNo) {
		case ID_AM: _tcscpy(buf, ID_MAP_AMBIENT); break;
		case ID_DI: _tcscpy(buf, ID_MAP_DIFFUSE); break;
		case ID_SP: _tcscpy(buf, ID_MAP_SPECULAR); break;
		case ID_SH: _tcscpy(buf, ID_MAP_SHINE); break;
		case ID_SS: _tcscpy(buf, ID_MAP_SHINESTRENGTH); break;
		case ID_SI: _tcscpy(buf, ID_MAP_SELFILLUM); break;
		case ID_OP: _tcscpy(buf, ID_MAP_OPACITY); break;
		case ID_FI: _tcscpy(buf, ID_MAP_FILTERCOLOR); break;
		case ID_BU: _tcscpy(buf, ID_MAP_BUMP); break;
		case ID_RL: _tcscpy(buf, ID_MAP_REFLECT); break;
		case ID_RR: _tcscpy(buf, ID_MAP_REFRACT); break;
		}
	}
	else {
		_tcscpy(buf, ID_MAP_GENERIC);
	}
	
	return buf;
}

void AsciiExp::DumpTexture(Texmap* tex, Class_ID cid, int subNo, float amt, int indentLevel)
{
	if (!tex) return;
	
	TSTR indent = GetIndent(indentLevel+1);
	
	TSTR className;
	tex->GetClassName(className);
	
	_ftprintf(pStream, _T("%s%s {\n"), indent.data(), GetMapID(cid, subNo));
	
	Interface14 *iface = GetCOREInterface14();
	UINT codepage  = iface-> DefaultTextSaveCodePage(true);
	TSTR texName = FixupName(tex->GetName());
	const char* texName_locale = texName.ToCP(codepage);
	_ftprintf(pStream, _T("%s\t%s \"%hs\"\n"), indent.data(), ID_TEXNAME, texName_locale);
	TSTR classNameStr = FixupName(className);
	const char* className_locale = classNameStr.ToCP(codepage);
	_ftprintf(pStream, _T("%s\t%s \"%hs\"\n"), indent.data(), ID_TEXCLASS, className_locale);
	
	// If we include the subtexture ID, a parser could be smart enough to get
	// the class name of the parent texture/material and make it mean something.
	_ftprintf(pStream, _T("%s\t%s %d\n"), indent.data(), ID_TEXSUBNO, subNo);
	
	_ftprintf(pStream, _T("%s\t%s %s\n"), indent.data(), ID_TEXAMOUNT, Format(amt));
	
	// Is this a bitmap texture?
	// We know some extra bits 'n pieces about the bitmap texture
	if (tex->ClassID() == Class_ID(BMTEX_CLASS_ID, 0x00)) {
		TSTR mapName = ((BitmapTex *)tex)->GetMapName();
		TSTR mapNameStr = FixupName(mapName);
		const char* mapName_locale = mapNameStr.ToCP(codepage);
		_ftprintf(pStream, _T("%s\t%s \"%hs\"\n"), indent.data(), ID_BITMAP, mapName_locale);
		
		StdUVGen* uvGen = ((BitmapTex *)tex)->GetUVGen();
		if (uvGen) {
			DumpUVGen(uvGen, indentLevel+1);
		}
		
		TextureOutput* texout = ((BitmapTex*)tex)->GetTexout();
		if (texout->GetInvert()) {
			_ftprintf(pStream, _T("%s\t%s\n"), indent.data(), ID_TEX_INVERT);
		}
		
		_ftprintf(pStream, _T("%s\t%s "), indent.data(), ID_BMP_FILTER);
		switch(((BitmapTex*)tex)->GetFilterType()) {
		case FILTER_PYR:  _ftprintf(pStream, _T("%s\n"), ID_BMP_FILT_PYR); break;
		case FILTER_SAT: _ftprintf(pStream, _T("%s\n"), ID_BMP_FILT_SAT); break;
		default: _ftprintf(pStream, _T("%s\n"), ID_BMP_FILT_NONE); break;
		}
	}
	
	for (int i=0; i<tex->NumSubTexmaps(); i++) {
		DumpTexture(tex->GetSubTexmap(i), tex->ClassID(), i, 1.0f, indentLevel+1);
	}
	
	_ftprintf(pStream, _T("%s}\n"), indent.data());
}

void AsciiExp::DumpUVGen(StdUVGen* uvGen, int indentLevel)
{
	int mapType = uvGen->GetCoordMapping(0);
	TimeValue t = GetStaticFrame();
	TSTR indent = GetIndent(indentLevel+1);
	
	_ftprintf(pStream, _T("%s%s "), indent.data(), ID_MAPTYPE);
	
	switch (mapType) {
	case UVMAP_EXPLICIT: _ftprintf(pStream, _T("%s\n"), ID_MAPTYPE_EXP); break;
	case UVMAP_SPHERE_ENV: _ftprintf(pStream, _T("%s\n"), ID_MAPTYPE_SPH); break;
	case UVMAP_CYL_ENV:  _ftprintf(pStream, _T("%s\n"), ID_MAPTYPE_CYL); break;
	case UVMAP_SHRINK_ENV: _ftprintf(pStream, _T("%s\n"), ID_MAPTYPE_SHR); break;
	case UVMAP_SCREEN_ENV: _ftprintf(pStream, _T("%s\n"), ID_MAPTYPE_SCR); break;
	}
	
	_ftprintf(pStream, _T("%s%s %s\n"), indent.data(), ID_U_OFFSET, Format(uvGen->GetUOffs(t)));
	_ftprintf(pStream, _T("%s%s %s\n"), indent.data(), ID_V_OFFSET, Format(uvGen->GetVOffs(t)));
	_ftprintf(pStream, _T("%s%s %s\n"), indent.data(), ID_U_TILING, Format(uvGen->GetUScl(t)));
	_ftprintf(pStream, _T("%s%s %s\n"), indent.data(), ID_V_TILING, Format(uvGen->GetVScl(t)));
	_ftprintf(pStream, _T("%s%s %s\n"), indent.data(), ID_ANGLE, Format(uvGen->GetAng(t)));
	_ftprintf(pStream, _T("%s%s %s\n"), indent.data(), ID_BLUR, Format(uvGen->GetBlur(t)));
	_ftprintf(pStream, _T("%s%s %s\n"), indent.data(), ID_BLUR_OFFSET, Format(uvGen->GetBlurOffs(t)));
	_ftprintf(pStream, _T("%s%s %s\n"), indent.data(), ID_NOISE_AMT, Format(uvGen->GetNoiseAmt(t)));
	_ftprintf(pStream, _T("%s%s %s\n"), indent.data(), ID_NOISE_SIZE, Format(uvGen->GetNoiseSize(t)));
	_ftprintf(pStream, _T("%s%s %d\n"), indent.data(), ID_NOISE_LEVEL, uvGen->GetNoiseLev(t));
	_ftprintf(pStream, _T("%s%s %s\n"), indent.data(), ID_NOISE_PHASE, Format(uvGen->GetNoisePhs(t)));
}

/****************************************************************************

  Face Mapped Material functions
  
****************************************************************************/

BOOL AsciiExp::CheckForAndExportFaceMap(Mtl* mtl, Mesh* mesh, int indentLevel)
{
	if (!mtl || !mesh) {
		return FALSE;
	}
	
	ULONG matreq = mtl->Requirements(-1);
	
	// Are we using face mapping?
	if (!(matreq & MTLREQ_FACEMAP)) {
		return FALSE;
	}
	
	TSTR indent = GetIndent(indentLevel+1);
	
	// OK, we have a FaceMap situation here...
	
	_ftprintf(pStream, _T("%s%s {\n"), indent.data(), ID_MESH_FACEMAPLIST);
	for (int i=0; i<mesh->getNumFaces(); i++) {
		Point3 tv[3];
		Face* f = &mesh->faces[i];
		make_face_uv(f, tv);
		_ftprintf(pStream, _T("%s\t%s %d {\n"), indent.data(), ID_MESH_FACEMAP, i);
		_ftprintf(pStream, _T("%s\t\t%s\t%d\t%d\t%d\n"), indent.data(), ID_MESH_FACEVERT, (int)tv[0].x, (int)tv[0].y, (int)tv[0].z);
		_ftprintf(pStream, _T("%s\t\t%s\t%d\t%d\t%d\n"), indent.data(), ID_MESH_FACEVERT, (int)tv[1].x, (int)tv[1].y, (int)tv[1].z);
		_ftprintf(pStream, _T("%s\t\t%s\t%d\t%d\t%d\n"), indent.data(), ID_MESH_FACEVERT, (int)tv[2].x, (int)tv[2].y, (int)tv[2].z);
		_ftprintf(pStream, _T("%s\t}\n"), indent.data());
	}
	_ftprintf(pStream, _T("%s}\n"), indent.data());
	
	return TRUE;
}


/****************************************************************************

  Misc Utility functions
  
****************************************************************************/

// Return an indentation string
TSTR AsciiExp::GetIndent(int indentLevel)
{
	TSTR indentString = _T("");
	for (int i=0; i<indentLevel; i++) {
		indentString += _T("\t");
	}
	
	return indentString;
}

// Determine is the node has negative scaling.
// This is used for mirrored objects for example. They have a negative scale factor
// so when calculating the normal we should take the vertices counter clockwise.
// If we don't compensate for this the objects will be 'inverted'.
BOOL AsciiExp::TMNegParity(Matrix3 &m)
{
	return (DotProd(CrossProd(m.GetRow(0),m.GetRow(1)),m.GetRow(2))<0.0)?1:0;
}

// Return a pointer to a TriObject given an INode or return NULL
// if the node cannot be converted to a TriObject
TriObject* AsciiExp::GetTriObjectFromNode(INode *node, TimeValue t, int &deleteIt)
{
	deleteIt = FALSE;
	Object *obj = node->EvalWorldState(t).obj;
	if (obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0))) { 
		TriObject *tri = (TriObject *) obj->ConvertToType(t, 
			Class_ID(TRIOBJ_CLASS_ID, 0));
		// Note that the TriObject should only be deleted
		// if the pointer to it is not equal to the object
		// pointer that called ConvertToType()
		if (obj != tri) deleteIt = TRUE;
		return tri;
	}
	else {
		return NULL;
	}
}

// Print out a transformation matrix in different ways.
// Apart from exporting the full matrix we also decompose
// the matrix and export the components.
void AsciiExp::DumpMatrix3(Matrix3* m, int indentLevel)
{
	Point3 row;
	TSTR indent = GetIndent(indentLevel);
	
	// Dump the whole Matrix
	row = m->GetRow(0);
	_ftprintf(pStream, _T("%s%s %s\n"), indent.data(), ID_TM_ROW0, Format(row));
	row = m->GetRow(1);
	_ftprintf(pStream, _T("%s%s %s\n"), indent.data(), ID_TM_ROW1, Format(row));
	row = m->GetRow(2);
	_ftprintf(pStream, _T("%s%s %s\n"), indent.data(), ID_TM_ROW2, Format(row));
	row = m->GetRow(3);
	_ftprintf(pStream, _T("%s%s %s\n"), indent.data(), ID_TM_ROW3, Format(row));
	
	// Decompose the matrix and dump the contents
	AffineParts ap;
	float rotAngle;
	Point3 rotAxis;
	float scaleAxAngle;
	Point3 scaleAxis;
	
	decomp_affine(*m, &ap);

	// Quaternions are dumped as angle axis.
	AngAxisFromQ(ap.q, &rotAngle, rotAxis);
	AngAxisFromQ(ap.u, &scaleAxAngle, scaleAxis);

	_ftprintf(pStream, _T("%s%s %s\n"), indent.data(), ID_TM_POS, Format(ap.t));
	_ftprintf(pStream, _T("%s%s %s\n"), indent.data(), ID_TM_ROTAXIS, Format(rotAxis));
	_ftprintf(pStream, _T("%s%s %s\n"), indent.data(), ID_TM_ROTANGLE, Format(rotAngle));
	_ftprintf(pStream, _T("%s%s %s\n"), indent.data(), ID_TM_SCALE, Format(ap.k));
	_ftprintf(pStream, _T("%s%s %s\n"), indent.data(), ID_TM_SCALEAXIS, Format(scaleAxis));
	_ftprintf(pStream, _T("%s%s %s\n"), indent.data(), ID_TM_SCALEAXISANG, Format(scaleAxAngle));
}

// From the SDK
// How to calculate UV's for face mapped materials.
static Point3 basic_tva[3] = { 
	Point3(0.0,0.0,0.0),Point3(1.0,0.0,0.0),Point3(1.0,1.0,0.0)
};
static Point3 basic_tvb[3] = { 
	Point3(1.0,1.0,0.0),Point3(0.0,1.0,0.0),Point3(0.0,0.0,0.0)
};
static int nextpt[3] = {1,2,0};
static int prevpt[3] = {2,0,1};

void AsciiExp::make_face_uv(Face *f, Point3 *tv)
{
	int na,nhid,i;
	Point3 *basetv;
	/* make the invisible edge be 2->0 */
	nhid = 2;
	if (!(f->flags&EDGE_A))  nhid=0;
	else if (!(f->flags&EDGE_B)) nhid = 1;
	else if (!(f->flags&EDGE_C)) nhid = 2;
	na = 2-nhid;
	basetv = (f->v[prevpt[nhid]]<f->v[nhid]) ? basic_tva : basic_tvb; 
	for (i=0; i<3; i++) {  
		tv[i] = basetv[na];
		na = nextpt[na];
	}
}


/****************************************************************************

  String manipulation functions
  
****************************************************************************/

#define CTL_CHARS  31
#define SINGLE_QUOTE 39

// Replace some characters we don't care for.
TCHAR* AsciiExp::FixupName(const TCHAR* name)
{
	static TCHAR buffer[256];
	TCHAR* cPtr;
	
    _tcscpy(buffer, name);
    cPtr = buffer;
	
    while(*cPtr) {
		if (*cPtr == _T('"'))
			*cPtr = SINGLE_QUOTE;
        else if (*cPtr <= CTL_CHARS)
			*cPtr = _T('_');
        cPtr++;
    }
	
	return buffer;
}

// International settings in Windows could cause a number to be written
// with a "," instead of a ".".
// To compensate for this we need to convert all , to . in order to make the
// format consistent.
void AsciiExp::CommaScan(TCHAR* buf)
{
    for(; *buf; buf++) if (*buf == _T(',')) *buf = _T('.');
}

TSTR AsciiExp::Format(int value)
{
	TCHAR buf[50];
	
	_stprintf(buf, _T("%d"), value);
	return buf;
}


TSTR AsciiExp::Format(float value)
{
	TCHAR buf[40];
	
	_stprintf(buf, szFmtStr, value);
	CommaScan(buf);
	return TSTR(buf);
}

TSTR AsciiExp::Format(Point3 value)
{
	TCHAR buf[120];
	TCHAR fmt[120];
	
	_stprintf(fmt, _T("%s\t%s\t%s"), szFmtStr, szFmtStr, szFmtStr);
	_stprintf(buf, fmt, value.x, value.y, value.z);

	CommaScan(buf);
	return buf;
}

TSTR AsciiExp::Format(Color value)
{
	TCHAR buf[120];
	TCHAR fmt[120];
	
	_stprintf(fmt, _T("%s\t%s\t%s"), szFmtStr, szFmtStr, szFmtStr);
	_stprintf(buf, fmt, value.r, value.g, value.b);

	CommaScan(buf);
	return buf;
}

TSTR AsciiExp::Format(AngAxis value)
{
	TCHAR buf[160];
	TCHAR fmt[160];
	
	_stprintf(fmt, _T("%s\t%s\t%s\t%s"), szFmtStr, szFmtStr, szFmtStr, szFmtStr);
	_stprintf(buf, fmt, value.axis.x, value.axis.y, value.axis.z, value.angle);

	CommaScan(buf);
	return buf;
}


TSTR AsciiExp::Format(Quat value)
{
	// A Quat is converted to an AngAxis before output.
	
	Point3 axis;
	float angle;
	AngAxisFromQ(value, &angle, axis);
	
	return Format(AngAxis(axis, angle));
}

TSTR AsciiExp::Format(ScaleValue value)
{
	TCHAR buf[280];
	
	_stprintf(buf, _T("%s %s"), Format(value.s), Format(value.q));
	CommaScan(buf);
	return buf;
}
