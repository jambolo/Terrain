/** @file *//********************************************************************************************************

                                                      Terrain.h

						                    Copyright 2006, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Terrain/Simple/Terrain.h#7 $

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

	struct DebugSettings
	{
		bool	bDrawWireframe;
	};

	struct PerformanceInfo
	{
		D3DXVECTOR3	m_position;
	};

	Terrain( IDirect3DDevice9 * pD3dDevice, char const * sFolderName, float x0, float y0 );

	~Terrain();

	void Update( __int64 t );

	void Draw( Dxx::Camera const & camera );

	float GetZ( float x, float y ) const;

	// Debugging

	static void AddConsoleVariables();

	static DebugSettings	m_DebugSettings;
	PerformanceInfo			m_PerformanceInfo;

private:

	typedef std::vector< Patch * >	PatchVector;

	void LoadHeightField( char const * sFileName );
	void LoadBaseTexture( char const * sFileName );
	void CreatePatches();

	IDirect3DDevice9 *			m_pD3dDevice;
	HeightField *				m_pHeightField;
	IDirect3DTexture9 *			m_pBaseTexture;
	float						m_x0, m_y0;
	PatchVector					m_patches;

};