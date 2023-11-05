#ifndef _STDAFX_H_
#define _STDAFX_H_

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 
// Put any headers here that won't change (like fstream or Ogre.h).  DO NOT put any
// ssuge-related headers here.  Make sure, in every .h and .cpp file we create in the
// project, to include <stdafx.h> first (it's mostly a Visual Studio thing, but it is also
// good so we don't have to include the headers (indirectly) included here in our files.
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ 

// system headers
#include <fstream>
#include <string>
#include <sstream>

// Ogre headers
#pragma warning(disable : 4251)				// Gets rid of some pesky dll-linkage warnings 
#pragma warning(disable : 4275)
#include <Ogre.h>
#include <OgreApplicationContext.h>

// SDL headers
#include <SDL.h>

// TinyXML
#include <tinyxml2.h>

// Lua
#include <lua.hpp>

#endif
