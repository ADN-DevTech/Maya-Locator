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
class squareLocator : public locatorLibBase {
public:
	static MTypeId id ;
	static MString drawDbClassification ;
	static MString drawRegistrantId ;
	static MObject size ; // The size of the square

public:
	squareLocator () : locatorLibBase () {}
	virtual ~squareLocator () {}

	//virtual void draw (M3dView &view, const MDagPath &path, M3dView::DisplayStyle style, M3dView::DisplayStatus status) ;
	virtual void myWireFrameDraw () ;
	virtual void myShadedDraw () ;
	virtual MBoundingBox boundingBox () const ;

	static void *creator () { return (new squareLocator ()) ; }
	static MStatus initialize () ;
	static MStatus registerMe (MFnPlugin &plugin) ;
	static MStatus unregisterMe (MFnPlugin &plugin) ;

public:
	static MPointArray &vertices () ;
	static MBoundingBox boundingbox (float multiplier =1.0f) ;

} ;

//---------------------------------------------------------------------------
// Viewport 2.0 override implementation
class squareLocatorData : public locatorLibData {
public:
	squareLocatorData () : locatorLibData () {} // don't delete after draw
	virtual ~squareLocatorData () {}

} ;

//---------------------------------------------------------------------------
class squareLocatorDrawOverride : public locatorLibDrawOverrideBase {
private:
	squareLocatorDrawOverride (const MObject &obj) : locatorLibDrawOverrideBase (obj, squareLocatorDrawOverride::draw) {}

public:
	virtual ~squareLocatorDrawOverride () {}

	virtual MBoundingBox boundingBox (const MDagPath &objPath,const MDagPath &cameraPath) const ;
#ifdef preMaya2014
	virtual MUserData *prepareForDraw (const MDagPath &objPath, const MDagPath &cameraPath, MUserData *oldData) ;
#else
	virtual MUserData *prepareForDraw (const MDagPath &objPath, const MDagPath &cameraPath, const MHWRender::MFrameContext &frameContext, MUserData *oldData) ;
#endif

	static void draw (const MHWRender::MDrawContext &context, const MUserData *data) ;
	static MHWRender::MPxDrawOverride *Creator (const MObject &obj) { return (new squareLocatorDrawOverride (obj)) ; }

private:
	virtual float getMultiplier (const MDagPath &objPath) const ;

} ;
