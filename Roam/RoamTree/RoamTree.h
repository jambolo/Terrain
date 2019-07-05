/** @file *//********************************************************************************************************

                                                      RoamTree.h

						                    Copyright 2006, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Terrain/Roam/RoamTree/RoamTree.h#2 $

	$NoKeywords: $

 ********************************************************************************************************************/

#pragma once

#include "DataTree.h"
#include <BinaryTree/BinaryTree.h>

#include <boost/noncopyable.hpp>
#include <d3dx9.h>

class GridTriangle;


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

//! A ROAM tree class.

class RoamTree : boost::noncopyable
{
public:

	class Triangle;

	typedef BinaryTree<Triangle>::Node	Node;

	struct SplitParameters
	{
		SplitParameters() {}
		SplitParameters( D3DXVECTOR3 const & position, D3DXVECTOR3 const & direction, float fov, float t )
			: cameraPosition( position ),
			cameraDirection( direction ),
			cameraFov( fov ),
			tolerance( t )
		{
		}

		D3DXVECTOR3		cameraPosition;
		D3DXVECTOR3		cameraDirection;
		float			cameraFov;
		float			tolerance;
	};

	//! Constructor
	RoamTree();

	//! Constructor
	RoamTree( DataTree const * pData, SplitParameters const & splitParameters );

	//! Destructor
	virtual ~RoamTree();

	//! Assigns the neighbors for this tree
	void MeetTheNeighbors( RoamTree * m_pBottomNeighbor, RoamTree * pLeftNeighbor, RoamTree * pRightNeighbor );

	//! Tesselates the tree
	void Tesselate();

private:

	// Splits a node
	void SplitNode( Node * pNode );

	// Tesselates the tree rooted at this node
	void TesselateTree( Node * pNode );

	// Returns true if the node should be split
	bool MeetsTheSplitCriteria( DataTree::Node const & data );

	SplitParameters const		m_splitParameters;		// Parameters for determining if a node should be split or not
	DataTree const *			m_pData;				// All triangle data
	BinaryTree<Triangle>		m_tree;					// The actual tree
};



/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

//! A ROAM tree node.
class RoamTree::Triangle
{
public:

	//! Constructor
	Triangle( DataTree::Node const * pData );

	//! Destructor
	virtual ~Triangle();

	//! Sets this node's neighbors
	void MeetTheNeighbors( RoamTree::Node * pBottomNeighbor,
						   RoamTree::Node * pLeftNeighbor,
						   RoamTree::Node * pRightNeighbor );

	//! Sets the bottom neighbor node.
	void SetBottomNeighbor( RoamTree::Node * pBottomNeighbor );

	//! Returns the bottom neighbor node.
	RoamTree::Node * BottomNeighbor();

	//! Returns the bottom neighbor node.
	RoamTree::Node const * BottomNeighbor() const;

	//! Sets the left neighbor node.
	void SetLeftNeighbor( RoamTree::Node * pLeftNeighbor );

	//! Returns the left neighbor node.
	RoamTree::Node * LeftNeighbor();

	//! Returns the left neighbor node.
	RoamTree::Node const * LeftNeighbor() const;

	//! Sets the right neighbor node.
	void SetRightNeighbor( RoamTree::Node * pRightNeighbor );

	//! Returns the right neighbor node.
	RoamTree::Node * RightNeighbor();

	//! Returns the right neighbor node.
	RoamTree::Node const * RightNeighbor() const;

	//! Returns the data for this node
	DataTree::Node const *	GetData() const;

	//! Returns the GridTriangle representation of this node.
	GridTriangle const & GetGridTriangle() const;

	//! Returns the normal
	D3DXVECTOR3 const & GetNormal() const;

	//! Returns the maximum variance allowed before splitting
	float GetMaxVariance() const;

	//! Returns vertex 0
	void const * GetV0() const;

	//! Returns vertex 1
	void const * GetV1() const;

	//! Returns vertex 2
	void const * GetV2() const;

	//! (Re-)Assigns all the neighbors in the neighborhood after this node is split
	void FixUpTheNeighborhoodAfterSplit();

private:

	RoamTree::Node *		m_pBottomNeighbor;			// The bottom neighbor
	RoamTree::Node *		m_pLeftNeighbor;			// The left neighbor
	RoamTree::Node *		m_pRightNeighbor;			// The right neighbor
	uint32					v0;
	uint32					v1;
	uint32					v2;
//	D3DXVECTOR3				m_normal;
	float					m_maxVariance;
	bool					m_bSplit;
	GridTriangle			m_gt;
};


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

// inline functions

#include "RoamTree.inl"
