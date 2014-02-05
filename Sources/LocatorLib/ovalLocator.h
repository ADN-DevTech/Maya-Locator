//
// Copyright 2012 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license 
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form. 
//

// Release History
// Written by Naiqi Weng
// 1.0    Original release                     (September 1, 2012)

#pragma once

#include "LocatorLibBase.h"

//-----------------------------------------------------------------------------
class ovalLocator : public locatorLibBase {
public:
	static MTypeId id ;
	static MString drawDbClassification ;
	static MString drawRegistrantId ;
	static MObject size ; // The size of the oval

public:
	ovalLocator () : locatorLibBase () {}
	virtual ~ovalLocator () {}

	//virtual void draw (M3dView &view, const MDagPath &path, M3dView::DisplayStyle style, M3dView::DisplayStatus status) ;
	virtual void myWireFrameDraw () ;
	virtual void myShadedDraw () ;
	virtual MBoundingBox boundingBox () const ;

	static void *creator () { return (new ovalLocator ()) ; }
	static MStatus initialize () ;
	static MStatus registerMe (MFnPlugin &plugin) ;
	static MStatus unregisterMe (MFnPlugin &plugin) ;

public:
	static MPointArray &vertices () ;
	static MBoundingBox boundingbox (float multiplier =1.0f) ;

} ;

//---------------------------------------------------------------------------
// Viewport 2.0 override implementation
class ovalLocatorData : public locatorLibData {
public:
	ovalLocatorData () : locatorLibData () {} 
	virtual ~ovalLocatorData () {}

} ;

//---------------------------------------------------------------------------
class ovalLocatorDrawOverride : public locatorLibDrawOverrideBase {

private:
	ovalLocatorDrawOverride (const MObject &obj) : locatorLibDrawOverrideBase (obj, ovalLocatorDrawOverride::draw) {}

public:
	virtual ~ovalLocatorDrawOverride () {}

	virtual MBoundingBox boundingBox (const MDagPath &objPath,const MDagPath &cameraPath) const ;
	virtual MUserData *prepareForDraw (const MDagPath &objPath, const MDagPath &cameraPath, MUserData* oldData) ;

	static void draw (const MHWRender::MDrawContext &context, const MUserData* data) ;
	static MHWRender::MPxDrawOverride *Creator (const MObject &obj) { return (new ovalLocatorDrawOverride (obj)) ; }

private:
	virtual float getMultiplier (const MDagPath &objPath) const ;

} ;
