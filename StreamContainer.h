// Copyright (C) 2016 SuperUnderDog
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#pragma once
#include <iostream>
#include <fstream>


class CBaseOutStream
{
protected:
	CBaseOutStream( void ) {}
public:
	virtual ~CBaseOutStream() {}

	virtual void close( void ) = 0;
	virtual int GetCrSize( void ) const = 0;
	virtual std::streampos GetOutCount( void ) const = 0;
	virtual void WriteLine( char const * const pccSrc ) = 0;
};

class CCoutStream : public CBaseOutStream
{
public:
	CCoutStream( std::ostream & ros ) :
		m_ros( ros ), m_liOut( 0 )
	{}
	virtual ~CCoutStream() {}

private:
	std::ostream & m_ros;
	std::streampos m_liOut;

public:
	virtual void close( void ) {}
	virtual int GetCrSize( void ) const { return 2; }
	virtual std::streampos GetOutCount( void ) const { return m_liOut; }
	virtual void WriteLine( char const * const pccSrc )
	{
		std::cout << pccSrc << _T( "\n" );
		m_liOut += ::_tcslen( pccSrc ) + 2;
	}
};

class COfstream : public CBaseOutStream
{
public:
	COfstream( std::ofstream & rofs ) :
		m_rofs( rofs )
	{}
	virtual ~COfstream()
	{
		m_rofs.close();
	}

private:
	std::ofstream & m_rofs;

public:
	virtual void close( void ) { m_rofs.close(); }
	virtual int GetCrSize( void ) const { return 1; }
	virtual std::streampos GetOutCount( void ) const { return m_rofs.tellp(); }
	virtual void WriteLine( char const * const pccSrc )
	{
		int const iLen = ::_tcslen( pccSrc );
		m_rofs.write( pccSrc, iLen );
		m_rofs.put( _T( '\n' ) );
	}
};


class CBaseInStream
{
protected:
	CBaseInStream( void ) {}
public:
	virtual ~CBaseInStream() {}

	virtual void close( void ) = 0;
	virtual bool fail( void ) const = 0;
	virtual bool eof( void ) const = 0;
	virtual void clear( void ) = 0;
	virtual void seekg( std::streampos const pos ) = 0;
	virtual void getline( char * const, std::streamsize const ) = 0;
};

class CCinStream : public CBaseInStream
{
public:
	CCinStream( std::istream & ris ) :
		m_ris( ris )
	{}
	virtual ~CCinStream() {}

private:
	std::istream & m_ris;

public:
	virtual void close( void ) {}
	virtual bool fail( void ) const { return m_ris.fail(); }
	virtual bool eof( void ) const { return m_ris.eof(); }
	virtual void clear( void ) { m_ris.clear(); }
	virtual void seekg( std::streampos const pos ) { m_ris.seekg( pos ); }
	virtual void getline( char * const pcDest, std::streamsize const size )
	{
		m_ris.getline( pcDest, size );
	}
};

class CIfstream : public CBaseInStream
{
public:
	CIfstream( std::ifstream & rifs ) :
		m_rifs( rifs )
	{}
	virtual ~CIfstream()
	{
		m_rifs.close();
	}

private:
	std::ifstream & m_rifs;

public:
	virtual void close( void ) { m_rifs.close(); }
	virtual bool fail( void ) const { return m_rifs.fail(); }
	virtual bool eof( void ) const { return m_rifs.eof(); }
	virtual void clear( void ) { m_rifs.clear(); }
	virtual void seekg( std::streampos const pos ) { m_rifs.seekg( pos ); }
	virtual void getline( char * const pcDest, std::streamsize const size )
	{
		m_rifs.getline( pcDest, size );
	}
};


