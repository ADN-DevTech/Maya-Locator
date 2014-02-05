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
#include "LocatorLibCmd.h"

//-----------------------------------------------------------------------------
#define kOvalFlag			"-o"
#define kOvalFlagLong		"-oval"
#define kSquareFlag			"-s"
#define kSquareFlagLong		"-square"
#define kCubeFlag			"-cu"
#define kCubeFlagLong		"-cube"
#define kSphereFlag			"-sp"
#define kSphereFlagLong		"-sphere"
#define kCustomFlag			"-c"
#define kCustomFlagLong		"-custom"

//-----------------------------------------------------------------------------
MStatus LocatorLibCmd::doIt (const MArgList &args) {
	MArgDatabase argData (syntax (), args) ;
	MString arg ;
	if ( argData.isFlagSet (kOvalFlag) ) {
		std::cerr << "the oval flag is set" << std::endl ;
		MGlobal::executeCommand ("createNode ovalLocator;") ;
	} else if ( argData.isFlagSet (kSquareFlag) ) {
		std::cerr << "the square flag is set" << std::endl ;
		MGlobal::executeCommand ("createNode squareLocator;") ;
	} else if ( argData.isFlagSet (kCubeFlag) ) {
		std::cerr << "the cube flag is set" << std::endl ;
		MGlobal::executeCommand ("createNode cubeLocator;") ;
	} else if ( argData.isFlagSet (kSphereFlag) ) {
		std::cerr << "the sphere flag is set" << std::endl ;
		MGlobal::executeCommand ("createNode sphereLocator;") ;
	} else if ( argData.isFlagSet (kCustomFlag) ) {
		std::cerr << "the custom flag is set" << std::endl ;
		MGlobal::executeCommand ("createNode customLocator;") ;
	} else {
		std::cerr << "NO flag is set" << std::endl ;
	}
	return (MS::kSuccess);
}

MSyntax LocatorLibCmd::newSyntax () {
	MSyntax syntax ;
	syntax.addFlag (kOvalFlag, kOvalFlagLong) ;
	syntax.addFlag (kSquareFlag, kSquareFlagLong) ;
	syntax.addFlag (kCubeFlag, kCubeFlagLong) ;
	syntax.addFlag (kSphereFlag, kSphereFlagLong) ;
	syntax.addFlag (kCustomFlag, kCustomFlagLong) ;
	return (syntax) ;
}