Copyright (c) Autodesk, Inc. All rights reserved 

Maya Locator sample
by Cyrille Fauvel / Naiqi Weng - Autodesk Developer Network (ADN)
October 2012

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


Maya Locator Library - Plug-in of the Month, October 2012
=======================================

Description
-----------

This plug-in allows users to create different shapes of locators other than Maya’s internal built-in locator. It provides a custom command, which has 5 built-in flags, 
allowing users to create 4 different basic types of locator, specifically an oval-shape locator, a square-shape locator, a cube-shape locator and a sphere-shape locator.

It also provide a 'custom' locator which allow the use to define its own locator shape. There is 2 functional modes for the 'custom'.

- Shape mode

   The user can define a simple shape (2d or 3d) as a continuous closed polyline. See the shape.txt in the Samples folder for details.
   This mode accepts files with a serie of 3d coordinates X, Y, Z

- Solid mode

   The user can define a simple solid as a serie of triangles. See the pyramid.txt in the Samples folder for details. This mode accepts files 
   with a serie of 3d coordinates X, Y, Z for vertices, and then a serie of triangle vertex index defined above.

   
Execute “help LocatorLibCmd” for more details.


'Shape mode' file format description
------------------------------------

This is the simple 'Shape mode' file structure.

<NNN>							# Number of vertex (integer)
<FFF> <FFF> <FFF>				# #0 Vertex X, Y, Z coordinates (float)
<FFF> <FFF> <FFF>				# #1 Vertex X, Y, Z coordinates (float)
...
<FFF> <FFF> <FFF>				# #NNN Vertex X, Y, Z coordinates (float)
0								# <null> to terminate

We usually recommend working with a unit size shape, but this is not limited to any size.


'Solid mode' file format description
------------------------------------

This is the simple 'Solid mode' file structure.
<NNN>							# Number of vertex (integer)
<FFF> <FFF> <FFF>				# #0 Vertex X, Y, Z coordinates (float)
<FFF> <FFF> <FFF>				# #1 Vertex X, Y, Z coordinates (float)
...
<FFF> <FFF> <FFF>				# #NNN Vertex X, Y, Z coordinates (float)
<TTT>							# Number of triangles (integer)
<III> <III> <III>				# 1st Triangle Vertex indexes (integer)
<III> <III> <III>				# 2nd Triangle Vertex indexes (integer)
...
<III> <III> <III>				# TTT Triangle Vertex indexes (integer)

We usually recommend working with a unit size shape, but this is not limited to any size.
The Vertex index starts at index 0. The triangle' vertex index runs from 0 to NNN-1


System Requirements
-------------------
This plug-in has been tested with Autodesk Maya 2013 and 2013 Extension. 

This plug-in is a C++ based plug-in, so it does require compilation in case you want to modify it or move to a newer Maya version. It works on Window 32bit/64bit, Mac OS, and Linux.


Installation
------------
The following steps are for using the plug-in with Autodesk Maya 2013 or 2013 Extension. If you are using Maya 2013 Extension, please 
consider the release number as "2013.5" in the paths listed below.  

1. Create a directory structure like this:

    -- myDirectory  --+-- plug-ins
	                              +-- scripts

2. Copy the scripts folder into your created directory.

3. Copy the plug-in binary into the plug-ins folder (.mll for windows, .bundle for OSX, .so for Linux)

4. Create a module file. For this go into:

    Windows 7
    C:\users\<your login>\Documents\maya\<version>[.5][-x64]\modules

    MacOS
    /Users/<your login>/Library/Preferences/Autodesk/maya/<version>[.5]-x64/modules

    Linux
    /home/<your login>/maya/modules
    or /home/<your login>/maya/2013[.5]-x64/modules

4. Edit the new LocatorLib.mod file in a text editor and modify the path <mypath> and <version>, to reflect the location where you copied the files.

5. Once installed, the "LocatorLib" plug-in will be available in your Maya plug-in manager. Open the plug-in manager, load
the plug-in, and select the 'Auto load' option. The plug-in manager can be found in:
      ->Window->Settings/Preferences/Plug-in manager

	  
Usage
-----
Inside Maya, call the command 'LocatorLibCmd' with one of the following options:
  -o or -oval to create an oval locator
  -s or -square to create a square locator
  -cu or -cube to create a cube locator
  -sp or -sphere to create to create a sphere locator
  -c or -custom to create a custom locator

You can also create an instance of the node locator by using the MEL command 'createNode'. I.e.:
  createNode ovalLocator;
  createNode squareLocator;
  createNode cubeLocator;
  createNode sphereLocator;
  createNode customLocator;

  
Uninstallation
--------------
Simply removing the "LocatorLib.mod" files from your system folder, and delete the plug-in module folder will uninstall the plug-in.

Author
------
This plug-in was written by Naiq Weng and Cyrille Fauvel from the Autodesk Developer Network team. 

Further Reading
---------------
For more information on developing with Maya, please visit the [Maya Developer Center at http://www.autodesk.com/developmaya](http://www.autodesk.com/developmaya)

Feedback
--------
Email us at labs.plug-ins@autodesk.com with feedback or requests for enhancements.


Release History
---------------

1.0    Original release                     (October 1, 2012)
