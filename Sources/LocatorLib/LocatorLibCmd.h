//
//  Copyright 2012 Autodesk, Inc.  All rights reserved.
//
//  Use of this software is subject to the terms of the Autodesk license 
//  agreement provided at the time of installation or download, or which 
//  otherwise accompanies this software in either electronic or hard copy form.   
//

// Release History
// Written by Naiqi Weng
// 1.0    Original release                     (September 1, 2012)

#pragma once

//-----------------------------------------------------------------------------
class LocatorLibCmd : public MPxCommand
{
public:
	LocatorLibCmd () : MPxCommand () {}
	virtual ~LocatorLibCmd () {}

	virtual MStatus doIt (const MArgList &args) ;

	static void *creator() { return new LocatorLibCmd () ; }
	static MSyntax	newSyntax () ;

	static MStatus registerMe (MFnPlugin &plugin) {
		return (plugin.registerCommand (_T("LocatorLibCmd"), LocatorLibCmd::creator, LocatorLibCmd::newSyntax)) ;
	}
	static MStatus unregisterMe (MFnPlugin &plugin) {
		return (plugin.deregisterCommand (_T("LocatorLibCmd"))) ;
	}

} ;
