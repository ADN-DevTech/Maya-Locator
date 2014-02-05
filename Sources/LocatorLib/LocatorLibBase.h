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

//-----------------------------------------------------------------------------
class locatorLibBase : public MPxLocatorNode {

protected:
	locatorLibBase () : MPxLocatorNode () {}
public:
	virtual ~locatorLibBase () {}

	virtual MStatus compute (const MPlug &plug, MDataBlock &data) { return (MS::kUnknownParameter) ; }
	virtual void draw (M3dView &view, const MDagPath &path, M3dView::DisplayStyle style, M3dView::DisplayStatus status) ;
	virtual void myWireFrameDraw () =0 ;
	virtual void myShadedDraw () {}
	virtual bool isBounded () const { return (true) ; }
	//virtual MBoundingBox boundingBox () const ;

	//static void *creator () ;
	//static MStatus initialize () ;
	static MStatus initializeSize (MObject &attr) ;
	static MStatus initializeColor (MObject &attr) ;

} ;

//-----------------------------------------------------------------------------
// Viewport 2.0 override implementation
class locatorLibData : public MUserData {
public:
	float multiplier ;

public:
	locatorLibData () : MUserData (false) {} // don't delete after draw
	virtual ~locatorLibData () {}

} ;

//-----------------------------------------------------------------------------
class locatorLibDrawOverrideBase : public MHWRender::MPxDrawOverride {

protected:
	locatorLibDrawOverrideBase (const MObject &obj, GeometryDrawOverrideCb callback)
		: MHWRender::MPxDrawOverride (obj, callback) {}

public:
	virtual ~locatorLibDrawOverrideBase () {}

	virtual MHWRender::DrawAPI supportedDrawAPIs () const { return (MHWRender::kOpenGL) ; }
	//virtual MBoundingBox boundingBox (const MDagPath &objPath, const MDagPath &cameraPath) const ;
	//virtual MUserData* prepareForDraw (const MDagPath &objPath, const MDagPath &cameraPath, MUserData *oldData) ;

	//static void draw (const MHWRender::MDrawContext &context, const MUserData *data) ;
	//static MHWRender::MPxDrawOverride *Creator (const MObject &obj) ;

protected:
	virtual float getMultiplier (const MDagPath &objPath, MObject &attr) const ;

} ;
