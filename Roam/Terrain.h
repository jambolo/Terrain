/** @file *//********************************************************************************************************

                                                      Terrain.h

						                    Copyright 2006, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Terrain/Roam/Terrain.h#2 $

	$NoKeywords: $

 ********************************************************************************************************************/

#pragma once

#include <Dxx/Camera.h>
#include <vector>
#include <d3dx9math.h>	// for PerformanceInfo::m_position

class HeightField;
struct IDirect3DDevice9;
struct IDirect3DTexture9;
struct IDirect3DVertexBuffer9;
class Patch;


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

class Terrain
{
public:

	struct PerformanceInfo
	{
		D3DXVECTOR3	position;
		int			nTriangles;
	};

	Terrain( IDirect3DDevice9 * pD3dDevice, char const * sFolderName, float x0, float y0 );

	~Terrain();

	void Update( __int64 t );

	void Draw( Dxx::Camera const & camera );

	float GetZ( float x, float y ) const;

	// Debugging

	static void AddConsoleVariables();

	PerformanceInfo			m_performanceInfo;

private:

	struct DrawParameters
	{
		int	nVertexes;
		int	nPrimitives;
	};
	typedef std::vector< Patch * >	PatchVector;

	void LoadHeightField( char const * sFileName );
	void LoadBaseTexture( char const * sFileName );
	void CreatePatches();

	IDirect3DDevice9 *			m_pD3dDevice;
	HeightField *				m_pHeightField;
	IDirect3DTexture9 *			m_pBaseTexture;
	float						m_x0, m_y0;
	DrawParameters				m_drawParameters;
	PatchVector					m_patches;

	RoamTree *	m_pLowerLeft;	// RoamTree in the lower left corner
	RoamTree *	m_pUpperRight;	// RoamTree in the upper right corner
};
