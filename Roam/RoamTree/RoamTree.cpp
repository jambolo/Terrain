/** @file *//********************************************************************************************************

                                                     RoamTree.cpp

						                    Copyright 2003, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Terrain/Roam/RoamTree/RoamTree.cpp#2 $

	$NoKeywords: $

 ********************************************************************************************************************/

#include "RoamTree.h"

#include "DataTree.h"
#include "Misc/Max.h"

#include <xutility>
#include <memory>


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

RoamTree::RoamTree()
{
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

RoamTree::RoamTree( DataTree const * pData, SplitParameters const & splitParameters )
	: m_pData( pData ),
	m_splitParameters( splitParameters ),
	m_tree( Triangle( pData->m_tree.Root() ) )
{
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

RoamTree::~RoamTree()
{
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

void RoamTree::MeetTheNeighbors( RoamTree * pBottomNeighbor,
								 RoamTree * pLeftNeighbor,
								 RoamTree * pRightNeighbor )
{
	m_tree.Root()->m_data.MeetTheNeighbors( pBottomNeighbor ? pBottomNeighbor->m_tree.Root() : 0,
											pLeftNeighbor ?   pLeftNeighbor->m_tree.Root()   : 0,
											pRightNeighbor ?  pRightNeighbor->m_tree.Root()  : 0 );
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

void RoamTree::Tesselate()
{
	TesselateTree( m_tree.Root() );
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

void RoamTree::TesselateTree( Node * pNode )
{
	// If this node meets the criteria for splitting, then split it

	DataTree::Node const *	pData	= pNode->m_data.GetData();
	if ( pNode->IsALeaf() && !pData->IsALeaf() && MeetsTheSplitCriteria( *pData ) )
	{
		SplitNode( pNode );
	}

	// Now tesselate its children if there are any

	if ( !pNode->IsALeaf() )
	{
		TesselateTree( pNode->Left() );
		TesselateTree( pNode->Right() );
	}
}



/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

void RoamTree::SplitNode( Node * pNode )
{
	assert( pNode->IsALeaf() );
	assert( !pNode->m_data.GetData()->IsALeaf() );

	Node *	pBN	= pNode->m_data.BottomNeighbor();

	// If this node has a bottom neighbor, the bottom neighbor must be split to match this node's split.

	if ( pBN )
	{
		// If the the bottom neighbor is split less than this node, then it needs to be split until it matches, so
		// that the following split will match this node's split. It turns out that only a single additional split
		// is ever necessary, though a chain of splits will be triggered.

		if ( pBN->m_data.BottomNeighbor() != pNode )
		{
			SplitNode( pBN );

			// pNode's bottom neighbor has changed because it was split!

			pBN	= pNode->m_data.BottomNeighbor();
		}

		// Split the bottom node.

		m_data.Grow( pBN, Triangle( pBN->m_data.GetData()->Left() ), Triangle( pBN->m_data.GetData()->Right() ) );
	}

	// Now split this node.

	m_data.Grow( pNode, Triangle( pNode->m_data.GetData()->Left() ), Triangle( pNode->m_data.GetData()->Right() ) );

	// Fix up the neighborhood

	if ( pBN )
	{
		pBN->m_data.FixUpTheNeighborhoodAfterSplit();
	}

	pNode->m_data.FixUpTheNeighborhoodAfterSplit();
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

bool RoamTree::MeetsTheSplitCriteria( DataTree::Data const & data )
{
	D3DXVECTOR3	vd0	= data.pV0->m_Position - m_splitParameters.cameraPosition;

	// If the triangle is facing the other way, then it can't be seen and will be culled, so there is no need to
	// split it.
	
	if ( D3DXVec3Dot( &vd0, &data.normal ) > 0.f )
	{
		return false;
	}

	D3DXVECTOR3	vd1	= data.pV1->position - m_SplitParameters.m_CameraPosition;
	D3DXVECTOR3	vd2	= data.pV2->position - m_SplitParameters.m_CameraPosition;

	// If all the vertices are behind the camera, then it can't be seen and will
	// be culled, so there is no need to split it. A more precise test would compare
	// against the view frustum (...someday).

	if ( Dot( vd0, m_SplitParameters.m_CameraDirection ) <= 0.f &&
		 Dot( vd1, m_SplitParameters.m_CameraDirection ) <= 0.f &&
		 Dot( vd2, m_SplitParameters.m_CameraDirection ) <= 0.f )
	{
		return false;
	}

	// If the data is far enough away so that the max variance doesn't make a
	// difference, then there is no need to split it.

	float const	d0			= vd0.Length();
	float const	d1			= vd1.Length();
	float const	d2			= vd2.Length();
	float const distance	= std::min( d0, std::min( d1, d2 ) );

	if ( data.m_MaxVariance < distance * m_SplitParameters.m_Tolerance )
	{
		return false;
	}

	return true;
}



/********************************************************************************************************************/
/********************************************************************************************************************/
/*																													*/
/*													RoamTree::Node													*/
/*																													*/
/********************************************************************************************************************/
/********************************************************************************************************************/


/********************************************************************************************************************/
/*																													*/
/*																													*/
/********************************************************************************************************************/

RoamTree::Node::Node( Node * pParent, DataTree::Node const * pData, Node * pLeft/* = 0*/, Node * pRight/* = 0*/ )
	: BinaryTree::Node( pParent, pLeft, pRight ),
	m_pBottomNeighbor( 0 ),
	m_pLeftNeighbor( 0 ),
	m_pRightNeighbor( 0 ),
	m_pData( pData )
{
}


/********************************************************************************************************************/
/*																													*/
/*																													*/
/********************************************************************************************************************/

RoamTree::Node::~Node()
{
}


/********************************************************************************************************************/
/*																													*/
/*																													*/
/********************************************************************************************************************/

void RoamTree::Node::MeetTheNeighbors( Node * pBottomNeighbor, Node * pLeftNeighbor, Node * pRightNeighbor )
{
	m_pBottomNeighbor	= pBottomNeighbor;
	m_pLeftNeighbor		= pLeftNeighbor;
	m_pRightNeighbor	= pRightNeighbor;
}


/********************************************************************************************************************/
/*																													*/
/*																													*/
/********************************************************************************************************************/

void RoamTree::Node::FixUpTheNeighborhoodAfterSplit()
{
	// Note: the bottom neighbor is always split also. It must be split before
	// this node's neighbors are determined.

	// Fix the left child's neighborhood

	LeftChild()->MeetTheNeighbors( m_pLeftNeighbor,
								   RightChild(),
								   m_pBottomNeighbor ? m_pBottomNeighbor->RightChild() : 0 );

	// Fix the right child's neighborhood

	RightChild()->MeetTheNeighbors( m_pRightNeighbor,
								    m_pBottomNeighbor ? m_pBottomNeighbor->LeftChild() : 0,
								    LeftChild() );

	// Fix the left neighbor

	if ( m_pLeftNeighbor )
	{
		if ( m_pLeftNeighbor->BottomNeighbor() == this )
		{
			m_pLeftNeighbor->SetBottomNeighbor( LeftChild() );
		}
		else if ( m_pLeftNeighbor->LeftNeighbor() == this )
		{
			m_pLeftNeighbor->SetLeftNeighbor( LeftChild() );
		}
		else
		{
			m_pLeftNeighbor->SetRightNeighbor( LeftChild() );
		}
	}

	// Fix the right neighbor

	if ( m_pRightNeighbor )
	{
		if ( m_pRightNeighbor->BottomNeighbor() == this )
		{
			m_pRightNeighbor->SetBottomNeighbor( RightChild() );
		}
		else if ( m_pRightNeighbor->LeftNeighbor() == this )
		{
			m_pRightNeighbor->SetLeftNeighbor( RightChild() );
		}
		else
		{
			m_pRightNeighbor->SetRightNeighbor( RightChild() );
		}
	}

	// Note: the bottom neighbor is always split also. It must be split before
	// this node's neighbors are determined.

	// Note: the bottom neighbor is not fixed up because this function
	// comes right after a split. The bottom neighbor is always split also,
	// so it will fix itself.
}
