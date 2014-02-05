//
// Copyright 2012 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license 
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form. 
//

// Release History
// Written by Cyrille Fauvel
// 1.0    Original release                     (September 1, 2012)

#pragma once

//-----------------------------------------------------------------------------
class Shape {
public:
	MPointArray	vertices ;
	MIntArray triangles ;

public:
	Shape () {}
	virtual ~Shape () {}

	virtual bool loadFromFile (std::string filename) ;
	virtual void renderWireFrame (float multiplier =1.0f) ;
	virtual void renderShaded (float multiplier =1.0f) ;
	virtual MBoundingBox boundingbox (float multiplier =1.0f) ;

} ;

//-----------------------------------------------------------------------------
class ShapeMgr {
public:
	std::map<std::string, Shape> shapeList ;

public:
	ShapeMgr () ;
	virtual ~ShapeMgr () {}

	Shape &shapeData (std::string filename) ;

} ;

extern ShapeMgr shapeMgr ;
