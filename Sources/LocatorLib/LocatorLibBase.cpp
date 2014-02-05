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
#include "LocatorLibBase.h"

//-----------------------------------------------------------------------------
void locatorLibBase::draw (M3dView &view, const MDagPath &path, M3dView::DisplayStyle style, M3dView::DisplayStatus status) {
	view.beginGL () ;
	if (   (style == M3dView::kFlatShaded)
		|| (style == M3dView::kGouraudShaded)
	) {
		// Push the color settings
		glPushAttrib (GL_CURRENT_BIT) ;
		if ( status == M3dView::kActive )
			view.setDrawColor (13, M3dView::kActiveColors) ;
		else
			view.setDrawColor (13, M3dView::kDormantColors) ;
		myShadedDraw () ;
		glPopAttrib () ;
	}
	myWireFrameDraw () ; // Draw the outline of the locator
	view.endGL () ;
}

MStatus locatorLibBase::initializeSize (MObject &attr) {
	MStatus stat ;
	MFnNumericAttribute attrFn ;
	attr =attrFn.create ("size", "sz", MFnNumericData::kFloat, 1.0f, &stat) ;
	attrFn.setMin (0.01f) ;
	attrFn.setMax (100.0f) ;
	addAttribute (attr) ;
	return (MS::kSuccess) ;
}

//-----------------------------------------------------------------------------
float locatorLibDrawOverrideBase::getMultiplier (const MDagPath &objPath, MObject &attr) const {
	// Retrieve value of the size attribute from the node
	MStatus status ;
	MObject locatorNode =objPath.node (&status) ;
	if ( status ) {
		MPlug plug (locatorNode, attr) ;
		if ( !plug.isNull () ) {
			double sizeVal ;
			if ( plug.getValue (sizeVal) )
				return (sizeVal) ;
		}
	}
	return (1.00) ;
}
