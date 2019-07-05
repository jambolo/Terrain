/** @file *//********************************************************************************************************

                                                    RoamTerrain.h

						                    Copyright 2006, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Terrain/Roam/RoamTerrain.h#1 $

	$NoKeywords: $

 ********************************************************************************************************************/

#pragma once

#include "RoamTree/RoamTree.h"

class DataTree;

/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

class RoamTerrain
{
public:

	RoamTerrain( DataTree const * pLowerLeftData,
				 DataTree const * pUpperRightData,
				 RoamTree::SplitParameters const & splitParameters );
	virtual ~RoamTerrain();

	RoamTree *	m_pLowerLeft;	// RoamTree in the lower left corner
	RoamTree *	m_pUpperRight;	// RoamTree in the upper right corner
};
