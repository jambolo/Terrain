/** @file *//********************************************************************************************************

                                                      DataTree.h

						                    Copyright 2003, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Terrain/Roam/RoamTree/DataTree.h#2 $

	$NoKeywords: $

 ********************************************************************************************************************/

#pragma once

#include "GridTriangle.h"
#include <BinaryTree/BinaryTree.h>
#include <memory.h>
#include <d3dx9.h>
#include <boost/noncopyable.hpp>



/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

class DataTree : boost::noncopyable
{
public:

	struct Data
	{
		void	*		pV0;
		void	*		pV1;
		void	*		pV2;
		D3DXVECTOR3		normal;
		float			maxVariance;
		GridTriangle	gt;
	};

	typedef BinaryTree<Data>::Node	Node;

	//! Constructor
	DataTree( GridTriangle const & gt, void * paVertexGrid, int size );

	void *				m_paVertexGrid;
	int					m_size;
	BinaryTree<Data>	m_tree;
};


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

