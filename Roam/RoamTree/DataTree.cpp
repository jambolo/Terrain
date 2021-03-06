/** @file *//********************************************************************************************************

                                                     DataTree.cpp

						                    Copyright 2003, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Terrain/Roam/RoamTree/DataTree.cpp#2 $

	$NoKeywords: $

 ********************************************************************************************************************/

#include "DataTree.h"

#include "GridTriangle.h"

/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

DataTree::DataTree( GridTriangle const & gt, void * paVertexGrid, int size )
: m_paVertexGrid( paVertexGrid ), m_size( size )
{
}
