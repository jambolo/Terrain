/********************************************************************************************************************

                                                      Patch.cpp

						                    Copyright 2006, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Terrain/Simple/Patch.cpp#7 $

	$NoKeywords: $

 ********************************************************************************************************************/

#include "PrecompiledHeaders.h"

#include "Patch.h"

#include "Console/Console.h"
#include "Dxx/Dxx.h"
#include "HeightField/HeightField.h"
#include "Misc/Assert.h"
#include "Wx/Wx.h"

using namespace std;


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

namespace
{
	struct VBEntry
	{
		D3DVECTOR	position;	// vertex position
		float		u, v;		// base texture
	};

	UINT32 const	FVF	= D3DFVF_XYZ | D3DFVF_TEX1;

} // anonymous namespace


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

Patch::DebugSettings	Patch::m_debugSettings			= { false };
IDirect3DIndexBuffer9 *	Patch::m_pIB;

/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

Patch::Patch( IDirect3DDevice9 * pD3dDevice,
			  HeightField const & hf, int i0, int j0,
			  float x0, float y0, float xyScale,
			  float u0, float v0, float uvScale )
	: m_pD3dDevice( pD3dDevice )
{
	m_pD3dDevice->AddRef();

	HRESULT		hr;

	// The patch is drawn as a single indexed triangle strip

	int nIndexes	= ( SIZE - 1 ) * ( 2 * SIZE + 2 ) - 2;
	int nVertexes	= SIZE * SIZE;

	m_drawParameters.nPrimitives	= nIndexes - 2;		// A single triangle strip
	m_drawParameters.nVertexes		= nVertexes;

	// If the index buffer has not been created yet then create it now. Otherwise, just bump the reference count

	if ( m_pIB == 0 )
	{
		hr = pD3dDevice->CreateIndexBuffer( nIndexes * sizeof( __int16 ),
											D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED,
											&m_pIB, NULL );
		assert_succeeded( hr );

		if ( SUCCEEDED( hr ) )
		{
			Dxx::IndexBufferLock	lock( m_pIB );

			uint16 *	pIndexes	= static_cast< uint16 * >( lock.GetLockedBuffer() );

			// Strip the grid to get the order of the vertexes

			int	n;

			n = Dxx::StripGrid( SIZE, SIZE, pIndexes );
			assert( n == nIndexes );	// Make sure the number of vertexes is right (if it hasn't already crashed)

		}
	}
	else
	{
		m_pIB->AddRef();
	}

	// Create the vertex buffer

	hr = m_pD3dDevice->CreateVertexBuffer( nVertexes * sizeof( VBEntry ),
										   D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED,
										   &m_pVB, NULL );
	assert_succeeded( hr );

	if ( SUCCEEDED( hr ) )
	{
		// Initialize it

		Dxx::VertexBufferLock	lock( m_pVB );

		VBEntry *	paVertexEntries	= static_cast< VBEntry* >( lock.GetLockedBuffer() );

		for ( int i = 0; i < SIZE; ++i )
		{
			for ( int j = 0; j < SIZE; ++j )
			{
				VBEntry * const	pEntry	= &paVertexEntries[ i * SIZE + j ];	// Convenience

				pEntry->position.x		= x0 + j * xyScale; 
				pEntry->position.y		= y0 + i * xyScale;
				pEntry->position.z		= hf.GetData( j0 + j, i0 + i )->m_Z;
				pEntry->u				= u0 + j * uvScale;
				pEntry->v				= v0 + i * uvScale;
			}
		}
	}
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

Patch::~Patch()
{
	Wx::SafeRelease( m_pVB );
	if ( m_pIB != 0 ) m_pIB->Release();
	Wx::SafeRelease( m_pD3dDevice );
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

void Patch::Draw()
{
	HRESULT	hr;

	m_performanceInfo.m_triangleCount = 0;

	// Draw the terrain

	hr = m_pD3dDevice->SetStreamSource( 0, m_pVB, 0, sizeof( VBEntry ) );
	assert_succeeded( hr );

	hr = m_pD3dDevice->SetIndices( m_pIB );
	assert_succeeded( hr );

	hr = m_pD3dDevice->SetFVF( FVF );
	assert_succeeded( hr );

	// Draw the patch

	hr = m_pD3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP,
												0,
												0,
												m_drawParameters.nVertexes,
												0,
												m_drawParameters.nPrimitives );
	assert_succeeded( hr );

	m_performanceInfo.m_triangleCount += m_drawParameters.nPrimitives;

	// Draw the bounding box

	if ( m_debugSettings.bDrawBoundingBox )
	{
	}
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

void Patch::AddConsoleVariables()
{
	Console::Instance().AddVariable( "boundingbox", &m_debugSettings.bDrawBoundingBox );
}
