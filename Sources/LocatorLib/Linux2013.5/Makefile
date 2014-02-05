#-
# Copyright 2012 Autodesk, Inc.  All rights reserved.
# 
# Use of this software is subject to the terms of the Autodesk license
# agreement provided at the time of installation or download, or which
# otherwise accompanies this software in either electronic or hard copy form.
#+

ifndef INCL_BUILDRULES

# Include platform specific build settings
TOP := .
SRCDIR := $(TOP)/..
DSTDIR := $(TOP)
include $(TOP)/buildrules

# Always build the local plug-in when make is invoked from the
# directory.
all : plugins

endif

# Variable definitions
LocatorLib_SOURCES  := $(SRCDIR)/LocatorLib.cpp $(SRCDIR)/LocatorLibBase.cpp $(SRCDIR)/cubeLocator.cpp $(SRCDIR)/sphereLocator.cpp $(DSTDIR)/squareLocator.cpp $(SRCDIR)/ovalLocator.cpp $(SRCDIR)/Shape.cpp $(SRCDIR)/customLocator.cpp $(SRCDIR)/LocatorLibCmd.cpp
LocatorLib_OBJECTS  := $(DSTDIR)/LocatorLib.o $(DSTDIR)/LocatorLibBase.o $(DSTDIR)/cubeLocator.o $(DSTDIR)/sphereLocator.o $(DSTDIR)/squareLocator.o $(DSTDIR)/ovalLocator.o $(DSTDIR)/Shape.o $(DSTDIR)/customLocator.o $(DSTDIR)/LocatorLibCmd.o
LocatorLib_PLUGIN   := $(DSTDIR)/LocatorLib.$(EXT)
LocatorLib_MAKEFILE := $(DSTDIR)/Makefile

# Include the optional per-plugin Makefile.inc
#
#    The file can contain macro definitions such as:
#       {pluginName}_EXTRA_CFLAGS
#       {pluginName}_EXTRA_C++FLAGS
#       {pluginName}_EXTRA_INCLUDES
#       {pluginName}_EXTRA_LIBS
-include $(SRCDIR)/Makefile.inc

# Set target specific flags.
$(LocatorLib_OBJECTS): CFLAGS   := $(CFLAGS)   $(LocatorLib_EXTRA_CFLAGS)
$(LocatorLib_OBJECTS): C++FLAGS := $(C++FLAGS) $(LocatorLib_EXTRA_C++FLAGS)
$(LocatorLib_OBJECTS): INCLUDES := $(INCLUDES) $(LocatorLib_EXTRA_INCLUDES)

depend_LocatorLib:     INCLUDES := $(INCLUDES) $(LocatorLib_EXTRA_INCLUDES)

$(LocatorLib_PLUGIN):  LFLAGS   := $(LFLAGS) $(LocatorLib_EXTRA_LFLAGS) 
$(LocatorLib_PLUGIN):  LIBS     := $(LIBS)   -lOpenMaya -lFoundation $(LocatorLib_EXTRA_LIBS) 

# Rules definitions
.PHONY: depend_LocatorLib clean_LocatorLib Clean_LocatorLib

$(LocatorLib_PLUGIN): $(LocatorLib_OBJECTS) 
	-rm -f $@
	$(LD) -o $@ $(LFLAGS) $^ $(LIBS)

depend_LocatorLib :
	makedepend $(INCLUDES) $(MDFLAGS) -f$(DSTDIR)/Makefile $(LocatorLib_SOURCES)

clean_LocatorLib:
	-rm -f $(LocatorLib_OBJECTS)

Clean_LocatorLib:
	-rm -f $(LocatorLib_MAKEFILE).bak $(LocatorLib_OBJECTS) $(LocatorLib_PLUGIN)


plugins: $(LocatorLib_PLUGIN)
depend:	 depend_LocatorLib
clean:	 clean_LocatorLib
Clean:	 Clean_LocatorLib
