/*********************************************************************************************************************

                                                 PrecompiledHeaders.h

						                    Copyright 2006, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Terrain/Common/Hud/PrecompiledHeaders.h#2 $

	$NoKeywords: $

*********************************************************************************************************************/

#pragma once


/********************************************************************************************************************/
/*																													*/
/* Windows header files																								*/
/*																													*/
/********************************************************************************************************************/

#define STRICT
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <windows.h>
//#include <objbase.h>
//#include <atlbase.h>
//#include <atlcomcli.h>
//#include <msxml2.h>
//#include <mmsystem.h>
//#include <scrnsave.h>

/********************************************************************************************************************/
/*																													*/
/* Direct X header files																							*/
/*																													*/
/********************************************************************************************************************/

#if defined( _DEBUG )
	#define D3D_DEBUG_INFO
#endif
#include <d3d9.h>
#include <d3dx9.h>
//#include <dinput.h>
//#include <dsound.h>
//#include <dxerr9.h>


/********************************************************************************************************************/
/*																													*/
/* OpenGL header files																								*/
/*																													*/
/********************************************************************************************************************/

//#include <gl/gl.h>
//#include <gl/glu.h>
//#include <gl/glaux.h>


/********************************************************************************************************************/
/*																													*/
/* Standard Library header files																									*/
/*																													*/
/********************************************************************************************************************/

//#include <algorithm>
//#include <bitset>
//#include <cassert>
//#include <cmath>
//#include <cstdio>
//#include <cstdlib>
//#include <cstring>
//#include <ctime>
//#include <deque>
//#include <errno.h>
//#include <exception>
//#include <fstream>
//#include <functional>
//#include <ios>
//#include <iosfwd>
//#include <iostream>
//#include <istream>
//#include <limits>
//#include <list>
//#include <locale>
#include <map>
//#include <memory>
//#include <new>
//#include <queue>
#include <sstream>
//#include <stack>
//#include <string>
//#include <vector>
//#include <xutility>


/********************************************************************************************************************/
/*																													*/
/* Library header files																								*/
/*																													*/
/********************************************************************************************************************/

#include <boost/noncopyable.hpp>
//#include <Misc/Assert.h>
//#include <Misc/Etc.h>
//#include <Misc/exceptions.h>
//#include <Misc/max.h>
//#include <Misc/Random.h>
//#include <Misc/SafeStr.h>
//#include <Misc/Singleton.h>
//#include <Misc/Trace.h>
//#include <Misc/Types.h>
#include <Wx/Wx.h>


/********************************************************************************************************************/
/*																													*/
/* Project header files																								*/
/*																													*/
/********************************************************************************************************************/
