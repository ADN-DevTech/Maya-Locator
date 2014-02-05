//
//  Copyright 2012 Autodesk, Inc.  All rights reserved.
//
//  Use of this software is subject to the terms of the Autodesk license 
//  agreement provided at the time of installation or download, or which 
//  otherwise accompanies this software in either electronic or hard copy form.   
//

//-----------------------------------------------------------------------------
//- StdAfx.h : include file for standard system include files,
//-      or project specific include files that are used frequently,
//-      but are changed infrequently
//-----------------------------------------------------------------------------
#pragma once

#ifdef NT_PLUGIN
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")
#endif

////-----------------------------------------------------------------------------
//----- This file is preprocessor symbol driven.
//----- Define:
//-----		_PYTHON_MODULE_ to include and import Python headers and libs in your project.
//#define _PYTHON_MODULE_
//-----		_MAYA_QT_ to include and import Maya QT headers and libs in your project.
//#define _MAYA_QT_

#include "mayaHeaders.h"
#ifdef NT_PLUGIN
#include <tchar.h>
#else
#define _T(a) a
#endif
#include <string>
#include <map>

// Viewport 2.0 includes
#include <maya/MDrawRegistry.h>
#include <maya/MPxDrawOverride.h>
#include <maya/MUserData.h>
#include <maya/MDrawContext.h>
#include <maya/MViewport2Renderer.h>
#include <maya/MHWGeometryUtilities.h>

#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif

#ifndef M_PI_2
#define M_PI_2		1.57079632679489661923
#endif
