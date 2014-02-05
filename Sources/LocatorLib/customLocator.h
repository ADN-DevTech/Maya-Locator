//
//  Copyright 2012 Autodesk, Inc.  All rights reserved.
//
//  Use of this software is subject to the terms of the Autodesk license 
//  agreement provided at the time of installation or download, or which 
//  otherwise accompanies this software in either electronic or hard copy form.   
//

// Release History
// Written by Cyrille Fauvel
// 1.0    Original release                     (September 1, 2012)

#pragma once
#include "Shape.h"
#include "LocatorLibBase.h"

//-----------------------------------------------------------------------------
class customLocator : public locatorLibBase {
public:
	static MTypeId id ;
	static MString drawDbClassification ;
	static MString drawRegistrantId ;
	static MObject size ; // The size of the locator
	static MObject shapeFilename ; // The shape of the locator

public:
	customLocator () : locatorLibBase () {}
	virtual ~customLocator () {}

	//virtual void draw (M3dView &view, const MDagPath &path, M3dView::DisplayStyle style, M3dView::DisplayStatus status) ;
	virtual void myWireFrameDraw () ;
	virtual void myShadedDraw () ;
	virtual MBoundingBox boundingBox () const ;

	static void *creator () { return (new customLocator ()) ; }
	static MStatus initialize () ;
	static MStatus registerMe (MFnPlugin &plugin) ;
	static MStatus unregisterMe (MFnPlugin &plugin) ;

public:
	static MPointArray vertices (MString &name) ;

} ;

//-----------------------------------------------------------------------------
// Viewport 2.0 override implementation
class customLocatorData : public locatorLibData {
public:
	MString shapeName ;

public:
	customLocatorData () : locatorLibData () {} // don't delete after draw
	virtual ~customLocatorData () {}

} ;

//-----------------------------------------------------------------------------
class customLocatorDrawOverride : public locatorLibDrawOverrideBase {

private:
	customLocatorDrawOverride (const MObject &obj) : locatorLibDrawOverrideBase (obj, customLocatorDrawOverride::draw) {}

public:
	virtual ~customLocatorDrawOverride () {}

	virtual MBoundingBox boundingBox (const MDagPath &objPath, const MDagPath &cameraPath) const ;
#ifdef preMaya2014
	virtual MUserData *prepareForDraw (const MDagPath &objPath, const MDagPath &cameraPath, MUserData *oldData) ;
#else
	virtual MUserData *prepareForDraw (const MDagPath &objPath, const MDagPath &cameraPath, const MHWRender::MFrameContext &frameContext, MUserData *oldData) ;
#endif

	static void draw (const MHWRender::MDrawContext &context, const MUserData *data) ;
	static MHWRender::MPxDrawOverride *Creator (const MObject &obj) { return (new customLocatorDrawOverride (obj)) ; }

private:
	virtual float getMultiplier (const MDagPath &objPath) const ;
	virtual MString getShapeName (const MDagPath &objPath) const ;
} ;
