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
class sphereLocator : public locatorLibBase {

public:
	static MTypeId id ;
	static MString drawDbClassification ;
	static MString drawRegistrantId ;
	static MObject size ; // The size of the sphere

public:
	sphereLocator () : locatorLibBase () {}
	virtual ~sphereLocator () {}

	//virtual void draw (M3dView &view, const MDagPath &path, M3dView::DisplayStyle style, M3dView::DisplayStatus status) ;
	virtual void myWireFrameDraw () ;
	virtual void myShadedDraw () ;
	virtual MBoundingBox boundingBox () const ;

	static void *creator () { return (new sphereLocator ()) ; }
	static MStatus initialize () ;
	static MStatus registerMe (MFnPlugin &plugin) ;
	static MStatus unregisterMe (MFnPlugin &plugin) ;

public:
	static MPointArray &vertices () ;
	static MBoundingBox boundingbox (float multiplier =1.0f) ;

} ;

//-----------------------------------------------------------------------------
// Viewport 2.0 override implementation
class sphereLocatorData : public locatorLibData {
public:
	sphereLocatorData () : locatorLibData () {} // don't delete after draw
	virtual ~sphereLocatorData () {}

} ;

//-----------------------------------------------------------------------------
class sphereLocatorDrawOverride : public locatorLibDrawOverrideBase {
private:
	sphereLocatorDrawOverride (const MObject &obj) : locatorLibDrawOverrideBase (obj, sphereLocatorDrawOverride::draw) {}

public:
	virtual ~sphereLocatorDrawOverride () {}

	virtual MBoundingBox boundingBox (const MDagPath &objPath, const MDagPath &cameraPath) const ;
#ifdef preMaya2014
	virtual MUserData *prepareForDraw (const MDagPath &objPath, const MDagPath &cameraPath, MUserData *oldData) ;
#else
	virtual MUserData *prepareForDraw (const MDagPath &objPath, const MDagPath &cameraPath, const MHWRender::MFrameContext &frameContext, MUserData *oldData) ;
#endif

	static void draw (const MHWRender::MDrawContext &context, const MUserData *data) ;
	static MHWRender::MPxDrawOverride *Creator (const MObject &obj) { return (new sphereLocatorDrawOverride (obj)) ; }

private:
	virtual float getMultiplier (const MDagPath &objPath) const ;

} ;
