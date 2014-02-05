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
#include "ovalLocator.h"

//-----------------------------------------------------------------------------
// Testing id for now
MTypeId ovalLocator::id (0x00118382) ;
MString	ovalLocator::drawDbClassification ("drawdb/geometry/ovalLocator") ;
MString	ovalLocator::drawRegistrantId ("overLocatorNode") ;
MObject ovalLocator::size ;

//-----------------------------------------------------------------------------
MPointArray &ovalLocator::vertices () {
	static MPointArray vertices ;
	if ( vertices.length () != 0 )
		return (vertices) ;
	// Oval data
	static float oval [] [3] ={
		{ -2.00f, 0.0f, 0.0f }, { -1.90f, 0.0f, 0.309f }, { -1.618f, 0.0f, 0.588f }, { -1.176f, 0.0f, 0.809f }, 
		{ -0.618f, 0.0f, 0.951f }, { 0.0f, 0.0f, 1.00f }, { 0.618f, 0.0f, 0.951f}, { 1.176f, 0.0f, 0.809f}, 
		{ 1.618f, 0.0f, 0.588f}, { 1.90f, 0.0f, 0.309f}, { 2.00f, 0.0f, 0.0f}, { 1.90f, 0.0f, -0.309f}, 
		{ 1.618f, 0.0f, -0.588f}, { 1.176f, 0.0f, -0.809f}, { 0.618f, 0.0f, -0.951f}, { 0.0f, 0.0f, -1.00f }, 
		{ -0.618f, 0.0f, -0.951f}, { -1.176f, 0.0f, -0.809f}, { -1.618f, 0.0f, -0.588f}, { -1.90f, 0.0f, -0.309f}, 
		{ -2.00f, 0.0f, 0.0f }
	} ;
	int len =sizeof (oval) / sizeof (oval [0]) ;
	for ( int i =0 ; i < len ; i++ )
		vertices.append (oval [i] [0], oval [i] [1], oval [i] [2]) ;
	return (vertices) ;
}

void ovalLocator::myWireFrameDraw () {
	MPlug plug (thisMObject (), ovalLocator::size) ;
	float multiplier =plug.asFloat () ;
	MPointArray vertices =ovalLocator::vertices () ;

	glBegin (GL_LINES) ;
	for ( int i =0 ; i < vertices.length () - 1 ; i++ ) {
		glVertex3f (vertices [i].x * multiplier, vertices [i].y * multiplier, vertices [i].z * multiplier) ;
		glVertex3f (vertices [i + 1].x * multiplier, vertices [i + 1].y * multiplier, vertices [i + 1].z * multiplier) ;
	}
	glEnd () ;
}

void ovalLocator::myShadedDraw () {
	MPlug plug (thisMObject (), ovalLocator::size) ;
	float multiplier =plug.asFloat () ;
	MPointArray vertices =ovalLocator::vertices () ;

	glBegin (GL_TRIANGLE_FAN) ;
	glVertex3f (0, 0, 0) ;
	for ( int i =0 ; i < vertices.length () ; i++ )
		glVertex3f (vertices [i].x * multiplier, vertices [i].y * multiplier, vertices [i].z * multiplier) ;
	glEnd () ;
}

MBoundingBox ovalLocator::boundingbox (float multiplier /*=1.0f*/) {
	static MBoundingBox boundingbox ;
	if ( boundingbox.min () == boundingbox.max () ) {
		MPointArray vert =ovalLocator::vertices () ;
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

MBoundingBox ovalLocator::boundingBox () const {
	MPlug plug (thisMObject (), ovalLocator::size) ;
	float multiplier =plug.asFloat () ;
	return (ovalLocator::boundingbox (multiplier)) ;
}

MStatus ovalLocator::initialize () {
	locatorLibBase::initializeSize (ovalLocator::size) ;
	return (MS::kSuccess) ;
}

MStatus ovalLocator::registerMe (MFnPlugin &plugin) {
	plugin.registerNode (
		"ovalLocator",
		ovalLocator::id,
		ovalLocator::creator,
		ovalLocator::initialize,
		MPxNode::kLocatorNode,
		&ovalLocator::drawDbClassification
	) ;
	MHWRender::MDrawRegistry::registerDrawOverrideCreator (
		ovalLocator::drawDbClassification,
		ovalLocator::drawRegistrantId,
		ovalLocatorDrawOverride::Creator
	) ;
	return (MS::kSuccess) ;
}

MStatus ovalLocator::unregisterMe (MFnPlugin &plugin) {
	MHWRender::MDrawRegistry::deregisterDrawOverrideCreator (
		ovalLocator::drawDbClassification,
		ovalLocator::drawRegistrantId
	) ;
	return (plugin.deregisterNode (ovalLocator::id)) ;
}

//---------------------------------------------------------------------------
// Viewport 2.0 override implementation
float ovalLocatorDrawOverride::getMultiplier (const MDagPath &objPath) const {
	// Retrieve value of the size attribute from the node
	return (locatorLibDrawOverrideBase::getMultiplier (objPath, ovalLocator::size)) ;
}

MBoundingBox ovalLocatorDrawOverride::boundingBox (const MDagPath &objPath, const MDagPath &cameraPath) const {
	float multiplier =getMultiplier (objPath) ;
	return (ovalLocator::boundingbox (multiplier)) ;
}

MUserData *ovalLocatorDrawOverride::prepareForDraw (const MDagPath &objPath, const MDagPath &cameraPath, MUserData *oldData) {
	// Retrieve data cache (create if does not exist)
	ovalLocatorData *data =dynamic_cast<ovalLocatorData *>(oldData) ;
	if ( !data )
		data =new ovalLocatorData () ;
	// compute data and cache it
	data->multiplier =getMultiplier (objPath) ;
	return (data) ;
}

void ovalLocatorDrawOverride::draw (const MHWRender::MDrawContext &context, const MUserData *data) {
	MPointArray vertices =ovalLocator::vertices () ;
	// get cached data
	float color [3] ={ 0.0f, 1.0f, 0.0f } ;
	float multiplier =1.0f ;
	const ovalLocatorData *ovalData =dynamic_cast<const ovalLocatorData *>(data) ;
	if ( ovalData )
		multiplier =ovalData->multiplier ;	
	// get state data
	MStatus status ;
	const MMatrix transform =context.getMatrix (MHWRender::MDrawContext::kWorldViewMtx, &status) ;
	if ( status !=MStatus::kSuccess )
		return ;
	const MMatrix projection =context.getMatrix (MHWRender::MDrawContext::kProjectionMtx, &status) ;
	if ( status !=MStatus::kSuccess )
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
			glBegin (GL_TRIANGLE_FAN) ;
			glVertex3f (0, 0, 0) ;
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
