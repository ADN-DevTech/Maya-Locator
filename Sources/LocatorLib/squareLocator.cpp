//
//  Copyright 2012 Autodesk, Inc.  All rights reserved.
//
//  Use of this software is subject to the terms of the Autodesk license 
//  agreement provided at the time of installation or download, or which 
//  otherwise accompanies this software in either electronic or hard copy form.   
//

// Release History
// Written by Naiqi Weng
// 1.0    Original release                     (October 1, 2012)

#include "StdAfx.h"
#define MNoVersionString
#define MNoPluginEntry
#include <maya/MFnPlugin.h>
#include "squareLocator.h"

//-----------------------------------------------------------------------------
MTypeId squareLocator::id (0x00118381) ;
MString	squareLocator::drawDbClassification ("drawdb/geometry/squareLocator") ;
MString	squareLocator::drawRegistrantId ("squareLocatorNode") ;
MObject squareLocator::size ;

//-----------------------------------------------------------------------------
MPointArray &squareLocator::vertices () {
	static MPointArray vertices ;
	if ( vertices.length () != 0 )
		return (vertices) ;
	// Square data
	static float square [] [3] ={
		{ -0.5f, 0.0f, 0.5f }, { 0.5f, 0.0f, 0.5f }, { 0.5f, 0.0f, -0.5f },
		{ -0.5f, 0.0f, -0.5f }, { -0.5f, 0.0f, 0.5f }
	} ;
	int len =sizeof (square) / sizeof (square [0]) ;
	for ( int i =0 ; i < len ; i++ )
		vertices.append (square [i] [0], square [i] [1], square [i] [2]) ;
	return (vertices) ;
}

void squareLocator::myWireFrameDraw () {
	MPlug plug (thisMObject (), squareLocator::size) ;
	float multiplier =plug.asFloat () ;
	MPointArray vertices =squareLocator::vertices () ;

	glBegin (GL_LINES) ;
	for ( int i =0 ; i < vertices.length () - 1 ; i++ ) {
		glVertex3f (vertices [i].x * multiplier, vertices [i].y * multiplier, vertices [i].z * multiplier) ;
		glVertex3f (vertices [i + 1].x * multiplier, vertices [i + 1].y * multiplier, vertices [i + 1].z * multiplier) ;
	}
	glEnd () ;
}

void squareLocator::myShadedDraw () {
	MPlug plug (thisMObject (), squareLocator::size) ;
	float multiplier =plug.asFloat () ;
	MPointArray vertices =squareLocator::vertices () ;

	glBegin (GL_POLYGON) ;
	for ( int i =0 ; i < vertices.length () ; i++ )
		glVertex3f (vertices [i].x * multiplier, vertices [i].y * multiplier, vertices [i].z * multiplier) ;
	glEnd () ;
}

MBoundingBox squareLocator::boundingbox (float multiplier /*=1.0f*/) {
	static MBoundingBox boundingbox ;
	if ( boundingbox.min () == boundingbox.max () ) {
		MPointArray vert =squareLocator::vertices () ;
		for ( unsigned int i =0 ; i < vert.length () ; i++ )
			boundingbox.expand (vert [i]) ;
	}
	MBoundingBox bbox (boundingbox) ;
	if ( multiplier != 1.0f ) {
		double factors [3] ={ multiplier, multiplier, multiplier } ;
		MTransformationMatrix mat ;
		mat.setScale (factors, MSpace::kWorld) ;
		bbox.transformUsing (mat.asScaleMatrix ()) ;
	}
	return (bbox) ;
}

MBoundingBox squareLocator::boundingBox () const {
	MPlug plug (thisMObject (), squareLocator::size) ;
	float multiplier =plug.asFloat () ;
	return (squareLocator::boundingbox (multiplier)) ;
}

MStatus squareLocator::initialize () {
	locatorLibBase::initializeSize (squareLocator::size) ;
	return (MS::kSuccess) ;
}

MStatus squareLocator::registerMe (MFnPlugin &plugin) {
	plugin.registerNode (
		"squareLocator",
		squareLocator::id,
		squareLocator::creator,
		squareLocator::initialize,
		MPxNode::kLocatorNode,
		&squareLocator::drawDbClassification
	) ;
	MHWRender::MDrawRegistry::registerDrawOverrideCreator (
		squareLocator::drawDbClassification,
		squareLocator::drawRegistrantId,
		squareLocatorDrawOverride::Creator
	) ;
	return (MS::kSuccess) ;
}

MStatus squareLocator::unregisterMe (MFnPlugin &plugin) {
	MHWRender::MDrawRegistry::deregisterDrawOverrideCreator (
		squareLocator::drawDbClassification,
		squareLocator::drawRegistrantId
	) ;
	return (plugin.deregisterNode (squareLocator::id)) ;
}

//---------------------------------------------------------------------------
// Viewport 2.0 override implementation
float squareLocatorDrawOverride::getMultiplier (const MDagPath &objPath) const {
	// Retrieve value of the size attribute from the node
	return (locatorLibDrawOverrideBase::getMultiplier (objPath, squareLocator::size)) ;
}

MBoundingBox squareLocatorDrawOverride::boundingBox (const MDagPath &objPath, const MDagPath &cameraPath) const {
	float multiplier =getMultiplier (objPath) ;
	return (squareLocator::boundingbox (multiplier)) ;
}

MUserData *squareLocatorDrawOverride::prepareForDraw (const MDagPath &objPath, const MDagPath &cameraPath, MUserData *oldData) {
	// Retrieve data cache (create if does not exist)
	squareLocatorData *data =dynamic_cast<squareLocatorData *>(oldData) ;
	if ( !data )
		data =new squareLocatorData () ;
	// compute data and cache it
	data->multiplier =getMultiplier (objPath) ;
	return (data) ;
}

void squareLocatorDrawOverride::draw (const MHWRender::MDrawContext &context, const MUserData *data) {
	MPointArray vertices =squareLocator::vertices () ;
	// get cached data
	float color [3] ={ 0.0f, 1.0f, 0.0f } ;
	float multiplier =1.0f ;
	const squareLocatorData *squareData =dynamic_cast<const squareLocatorData *>(data) ;
	if ( squareData )
		multiplier =squareData->multiplier ;	
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
			glBegin (GL_POLYGON) ;
			for ( int i =0 ; i < vertices.length () ; ++i )
				glVertex3f (vertices [i].x * multiplier, vertices [i].y * multiplier, vertices [i].z * multiplier) ;
			glEnd () ;
			glPopAttrib () ;
		}
		if ( displayStyle & MHWRender::MDrawContext::kWireFrame ) {
			// See myWireFrameDraw
			glBegin (GL_LINES) ;
			for ( int i =0 ; i < vertices.length () - 1 ; ++i ) {
				glVertex3f (vertices [i].x * multiplier, vertices [i].y * multiplier, vertices [i].z * multiplier) ;
				glVertex3f (vertices [i + 1].x * multiplier, vertices [i + 1].y * multiplier, vertices [i + 1].z * multiplier) ;
			}
			glEnd () ;
		}
		glPopMatrix () ;
		glMatrixMode (GL_MODELVIEW) ;
		glPopMatrix () ;
	}
}
