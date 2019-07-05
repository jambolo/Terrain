/********************************************************************************************************************

                                                       Patch.h

						                    Copyright 2006, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Terrain/Simple/Patch.h#7 $

	$NoKeywords: $

 ********************************************************************************************************************/

#pragma once


#include "Dxx/Camera.h"
#include <boost/noncopyable.hpp>
#include <memory>
#include <cassert>

class HeightField;
struct IDirect3DDevice9;
struct IDirect3DIndexBuffer9;
struct IDirect3DVertexBuffer9;



/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

class Patch : boost::noncopyable
{
public:

	enum
	{
		SIZE	= 129	// Size of a patch in each direction
	};

	struct PerformanceInfo
	{
		int		m_triangleCount;
	};

	struct DebugSettings
	{
		bool	bDrawBoundingBox;
	};

	//! Constructor
	Patch( IDirect3DDevice9 * pD3dDevice,
		   HeightField const & hf, int i0, int j0,
		   float x0, float y0, float xyScale,
		   float u0, float v0, float uvScale );

	// Destructor
	~Patch();

	//! Draws the patch
	void Draw();

	// Debugging

	PerformanceInfo const & GetPerformanceInfo() const	{ return m_performanceInfo; }
	static DebugSettings const & GetDebugSettings()		{ return m_debugSettings; }
	static void AddConsoleVariables();

private:

	struct DrawParameters
	{
		int		nPrimitives;
		int		nVertexes;
	};

	IDirect3DDevice9 *				m_pD3dDevice;
	IDirect3DVertexBuffer9 *		m_pVB;
	static IDirect3DIndexBuffer9 *	m_pIB;

	DrawParameters					m_drawParameters;

	// Debugging

	PerformanceInfo			m_performanceInfo;				// Performance info
	static DebugSettings	m_debugSettings;
};
