/** @file *//********************************************************************************************************

                                                    GridTriangle.h

						                    Copyright 2003, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Terrain/Roam/RoamTree/GridTriangle.h#2 $

	$NoKeywords: $

 ********************************************************************************************************************/

#pragma once


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

class GridTriangle
{
public:
	// The orientation specifies which way a triangle faces ("up" signifies
	// increasing y and "right" signifies increasing x)

	enum Orientation
	{
		O_UPRIGHT,
		O_RIGHT,
		O_DOWNRIGHT,
		O_DOWN,
		O_DOWNLEFT,
		O_LEFT,
		O_UPLEFT,
		O_UP
	};

	GridTriangle() {};
	GridTriangle( int x, int y, Orientation orientation, int size );

	// Returns the top vertex
	void Top( int *x, int * y ) const;

	// Returns the left vertex
	void Left( int *x, int * y ) const;

	// Returns the right vertex
	void Right( int *x, int * y ) const;

	// Returns the top vertexes of the children (both children have the same top vertex)
	void ChildTop( int *pChildX, int * pChildY ) const;

	// Returns the size of a split triangle's child
	int ChildSize() const;

	// Returns the orientation of the left child
	Orientation RightChildOrientation() const;

	// Returns the orientation of the right child
	Orientation LeftChildOrientation() const;

	// Returns a GridTriangle representing the left child
	GridTriangle LeftChild() const;

	// Returns a GridTriangle representing the right child
	GridTriangle RightChild() const;

	int			m_x, m_y;
	int			m_size;
	Orientation	m_orientation;
};


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

inline GridTriangle::GridTriangle( int x, int y, Orientation orientation, int size )
	: m_x( x ), m_y( y ), m_orientation( orientation ), m_size( size )
{
}
