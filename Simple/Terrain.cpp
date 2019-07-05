/** @file *//********************************************************************************************************

                                                     Terrain.cpp

						                    Copyright 2006, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Terrain/Simple/Terrain.cpp#9 $

	$NoKeywords: $

 ********************************************************************************************************************/

#include "PrecompiledHeaders.h"

#include "Terrain.h"

#include "Patch.h"

#include <Console/Console.h>
#include <Dxx/Camera.h>
#include <Dxx/D3d.h>
#include <Dxx/VertexBufferLock.h>
#include <HeightField/HeightField.h>
#include <HeightField/HeightFieldLoader.h>
#include <Wx/Wx.h>

using namespace std;


namespace
{

	float const	XYSCALE				= 1.0f;
	float const	ZSCALE				= 135.0f;

	char const	HEIGHTFIELD_FILENAME[]	= "4_hf.tga";
	char const	BASE_TEXTURE_FILENAME[]	= "4_tm.bmp";

} // anonymous namespace

/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

Terrain::DebugSettings	Terrain::m_DebugSettings	= { false };

/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

Terrain::Terrain( IDirect3DDevice9 * pD3dDevice, char const * sFolderName, float x0, float y0 )
	: m_x0( x0 ), m_y0( y0 ),
	m_pD3dDevice( pD3dDevice ),
	m_pHeightField( 0 ),
	m_pBaseTexture( 0 )
{
	assert( sFolderName != 0 );

	pD3dDevice->AddRef();

	try
	{
		string heightFieldfPath( sFolderName );
		heightFieldfPath += '/';
		heightFieldfPath += HEIGHTFIELD_FILENAME;

		string baseTexturePath( sFolderName );
		baseTexturePath += '/';
		baseTexturePath += BASE_TEXTURE_FILENAME;

		// Load the height field

		LoadHeightField( heightFieldfPath.c_str() );

		// Load the base texture

		LoadBaseTexture( baseTexturePath.c_str() );

		// Create the patches

		CreatePatches();
	}
	catch ( ... )
	{
		this->~Terrain();
		throw;
	}
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

Terrain::~Terrain()
{
	for ( PatchVector::iterator i = m_patches.begin(); i != m_patches.end(); ++i )
	{
		Patch *		pPatch	= *i;		// Convenience

		delete pPatch;
	}

	Wx::SafeRelease( m_pBaseTexture );
	delete m_pHeightField;
	Wx::SafeRelease( m_pD3dDevice );
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

void Terrain::Update( __int64 t )
{
	(void)t;
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

void Terrain::Draw( Dxx::Camera const & camera )
{
	HRESULT		hr;

	m_PerformanceInfo.m_position		= camera.GetPosition();

	if ( m_DebugSettings.bDrawWireframe )
	{
		hr = m_pD3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
		assert_succeeded( hr );
	}

	D3DXMATRIX	world;
	D3DXMatrixTranslation( &world, m_x0, m_y0, 0.0f );
	m_pD3dDevice->SetTransform( D3DTS_WORLD, &world );

	m_pD3dDevice->SetTexture( 0, m_pBaseTexture );
	hr = m_pD3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
	hr = m_pD3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	hr = m_pD3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE );

	for ( PatchVector::iterator i = m_patches.begin(); i != m_patches.end(); ++i )
	{
		Patch *		pPatch	= *i;		// Convenience

		pPatch->Draw();
	}

	if ( m_DebugSettings.bDrawWireframe )
	{
		hr = m_pD3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
		assert_succeeded( hr );
	}
}

/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

float Terrain::GetZ( float x, float y ) const
{
	x = ( x - m_x0 ) / XYSCALE;
	y = ( y - m_y0 ) / XYSCALE;

	if ( x < 0.0f || x > m_pHeightField->GetSizeJ()-1 || y < 0.0f || y > m_pHeightField->GetSizeI()-1 )
	{
		return 0.0f;
	}

	return m_pHeightField->GetInterpolatedZ( x, y );
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

void Terrain::AddConsoleVariables()
{
	Console::Instance().AddVariable( "wireframe", &m_DebugSettings.bDrawWireframe );
}

/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

void Terrain::LoadHeightField( char const * sFileName )
{
	m_pHeightField = HeightFieldLoader::LoadTga( sFileName, ZSCALE ).release();

	assert( m_pHeightField->GetSizeI() == m_pHeightField->GetSizeJ() );
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

void Terrain::LoadBaseTexture( char const * sFileName )
{
	if ( sFileName )
	{
		HRESULT		hr;

		hr = D3DXCreateTextureFromFileEx( m_pD3dDevice, sFileName, D3DX_DEFAULT, D3DX_DEFAULT,
										D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
										D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL,
										&m_pBaseTexture);
		assert_succeeded( hr );
	}
	else
	{
		m_pBaseTexture = 0;
	}
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

void Terrain::CreatePatches()
{
	int sizeI	= m_pHeightField->GetSizeI();
	int sizeJ	= m_pHeightField->GetSizeJ();

	assert( sizeI == sizeJ );
	float const	uvScale		= 1.0f / float( sizeI );
	float const uvOffset	= 0.5f / float( sizeI );

	for ( int i = 0; i < sizeI-1; i += Patch::SIZE-1 )
	{
		float	y	= m_y0 + i * XYSCALE;
		float	v	= uvOffset + i * uvScale;

		for ( int j = 0; j < sizeJ-1; j += Patch::SIZE-1 )
		{
			float	x	= m_x0 + j * XYSCALE;
			float	u	= uvOffset + j * uvScale;

			Patch *	pPatch = new Patch( m_pD3dDevice, *m_pHeightField, i, j, x, y, XYSCALE, u, v, uvScale );
			m_patches.push_back( pPatch );
		}
	}
}


