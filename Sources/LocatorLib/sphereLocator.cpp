//
// Copyright 2012 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license 
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form. 
//

// Release History
// Written by Naiqi Weng
// 1.0    Original release                     (October 1, 2012)

#include "StdAfx.h"
#define MNoVersionString
#define MNoPluginEntry
#include <maya/MFnPlugin.h>
#include "sphereLocator.h"

//-----------------------------------------------------------------------------
MTypeId sphereLocator::id (0x00118384) ;
MString	sphereLocator::drawDbClassification ("drawdb/geometry/sphereLocator") ;
MString	sphereLocator::drawRegistrantId ("sphereLocatorNode") ;
MObject sphereLocator::size ;

//-----------------------------------------------------------------------------
MPointArray &sphereLocator::vertices () {
	static MPointArray vertices ;
	if ( vertices.length () != 0 )
		return (vertices) ;
	// Construct the sphere
	double u =-M_PI_2 ;
	double v =-M_PI ;
	double u_delta =M_PI / 32.00 ; 
	double v_delta =2 * M_PI / 32.00 ; 
	vertices.append (MPoint (0.0, -0.5, 0.0)) ;
	for ( int i =0 ; i<31 ; i++ ) {
		u +=u_delta ;
		v =-M_PI ;
		for ( int j =0 ; j < 32 ; j++ ) {
			double x =0.5 * cos (u) * cos (v) ;
			double y =0.5 * sin (u) ;
			double z =0.5 * cos (u) * sin (v) ;
			MPoint pnt (x, y, z) ;
			vertices.append (pnt) ;
			v +=v_delta ;
		}
	}
	vertices.append (MPoint (0.0, 0.5, 0.0)) ;
	return (vertices) ;
}

void sphereLocator::myWireFrameDraw () {
	MPlug plug (thisMObject (), sphereLocator::size) ;
	float multiplier =plug.asFloat () ;
	MPointArray vertices =sphereLocator::vertices () ;

	glBegin (GL_LINE_STRIP) ;
	for ( int i =0 ; i < vertices.length () ; i++ )
		glVertex3f (vertices [i].x * multiplier, vertices [i].y * multiplier, vertices [i].z * multiplier) ;
	glEnd () ;
}

void sphereLocator::myShadedDraw () {
	MPlug plug (thisMObject (), sphereLocator::size) ;
	float multiplier =plug.asFloat () ;
	MPointArray vertices =this->vertices () ;

	glBegin (GL_TRIANGLE_FAN) ;
	for ( int i =0 ; i < vertices.length () ; i++ )
		glVertex3f (vertices [i].x * multiplier, vertices [i].y * multiplier, vertices [i].z * multiplier) ;
	glEnd () ;
}

MBoundingBox sphereLocator::boundingbox (float multiplier /*=1.0f*/) {
	static MBoundingBox boundingbox ;
	if ( boundingbox.min () == boundingbox.max () ) {
		MPointArray vert =sphereLocator::vertices () ;
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

MBoundingBox sphereLocator::boundingBox () const {
	MPlug plug (thisMObject (), sphereLocator::size) ;
	float multiplier =plug.asFloat () ;
	return (sphereLocator::boundingbox (multiplier)) ;
}

MStatus sphereLocator::initialize () {
	locatorLibBase::initializeSize (sphereLocator::size) ;
	return (MS::kSuccess) ;
}

MStatus sphereLocator::registerMe (MFnPlugin &plugin) {
	plugin.registerNode (
		"sphereLocator",
		sphereLocator::id,
		sphereLocator::creator,
		sphereLocator::initialize,
		MPxNode::kLocatorNode,
		&sphereLocator::drawDbClassification
	) ;
	MHWRender::MDrawRegistry::registerDrawOverrideCreator (
		sphereLocator::drawDbClassification,
		sphereLocator::drawRegistrantId,
		sphereLocatorDrawOverride::Creator
	) ;
	return (MS::kSuccess) ;
}

MStatus sphereLocator::unregisterMe (MFnPlugin &plugin) {
	MHWRender::MDrawRegistry::deregisterDrawOverrideCreator (
		sphereLocator::drawDbClassification,
		sphereLocator::drawRegistrantId
	) ;
	return (plugin.deregisterNode (sphereLocator::id)) ;
}

//---------------------------------------------------------------------------
// Viewport 2.0 override implementation
float sphereLocatorDrawOverride::getMultiplier (const MDagPath &objPath) const {
	// Retrieve value of the size attribute from the node
	return (locatorLibDrawOverrideBase::getMultiplier (objPath, sphereLocator::size)) ;
}

MBoundingBox sphereLocatorDrawOverride::boundingBox (const MDagPath &objPath, const MDagPath &cameraPath) const {
	float multiplier =getMultiplier (objPath) ;
	return (sphereLocator::boundingbox (multiplier)) ;
}

MUserData *sphereLocatorDrawOverride::prepareForDraw (const MDagPath &objPath, const MDagPath &cameraPath, MUserData *oldData) {
	// Retrieve data cache (create if does not exist)
	sphereLocatorData *data =dynamic_cast<sphereLocatorData *>(oldData) ;
	if ( !data )
		data =new sphereLocatorData () ;
	// compute data and cache it
	data->multiplier =getMultiplier (objPath) ;
	return (data) ;
}

void sphereLocatorDrawOverride::draw (const MHWRender::MDrawContext &context, const MUserData *data) {
	// Construct the sphere shape
	MPointArray vertices =sphereLocator::vertices () ;
	// get cached data
	float color [3] ={ 0.0f, 1.0f, 0.0f } ;
	float multiplier =1.0f ;
	const sphereLocatorData *sphereData =dynamic_cast<const sphereLocatorData *>(data) ;
	if ( sphereData )
		multiplier =sphereData->multiplier ;	
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
			glBegin (GL_TRIANGLE_FAN) ;
			for ( int i =0 ; i < vertices.length () ; ++i )
				glVertex3f (vertices [i].x * multiplier, vertices [i].y * multiplier, vertices [i].z * multiplier) ;	
			glEnd () ;
			glPopAttrib () ;
		}
		if ( displayStyle & MHWRender::MDrawContext::kWireFrame ) {
			// See myWireFrameDraw
			glBegin (GL_LINE_STRIP) ;
			for ( int i =0 ; i < vertices.length () ; ++i )
				glVertex3f (vertices [i].x * multiplier, vertices [i].y * multiplier, vertices [i].z * multiplier) ;
			glEnd () ;
		}
		glPopMatrix () ;
		glMatrixMode (GL_MODELVIEW) ;
		glPopMatrix () ;
	}
}
