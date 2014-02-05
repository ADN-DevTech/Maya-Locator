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
#include "cubeLocator.h"

//-----------------------------------------------------------------------------
MTypeId cubeLocator::id (0x00118383) ;
MString	cubeLocator::drawDbClassification ("drawdb/geometry/cubeLocator") ;
MString	cubeLocator::drawRegistrantId ("cubeLocatorNode") ;
MObject cubeLocator::size ;

//-----------------------------------------------------------------------------
MPointArray &cubeLocator::vertices () {
	static MPointArray vertices ;
	if ( vertices.length () != 0 )
		return (vertices) ;
	// Cube data
	static float cube [] [3] ={
		{ -0.5f, -0.5f, -0.5f }, { 0.5f, -0.5f, -0.5f }, { 0.5f, 0.5f, -0.5f }, { -0.5f, 0.5f, -0.5f }, 
		{ -0.5f, 0.5f, -0.5f }, { -0.5f, 0.5f, 0.5f }, { -0.5f, -0.5f, 0.5f }, { -0.5f, -0.5f, -0.5f }, 
		{ -0.5f, 0.5f, 0.5f }, { -0.5f, -0.5f, 0.5f }, { 0.5f, -0.5f, 0.5f }, { 0.5f, 0.5f, 0.5f }, 
		{ 0.5f, 0.5f, 0.5f }, { 0.5f, -0.5f, 0.5f }, { 0.5f, -0.5f, -0.5f }, { 0.5f, 0.5f, -0.5f }, 
		{ 0.5f, -0.5f, 0.5f }, { 0.5f, -0.5f, -0.5f }, { -0.5f, -0.5f, -0.5f }, { -0.5f, -0.5f, 0.5f }, 
		{ -0.5f, 0.5f, -0.5f }, { -0.5f, 0.5f, 0.5f }, { 0.5f, 0.5f, 0.5f }, { 0.5f, 0.5f, -0.5f }
	} ;
	int len =sizeof (cube) / sizeof (cube [0]) ;
	for ( int i =0 ; i < len ; i++ )
		vertices.append (cube [i] [0], cube [i] [1], cube [i] [2]) ;
	return (vertices) ;
}

void cubeLocator::myWireFrameDraw () {
	MPlug plug (thisMObject (), cubeLocator::size) ;
	float multiplier =plug.asFloat () ;
	MPointArray vertices =cubeLocator::vertices () ;

	for ( int i =0 ; i < vertices.length () - 3 ; i +=4 ) {
		glBegin (GL_LINE_LOOP) ;
		glVertex3f (vertices [i].x * multiplier, vertices [i].y * multiplier, vertices [i].z * multiplier) ;
		glVertex3f (vertices [i + 1].x * multiplier, vertices [i + 1].y * multiplier, vertices [i + 1].z * multiplier) ;
		glVertex3f (vertices [i + 2].x * multiplier, vertices [i + 2].y * multiplier, vertices [i + 2].z * multiplier) ;
		glVertex3f (vertices [i + 3].x * multiplier, vertices [i + 3].y * multiplier, vertices [i + 3].z * multiplier) ;
		glEnd () ;
	}
}

void cubeLocator::myShadedDraw () {
	MPlug plug (thisMObject (), cubeLocator::size) ;
	float multiplier =plug.asFloat () ;
	MPointArray vertices =cubeLocator::vertices () ;

	glBegin (GL_QUADS) ;
	for ( int i =0 ; i < vertices.length () - 3 ; i +=4 ) {
		glVertex3f (vertices [i].x * multiplier, vertices [i].y * multiplier, vertices [i].z * multiplier) ;
		glVertex3f (vertices [i + 1].x * multiplier, vertices [i + 1].y * multiplier, vertices [i + 1].z * multiplier) ;
		glVertex3f (vertices [i + 2].x * multiplier, vertices [i + 2].y * multiplier, vertices [i + 2].z * multiplier) ;
		glVertex3f (vertices [i + 3].x * multiplier, vertices [i + 3].y * multiplier, vertices [i + 3].z * multiplier) ;
	}
	glEnd () ;
}

MBoundingBox cubeLocator::boundingbox (float multiplier /*=1.0f*/) {
	static MBoundingBox boundingbox ;
	if ( boundingbox.min () == boundingbox.max () ) {
		MPointArray vert =cubeLocator::vertices () ;
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

MBoundingBox cubeLocator::boundingBox () const {
	MPlug plug (thisMObject (), cubeLocator::size) ;
	float multiplier =plug.asFloat () ;
	return (cubeLocator::boundingbox (multiplier)) ;
}

MStatus cubeLocator::initialize () {
	locatorLibBase::initializeSize (cubeLocator::size) ;
	return (MS::kSuccess) ;
}

MStatus cubeLocator::registerMe (MFnPlugin &plugin) {
	plugin.registerNode (
		"cubeLocator",
		cubeLocator::id,
		cubeLocator::creator,
		cubeLocator::initialize,
		MPxNode::kLocatorNode,
		&cubeLocator::drawDbClassification
	) ;
	MHWRender::MDrawRegistry::registerDrawOverrideCreator (
		cubeLocator::drawDbClassification,
		cubeLocator::drawRegistrantId,
		cubeLocatorDrawOverride::Creator
	) ;
	return (MS::kSuccess) ;
}

MStatus cubeLocator::unregisterMe (MFnPlugin &plugin) {
	MHWRender::MDrawRegistry::deregisterDrawOverrideCreator (
		cubeLocator::drawDbClassification,
		cubeLocator::drawRegistrantId
	) ;
	return (plugin.deregisterNode (cubeLocator::id)) ;
}

//---------------------------------------------------------------------------
// Viewport 2.0 override implementation
float cubeLocatorDrawOverride::getMultiplier (const MDagPath &objPath) const {
	// Retrieve value of the size attribute from the node
	return (locatorLibDrawOverrideBase::getMultiplier (objPath, cubeLocator::size)) ;
}

MBoundingBox cubeLocatorDrawOverride::boundingBox (const MDagPath &objPath, const MDagPath &cameraPath) const {
	float multiplier =getMultiplier (objPath) ;
	return (cubeLocator::boundingbox (multiplier)) ;
}

MUserData *cubeLocatorDrawOverride::prepareForDraw (const MDagPath &objPath, const MDagPath &cameraPath, MUserData *oldData) {
	// Retrieve data cache (create if does not exist)
	cubeLocatorData *data =dynamic_cast<cubeLocatorData *>(oldData) ;
	if ( !data )
		data =new cubeLocatorData () ;
	// compute data and cache it
	data->multiplier =getMultiplier (objPath) ;
	return (data) ;
}

void cubeLocatorDrawOverride::draw (const MHWRender::MDrawContext &context, const MUserData *data) {
	MPointArray vertices =cubeLocator::vertices () ;
	// get cached data
	float color [3] ={ 0.0f, 1.0f, 0.0f } ;
	float multiplier =1.0f ;
	const cubeLocatorData *cubeData =dynamic_cast<const cubeLocatorData *>(data) ;
	if ( cubeData )
		multiplier =cubeData->multiplier ;
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
			glBegin (GL_QUADS) ;
			for ( int i =0 ; i < vertices.length () - 3 ; i +=4 ) {
				glVertex3f (vertices [i].x * multiplier, vertices [i].y * multiplier, vertices [i].z * multiplier) ;
				glVertex3f (vertices [i + 1].x * multiplier, vertices [i + 1].y * multiplier, vertices [i + 1].z * multiplier) ;
				glVertex3f (vertices [i + 2].x * multiplier, vertices [i + 2].y * multiplier, vertices [i + 2].z * multiplier) ;
				glVertex3f (vertices [i + 3].x * multiplier, vertices [i + 3].y * multiplier, vertices [i + 3].z * multiplier) ;
			}
			glEnd () ;
			glPopAttrib () ;
		}
		if ( displayStyle & MHWRender::MDrawContext::kWireFrame ) {
			// See myWireFrameDraw
			for ( int i =0 ; i < vertices.length () - 3 ; i +=4 ) {
				glBegin (GL_LINE_LOOP) ;
				glVertex3f (vertices [i].x * multiplier, vertices [i].y * multiplier, vertices [i].z * multiplier) ;
				glVertex3f (vertices [i + 1].x * multiplier, vertices [i + 1].y * multiplier, vertices [i + 1].z * multiplier) ;
				glVertex3f (vertices [i + 2].x * multiplier, vertices [i + 2].y * multiplier, vertices [i + 2].z * multiplier) ;
				glVertex3f (vertices [i + 3].x * multiplier, vertices [i + 3].y * multiplier, vertices [i + 3].z * multiplier) ;
				glEnd () ;
			}
		}
		glPopMatrix () ;
		glMatrixMode (GL_MODELVIEW) ;
		glPopMatrix () ;
	}
}
