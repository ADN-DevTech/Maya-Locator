//
// Copyright 2012 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license 
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form. 
//

// Release History
// Written by Cyrille Fauvel
// 1.0    Original release                     (October 1, 2012)

#include "StdAfx.h"
#include "Shape.h"

//-----------------------------------------------------------------------------
ShapeMgr shapeMgr ;

//-----------------------------------------------------------------------------
bool Shape::loadFromFile (std::string filename) {
	FILE *fp =fopen (filename.c_str (), "r") ;
	if ( fp == NULL )
		return (false) ;
	int num_vertices =0 ;
	fscanf (fp, "%d\n", &num_vertices) ;
	vertices.clear () ;
	for ( int i =0 ; i < num_vertices ; i++ ) {
		float x, y, z ;
		fscanf (fp, "%f %f %f\n", &x, &y, &z) ;
		vertices.append (x, y, z) ;
	}
	int num_triangles =0 ;
	fscanf (fp, "%d\n", &num_triangles) ;
	triangles.clear () ;
	for ( int i =0 ; i < num_triangles ; i++ ) {
		int a, b, c ;
		fscanf (fp, "%d %d %d\n", &a, &b, &c) ;
		triangles.append (a) ;
		triangles.append (b) ;
		triangles.append (c) ;
	}
	fclose (fp) ;
	return (true) ;
}

void Shape::renderWireFrame (float multiplier /*=1.0f*/) {
	if ( triangles.length () == 0 ) {
		glBegin (GL_LINES) ;
		for ( int i =0 ; i < vertices.length () - 1 ; i++ ) {
			glVertex3f (vertices [i].x * multiplier, vertices [i].y * multiplier, vertices [i].z * multiplier) ;
			glVertex3f (vertices [i + 1].x * multiplier, vertices [i + 1].y * multiplier, vertices [i + 1].z * multiplier) ;
		}
		glEnd () ;
	} else {
		for ( int i =0 ; i < triangles.length () - 2 ; i +=3 ) {
			glBegin (GL_LINE_LOOP) ;
			glVertex3f (vertices [triangles [i]].x * multiplier, vertices [triangles [i]].y * multiplier, vertices [triangles [i]].z * multiplier) ;
			glVertex3f (vertices [triangles [i + 1]].x * multiplier, vertices [triangles [i + 1]].y * multiplier, vertices [triangles [i + 1]].z * multiplier) ;
			glVertex3f (vertices [triangles [i + 2]].x * multiplier, vertices [triangles [i + 2]].y * multiplier, vertices [triangles [i + 2]].z * multiplier) ;
			glEnd () ;
		}
	}
}

void Shape::renderShaded (float multiplier /*=1.0f*/) {
	if ( triangles.length () == 0 ) {
		glBegin (GL_POLYGON) ;
		for ( int i =0 ; i < vertices.length () ; i++ )
			glVertex3f (vertices [i].x * multiplier, vertices [i].y * multiplier, vertices [i].z * multiplier) ;
		glEnd () ;
	} else {
		glBegin (GL_TRIANGLES) ;
		for ( int i =0 ; i < triangles.length () - 2 ; i +=3 ) {
			glVertex3f (vertices [triangles [i]].x * multiplier, vertices [triangles [i]].y * multiplier, vertices [triangles [i]].z * multiplier) ;
			glVertex3f (vertices [triangles [i + 1]].x * multiplier, vertices [triangles [i + 1]].y * multiplier, vertices [triangles [i + 1]].z * multiplier) ;
			glVertex3f (vertices [triangles [i + 2]].x * multiplier, vertices [triangles [i + 2]].y * multiplier, vertices [triangles [i + 2]].z * multiplier) ;
		}
		glEnd () ;
	}
}

MBoundingBox Shape::boundingbox (float multiplier /*=1.0f*/) {
	static MBoundingBox boundingbox ;
	if ( boundingbox.min () == boundingbox.max () ) {
		for ( unsigned int i =0 ; i < vertices.length () ; i++ )
			boundingbox.expand (vertices [i]) ;
	}
	MBoundingBox bbox (boundingbox) ;
	if ( multiplier != 1.0 ) {
		double factors [3] ={ multiplier, multiplier, multiplier } ;
		MTransformationMatrix mat ;
		mat.setScale (factors, MSpace::kWorld) ;
		bbox.transformUsing (mat.asScaleMatrix ()) ;
	}
	return (bbox) ;
}

//-----------------------------------------------------------------------------
ShapeMgr::ShapeMgr () {
	Shape &shape =shapeList ["default"] ;
	shape.vertices.append (-0.5f, 0.0f, 0.5f) ;
	shape.vertices.append (0.5f, 0.0f, 0.5f) ; 
	shape.vertices.append (0.5f, 0.0f, -0.5f) ;
	shape.vertices.append (-0.5f, 0.0f, -0.5f) ;
	shape.vertices.append (-0.5f, 0.0f, 0.5f) ;
}

Shape &ShapeMgr::shapeData (std::string filename) {
	if ( filename.length () == 0 )
		filename ="default" ;
	if ( shapeList.find (filename) == shapeList.end () ) {
		Shape &shape =shapeList [filename] ;
		shape.loadFromFile (filename) ;
		return (shape) ;
	}
	return ((*(shapeList.find (filename))).second) ;
}
