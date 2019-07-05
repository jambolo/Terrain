/** @file *//********************************************************************************************************

                                                 BinaryTriangleTree.h

						                    Copyright 2003, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Terrain/Roam/RoamTree/BinaryTriangleTree.h#2 $

	$NoKeywords: $

 ********************************************************************************************************************/

#pragma once

#include <BinaryTree/BinaryTree.h>

/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

struct Triangle
{
	// Vertices are CCW, starting with the "top"
	void const *	m_pV0;
	void const *	m_pV1;
	void const *	m_pV2;
};


typedef BinaryTree<Triangle>	BinaryTriangleTree; 
