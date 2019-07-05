/** @file *//********************************************************************************************************

                                                        Hud.h

						                    Copyright 2003, John J. Bolton
	--------------------------------------------------------------------------------------------------------------

	$Header: //depot/Terrain/Common/Hud/Hud.h#2 $

	$NoKeywords: $

 ********************************************************************************************************************/

#pragma once

#include <Time/FrameRateCalculator.h>
#include <sstream>
#include <map>

struct IDirect3DDevice9;
struct ID3DXFont;


/********************************************************************************************************************/
/*																													*/
/********************************************************************************************************************/

class Hud
{
private:

	class IConvertibleToString
	{
	public:
		virtual std::string	ToString() const	= 0;
	};

	template < typename T > class DisplayedValue : public IConvertibleToString
	{
	public:

		DisplayedValue( T * pValue ) : m_pValue( pValue )
		{
		}

		std::string ToString() const
		{
			std::ostringstream	buffer;
			buffer << *m_pValue;
			return buffer.str();
		}

	private:
		T *	m_pValue;
	};

public:

	Hud( IDirect3DDevice9 * pD3dDevice );
	~Hud();

	void Update( __int64 t );

	void Draw();

	void DisplayConsole( bool turnOn );
	bool ConsoleIsDisplayed() const		{ return m_bDisplayConsole; }

	template< typename T >
	void DisplayValue( char const * sName, T const * pValue )
	{
		DisplayedValue<T> *	pDisplayedValue	= new DisplayedValue<T>( pValue );
		m_displayedValues.insert( DisplayedValueMap::value_type( sName, pDisplayedValue ) );
	}

	void RemoveDisplayedValue( char const * sName );

private:

	typedef std::map< std::string, IConvertibleToString* >	DisplayedValueMap;

	void InitializeFonts( IDirect3DDevice9 * pD3dDevice );
	void ShutDownFonts();

	IDirect3DDevice9 *		m_pD3dDevice;
	FrameRateCalculator		m_frameRateCalculator;
	ID3DXFont *				m_pFont;
	bool					m_bDisplayConsole;
	DisplayedValueMap		m_displayedValues;
};
