/** @file *//********************************************************************************************************

                                                   GridTriangle.cpp

						                    Copyright 2003, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Terrain/Roam/RoamTree/GridTriangle.cpp#2 $

	$NoKeywords: $

 ********************************************************************************************************************/

#include "GridTriangle.h"

#include <cassert>

/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

void Top( int *pX, int * pY ) const
{
	*pX = m_x;
	*pY = m_y;
}

/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

void Left( int *pX, int * pY ) const
{
	int const	size2	= m_size / 2;

	switch( m_orientation )
	{
	case O_UP:
		*pX = m_x + size2;
		*pY = m_y + size2;
		break;

	case O_UPRIGHT:
		*pX = m_x + size;
		*pY = m_y + 0;
		break;

	case O_RIGHT:
		*pX = m_x + size2;
		*pY = m_y - size2;
		break;

	case O_DOWNRIGHT:
		*pX = m_x + 0;
		*pY = m_y - size;
		break;

	case O_DOWN:
		*pX = m_x - size2;
		*pY = m_y - size2;
		break;

	case O_DOWNLEFT:
		*pX = m_x - size;
		*pY = m_y + 0;
		break;
	case O_LEFT:
		*pX = m_x - size2;
		*pY = m_y + size2;
		break;

	case O_UPLEFT:
		*pX = m_x + 0;
		*pY = m_y + size;
		break;
	}

}

/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

void Right( int *pX, int * pY ) const
{
	int const	size2	= m_size / 2;

	switch( m_orientation )
	{
	case O_UP:
		*pX = m_x - size2;
		*pY = m_y + size2;
		break;

	case O_UPRIGHT:
		*pX = m_x + 0;
		*pY = m_y + size;
		break;

	case O_RIGHT:
		*pX = m_x + size2;
		*pY = m_y + size2;
		break;

	case O_DOWNRIGHT:
		*pX = m_x + size;
		*pY = m_y + 0;
		break;

	case O_DOWN:
		*pX = m_x + size2;
		*pY = m_y - size2;
		break;

	case O_DOWNLEFT:
		*pX = m_x + 0;
		*pY = m_y - size;
		break;

	case O_LEFT:
		*pX = m_x - size2;
		*pY = m_y - size2;
		break;

	case O_UPLEFT:
		*pX = m_x - size;
		*pY = m_y + 0;
		break;
	}
}

/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

void GridTriangle::ChildTop( int *pChildX, int * pChildY ) const
{
	// Fill in the vertex pointers

	int const	size2	= m_size / 2;

	switch( m_orientation )
	{
	case O_UP:
		*pChildX = m_x + 0;
		*pChildY = m_y + size2;
		break;

	case O_UPRIGHT:
		*pChildX = m_x + size2;
		*pChildY = m_y + size2;
		break;

	case O_RIGHT:
		*pChildX = m_x + size2;
		*pChildY = m_y + 0;
		break;

	case O_DOWNRIGHT:
		*pChildX = m_x + size2;
		*pChildY = m_y - size2;
		break;

	case O_DOWN:
		*pChildX = m_x + 0;
		*pChildY = m_y - size2;
		break;

	case O_DOWNLEFT:
		*pChildX = m_x - size2;
		*pChildY = m_y - size2;
		break;

	case O_LEFT:
		*pChildX = m_x - size2;
		*pChildY = m_y + 0;
		break;

	case O_UPLEFT:
		*pChildX = m_x - size2;
		*pChildY = m_y + size2;
		break;
	}
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

int GridTriangle::ChildSize() const
{
	// Assumes that the enumerators for diagonally facing triangles are even
	assert( O_UPRIGHT == 0 );

	return ( m_orientation & 1 ) ? m_size / 2 : m_size;
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

GridTriangle::Orientation GridTriangle::LeftChildOrientation() const
{
	// Assume that the order of orientation enumerators is clockwise starting at O_UPRIGHT
	assert( GridTriangle::O_UPRIGHT == 0 );

	static Orientation const	leftChildOrientationMap[] =
	{
		O_DOWN,
		O_DOWNLEFT,
		O_LEFT,
		O_UPLEFT,
		O_UP,
		O_UPRIGHT,
		O_RIGHT,
		O_DOWNRIGHT,
	};

	return leftChildOrientationMap[ m_orientation ];
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

GridTriangle::Orientation GridTriangle::RightChildOrientation() const
{
	// Assume that the order of orientation enumerators is clockwise starting at O_UPRIGHT
	assert( O_UPRIGHT == 0 );

	static Orientation const	rightChildOrientationMap[] =
	{
		O_LEFT,
		O_UPLEFT,
		O_UP,
		O_UPRIGHT,
		O_RIGHT,
		O_DOWNRIGHT,
		O_DOWN,
		O_DOWNLEFT,
	};

	return rightChildOrientationMap[ m_orientation ];
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

GridTriangle GridTriangle::LeftChild() const
{
	int topX, topY;
	ChildTop( &topX, &topY );

	return GridTriangle( topX, topY, LeftChildOrientation(), ChildSize() );
}


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

GridTriangle GridTriangle::RightChild() const
{
	int topX, topY;
	ChildTop( &topX, &topY );

	return GridTriangle( topX, topY, RightChildOrientation(), ChildSize() );
}
