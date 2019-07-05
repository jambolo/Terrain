/** @file *//********************************************************************************************************

                                                     Terrain.cpp

						                    Copyright 2006, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Terrain/Roam/Terrain.cpp#2 $

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

	struct DebugSettings
	{
		bool	bDrawWireframe;
	};

	DebugSettings	s_debugSettings	= { false };


} // anonymous namespace

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

	assert( s_pHeightField->GetSizeI() == s_pHeightField->GetSizeJ() );

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
//	RoamTerrain	terrain( s_pLowerLeftData, s_pUpperRightData,
//						 RoamTree::SplitParameters( s_pCamera->GetPosition(),
//													s_pCamera->GetDirection(),
//													s_pCamera->GetAngleOfView() * s_pViewport->GetAspectRatio(),
//													.005f ) );
//
//	DrawRoamTree( *terrain.m_pLowerLeft );
//	DrawRoamTree( *terrain.m_pUpperRight );
//
//
//	for ( RoamTree::Node const * pNode = tree.FirstLeaf(); pNode != 0; pNode = tree.NextLeaf( pNode ) )
//	{
//		pNode->GetV0()->Apply();
//		pNode->GetV1()->Apply();
//		pNode->GetV2()->Apply();
//
//		s_VertexCount += 3;
//
//		s_TriangleCount += 1;
//	}
//
//	if ( s_debugSettings.bDrawWireframe )
//	{
//		hr = m_pD3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
//		assert_succeeded( hr );
//	}
//
//	D3DXMATRIX	world;
//	D3DXMatrixIdentity( &world );
//	m_pD3dDevice->SetTransform( D3DTS_WORLD, &world );
//
//	m_pD3dDevice->SetTexture( 0, m_pBaseTexture );
//	hr = m_pD3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
//	hr = m_pD3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
//	hr = m_pD3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE );
//
//	hr = m_pD3dDevice->SetStreamSource( 0, m_pVB, 0, sizeof( VBEntry ) );
//	assert_succeeded( hr );
//
//	hr = m_pD3dDevice->SetFVF( FVF );
//	assert_succeeded( hr );
//
//	hr = m_pD3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, m_drawParameters.nPrimitives );
//	assert_succeeded( hr );
//
//	if ( s_debugSettings.bDrawWireframe )
//	{
//		hr = m_pD3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
//		assert_succeeded( hr );
//	}
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
	Console::Instance().AddVariable( "wireframe", &s_debugSettings.bDrawWireframe );
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

void Terrain::CreateGeometry()
{
//	HRESULT	hr;
//
//	int sizeI	= m_pHeightField->GetSizeI();
//	int sizeJ	= m_pHeightField->GetSizeJ();
//
//	// Compute the terrain vertices
//
//	s_paVertexGrid = new Glx::TLVertex[ s_pHeightField->GetSizeI() * s_pHeightField->GetSizeJ() ];
//	if ( !s_paVertexGrid ) throw std::bad_alloc();
//
//	int const					sx		= s_pHeightField->GetSizeI();
//	int const					sy		= s_pHeightField->GetSizeJ();
//	HeightField::Vertex const *	pData	= s_pHeightField->GetData();
//	float const					s		= s_pHeightField->GetScale();
//	float const					x0		= -( sx - 1 ) * .5f * s;
//	float const					y0		= -( sy - 1 ) * .5f * s;
//
//	for ( int i = 0; i < sy; i ++ )
//	{
//		for ( int j = 0; j < sx; j ++ )
//		{
//			Glx::TLVertex * const	p	= &s_paVertexGrid[ i * s_pHeightField->GetSizeI() + j ];
//
//			p->m_Position.m_X			= x0 + j * s;
//			p->m_Position.m_Y			= y0 + i * s;
//			p->m_Position.m_Z			= s_pHeightField->GetData( j, i )->m_Z;
//			p->m_Normal					= ComputeGridNormal( *s_pHeightField, j, i );
//			p->m_TexturePosition.m_X	= j * .125f;
//			p->m_TexturePosition.m_Y	= i * .125f;
//		}
//	}
//
//	assert( s_pHeightField->GetSizeI() == s_pHeightField->GetSizeJ() );
//
//	s_pLowerLeftData = new DataTree( GridTriangle( 0, 0, GridTriangle::O_UPRIGHT, s_pHeightField->GetSizeI()-1 ),
//									s_paVertexGrid,
//									s_pHeightField->GetSizeI() );
//	if ( !s_pLowerLeftData ) throw std::bad_alloc();
//
//	s_pUpperRightData = new DataTree( GridTriangle( s_pHeightField->GetSizeI()-1, s_pHeightField->GetSizeI()-1,
//													GridTriangle::O_DOWNLEFT, s_pHeightField->GetSizeI()-1 ),
//									s_paVertexGrid,
//									s_pHeightField->GetSizeI() );
//	if ( !s_pUpperRightData ) throw std::bad_alloc();
//
//	// Strip the grid to get the order of the vertexes
//
//	int nVertexes	= (sizeI-1) * ( 2 * sizeJ + 2 ) - 2;
//	vector<uint32>	vbToHfMap( nVertexes );
//	int			n;
//
//	n = Dxx::StripGrid32( sizeJ-1, sizeI-1, &vbToHfMap[0] );
//	assert( n == nVertexes );	// Make sure the number of vertexes is right (if it hasn't already crashed)
//
//	m_drawParameters.nPrimitives	= nVertexes - 2;	// A single triangle strip
//	m_drawParameters.nVertexes		= nVertexes; 
//
//	IDirect3DVertexBuffer9 *	pVB;
//
//	hr = m_pD3dDevice->CreateVertexBuffer( m_drawParameters.nVertexes * sizeof( VBEntry ),
//										   D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED,
//										   &pVB, NULL );
//	assert_succeeded( hr );
//
//	if ( pVB != 0 )
//	{
//		float const	uvScaleJ	= 1.0f / float( sizeI );
//		float const uvOffsetJ	= 0.5f / float( sizeI );
//		float const	uvScaleI	= 1.0f / float( sizeJ );
//		float const uvOffsetI	= 0.5f / float( sizeJ );
//
//		// Initialize it
//
//		Dxx::VertexBufferLock	lock( pVB );
//
//		VBEntry *	paVertexEntries	= static_cast< VBEntry* >( lock.GetLockedBuffer() );
//
//		for ( int k = 0; k < nVertexes; ++k )
//		{
//			int		hfIndex	= vbToHfMap[k];
//			int		i 		= hfIndex / sizeJ;
//			int		j 		= hfIndex % sizeJ;
//			float	y 		= m_y0 + i * XYSCALE;
//			float	x 		= m_x0 + j * XYSCALE;
//
//			VBEntry * const	pEntry	= &paVertexEntries[ k ];	// Convenience
//
//			pEntry->position.x		= x; 
//			pEntry->position.y		= y;
//			pEntry->position.z		= m_pHeightField->GetData( j, i )->m_Z;
//			pEntry->u				= j * uvScaleJ + uvOffsetJ;
//			pEntry->v				= i * uvScaleI + uvOffsetI;
//		}
//	}
//
//	m_pVB = pVB;
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


