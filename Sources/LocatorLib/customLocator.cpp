//
//  Copyright 2012 Autodesk, Inc.  All rights reserved.
//
//  Use of this software is subject to the terms of the Autodesk license 
//  agreement provided at the time of installation or download, or which 
//  otherwise accompanies this software in either electronic or hard copy form.   
//

// Release History
// Written by Cyrille Fauvel
// 1.0    Original release                     (October 1, 2012)

#include "StdAfx.h"
#define MNoVersionString
#define MNoPluginEntry
#include <maya/MFnPlugin.h>
#include "customLocator.h"

//-----------------------------------------------------------------------------
MTypeId customLocator::id (0x00118385) ;
MString	customLocator::drawDbClassification ("drawdb/geometry/customLocator") ;
MString	customLocator::drawRegistrantId ("customLocatorNode") ;
MObject customLocator::size ;
MObject customLocator::shapeFilename ;

//-----------------------------------------------------------------------------
MPointArray customLocator::vertices (MString &shapeName) {
	Shape &shape =shapeMgr.shapeData (shapeName.asChar ()) ;
	MPointArray vertices (shape.vertices) ;
	return (vertices) ;
}

void customLocator::myWireFrameDraw () {
	MPlug plug (thisMObject (), customLocator::size) ;
	float multiplier =plug.asFloat () ;
	MPlug plugShape (thisMObject (), customLocator::shapeFilename) ;
	MString shapeName =plugShape.asString () ;
	//MPointArray vertices =customLocator::vertices (shapeName.asChar ()) ;

	Shape &shape =shapeMgr.shapeData (shapeName.asChar ()) ;
	shape.renderWireFrame (multiplier) ;
}

void customLocator::myShadedDraw () {
	MPlug plug (thisMObject (), customLocator::size) ;
	float multiplier =plug.asFloat () ;
	MPlug plugShape (thisMObject (), customLocator::shapeFilename) ;
	MString shapeName =plugShape.asString () ;
	//MPointArray vertices =customLocator::vertices (shapeName.asChar ()) ;

	Shape &shape =shapeMgr.shapeData (shapeName.asChar ()) ;
	shape.renderShaded (multiplier) ;
}

MBoundingBox customLocator::boundingBox () const {
	MPlug plug (thisMObject (), customLocator::size) ;
	float multiplier =plug.asFloat () ;
	MPlug plugShape (thisMObject (), customLocator::shapeFilename) ;
	MString shapeName =plugShape.asString () ;

	Shape &shape =shapeMgr.shapeData (shapeName.asChar ()) ;
	return (shape.boundingbox (multiplier)) ;
}

MStatus customLocator::initialize () {
	locatorLibBase::initializeSize (customLocator::size) ;

	MStatus stat ;
	MFnTypedAttribute typedAttr ;
	MFnStringData exprSt ;
	customLocator::shapeFilename =typedAttr.create ("shapeName", "sn", MFnData::kString, exprSt.create (), &stat) ;
	typedAttr.setStorable (true) ;
	typedAttr.setKeyable (false) ;
	addAttribute (customLocator::shapeFilename) ;

	return (MS::kSuccess) ;
}

MStatus customLocator::registerMe (MFnPlugin &plugin) {
	plugin.registerNode (
		"customLocator",
		customLocator::id,
		customLocator::creator,
		customLocator::initialize,
		MPxNode::kLocatorNode,
		&customLocator::drawDbClassification
	) ;
	MHWRender::MDrawRegistry::registerDrawOverrideCreator (
		customLocator::drawDbClassification,
		customLocator::drawRegistrantId,
		customLocatorDrawOverride::Creator
	) ;
	return (MS::kSuccess) ;
}

MStatus customLocator::unregisterMe (MFnPlugin &plugin) {
	MHWRender::MDrawRegistry::deregisterDrawOverrideCreator (
		customLocator::drawDbClassification,
		customLocator::drawRegistrantId
	) ;
	return (plugin.deregisterNode (customLocator::id)) ;
}

//---------------------------------------------------------------------------
// Viewport 2.0 override implementation
float customLocatorDrawOverride::getMultiplier (const MDagPath &objPath) const {
	// Retrieve value of the size attribute from the node
	return (locatorLibDrawOverrideBase::getMultiplier (objPath, customLocator::size)) ;
}

MString customLocatorDrawOverride::getShapeName (const MDagPath &objPath) const {
	// Retrieve value of the shapeFilename attribute from the node
	MStatus status ;
	MObject locatorNode =objPath.node (&status) ;
	if ( status ) {
		MPlug plug (locatorNode, customLocator::shapeFilename) ;
		if ( !plug.isNull () ) {
			MString nameVal ;
			if ( plug.getValue (nameVal) )
				return (nameVal) ;
		}
	}
	return ("") ;
}

MBoundingBox customLocatorDrawOverride::boundingBox (const MDagPath &objPath, const MDagPath &cameraPath) const {
	float multiplier =getMultiplier (objPath) ;
	MString shapeName =getShapeName (objPath) ;
	Shape &shape =shapeMgr.shapeData (shapeName.asChar ()) ;
	return (shape.boundingbox (multiplier)) ;
}

MUserData *customLocatorDrawOverride::prepareForDraw (const MDagPath &objPath, const MDagPath &cameraPath, MUserData *oldData) {
	// Retrieve data cache (create if does not exist)
	customLocatorData *data =dynamic_cast<customLocatorData *>(oldData) ;
	if ( !data )
		data =new customLocatorData () ;
	// compute data and cache it
	data->multiplier =getMultiplier (objPath) ;
	data->shapeName =getShapeName (objPath) ;
	return (data) ;
}

void customLocatorDrawOverride::draw (const MHWRender::MDrawContext &context, const MUserData *data) {
	// get cached data
	float color [3] ={ 0.0f, 1.0f, 0.0f } ;
	float multiplier =1.0f ;
	MString shapeName ;
	const customLocatorData *customData =dynamic_cast<const customLocatorData *>(data) ;
	if ( customData ) {
		multiplier =customData->multiplier ;
		shapeName =customData->shapeName ;
	}
	Shape &shape =shapeMgr.shapeData (shapeName.asChar ()) ;
	// get state data
	MStatus status ;
	const MMatrix transform =context.getMatrix (MHWRender::MDrawContext::kWorldViewMtx, &status) ;
	if ( status != MStatus::kSuccess )
		return ;
	const MMatrix projection =context.getMatrix (MHWRender::MDrawContext::kProjectionMtx, &status) ;
	if ( status != MStatus::kSuccess )
		return ;
	const int displayStyle =context.getDisplayStyle () ;
	// get renderer
	MHWRender::MRenderer *theRenderer =MHWRender::MRenderer::theRenderer () ;
	if ( !theRenderer )
		return ;

	// GL Draw
	if ( theRenderer->drawAPIIsOpenGL () ) {
		// set colour
		glColor3fv (color) ;
		// set world matrix
		glMatrixMode (GL_MODELVIEW) ;
		glPushMatrix () ;
		glLoadMatrixd (transform.matrix [0]) ;
		// set projection matrix
		glMatrixMode (GL_PROJECTION) ;
		glPushMatrix () ;
		glLoadMatrixd (projection.matrix [0]) ;
		if ( displayStyle & MHWRender::MDrawContext::kGouraudShaded ) {
			// See myShadedDraw
			glPushAttrib (GL_CURRENT_BIT) ;
			shape.renderShaded (multiplier) ;
			glPopAttrib () ;
		}
		if ( displayStyle & MHWRender::MDrawContext::kWireFrame ) {
			// See myWireFrameDraw
			shape.renderWireFrame (multiplier) ;
		}
		glPopMatrix () ;
		glMatrixMode (GL_MODELVIEW) ;
		glPopMatrix () ;
	}
}
