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

#include "stdafx.h"
#include <set>

#include "StreamContainer.h"
#include "GenWordDict.h"


static inline bool HasAlphabet( TCHAR const * const pctsz )
{
	bool bFound = false;
	for( int i = 0; ( !bFound ) && ( '\0' != pctsz[ i ] ); i++ )
	{
		TCHAR const tc = pctsz[ i ];
		if( ( ( 'A' <= tc ) && ( 'Z' >= tc ) ) ||
			( ( 'a' <= tc ) && ( 'z' >= tc ) ) )
		{
			bFound = true;
		}
	}
	return bFound;
}

static inline bool IsValidPattern( int const iValue, int const iCol )
{
	bool bFoundFalse = false;
	bool bPrev = ( 0 != ( iValue & 1 ) );
	for( int i = 1; ( !bFoundFalse ) && ( iCol > i ); i++ )
	{
		bool const bCurrent = ( 0 != ( ( iValue >> i ) & 1 ) );
		if( ( !bPrev ) && ( !bCurrent ) )
		{
			bFoundFalse = true;
		}
		bPrev = bCurrent;
	}
	return ( !bFoundFalse );
}

static inline int CountTrue( int const iValue, int const iCol )
{
	int iFoundCount = 0;
	for( int i = 0; iCol > i; i++ )
	{
		if( 0 != ( ( iValue >> i ) & 1 ) )
		{
			iFoundCount++;
		}
	}
	return iFoundCount;
}

static inline int FindFisrtOfLowerAlphabet( TCHAR const * const pctsz )
{
	int iFound = INT_MIN;
	for( int i = 0; ( INT_MIN >= iFound ) && ( '\0' != pctsz[ i ] ); i++ )
	{
		TCHAR const tc = pctsz[ i ];
		if( ( 'a' <= tc ) && ( 'z' >= tc ) )
		{
			iFound = i;
		}
	}
	return iFound;
}
static inline int FindLastOfLowerAlphabet( TCHAR const * const pctsz )
{
	int iLastIdx = INT_MIN;
	for( int i = 0; '\0' != pctsz[ i ]; i++ )
	{
		TCHAR const tc = pctsz[ i ];
		if( ( 'a' <= tc ) && ( 'z' >= tc ) )
		{
			iLastIdx = i;
		}
	}
	return iLastIdx;
}

static inline std::string ToLower( TCHAR const * const pctsz )
{
	std::string str;
	for( int i = 0; '\0' != pctsz[ i ]; i++ )
	{
		TCHAR c = pctsz[ i ];
		if( ( 'A' <= c ) && ( 'Z' >= c ) ) c += 'a' - 'A';
		str.push_back( c );
	}
	return str;
}

static inline std::string ToUpper( TCHAR const * const pctsz )
{
	std::string str;
	for( int i = 0; '\0' != pctsz[ i ]; i++ )
	{
		TCHAR c = pctsz[ i ];
		if( ( 'a' <= c ) && ( 'z' >= c ) ) c -= 'a' - 'A';
		str.push_back( c );
	}
	return str;
}

static inline std::string ToInitialUpperFromLower( TCHAR const * const pctsz )
{
	std::string str = pctsz;
	int const iIdx = ::FindFisrtOfLowerAlphabet( pctsz );
	if( 0 <= iIdx ) str[ iIdx ] -= 'a' - 'A';
	return str;
}
static inline std::string ToLastUpperFromLower( TCHAR const * const pctsz )
{
	std::string str = pctsz;
	int const iIdx = ::FindLastOfLowerAlphabet( pctsz );
	if( 0 <= iIdx ) str[ iIdx ] -= 'a' - 'A';
	return str;
}
static inline std::string ToInitialAndLastUpperFromLower( TCHAR const * const pctsz )
{
	std::string str = pctsz;
	int const iFirstIdx = ::FindFisrtOfLowerAlphabet( pctsz );
	int const iLastIdx = ::FindLastOfLowerAlphabet( pctsz );
	if( 0 <= iFirstIdx ) str[ iFirstIdx ] -= 'a' - 'A';
	if( ( 0 <= iLastIdx ) && ( iFirstIdx != iLastIdx ) )
	{
		str[ iLastIdx ] -= 'a' - 'A';
	}
	return str;
}
#if 0
static inline std::string ToTrailUpperFromLower( TCHAR const * const pctsz )
{
	std::string str = pctsz;
	int const iIdx = ::FindFisrtOfLowerAlphabet( pctsz );
	if( 0 <= iIdx )
	{
		for( int i = iIdx + 1; str.size() > (std::size_t)i; i++ )
		{
			TCHAR const tc = str[ i ];
			if( ( 'a' <= tc ) && ( 'z' >= tc ) )
			{
				str[ i ] -= 'a' - 'A';
			}
		}
	}
	return str;
}
#endif


static inline void ReadAllLine(
	std::set<std::string> & rstrset,
	std::istream & ris, SApplicationParam const & rcparam )
{
	TCHAR * pctszLine = NULL;
	try
	{
		pctszLine = new TCHAR[ rcparam.iMaxLength + 1 ];
		pctszLine[ rcparam.iMaxLength ] = 0;

		while( !( ris.eof() ) )
		{
			ris.getline( pctszLine, rcparam.iMaxLength );
			if( ris.fail() )
			{
				while( ( !( ris.eof() ) ) && ris.fail() )
				{
					ris.clear();
					ris.getline( pctszLine, rcparam.iMaxLength );
				}
			}
			else if( '\0' != pctszLine[ 0 ] )
			{
				if( rstrset.end() == rstrset.find( pctszLine ) )
				{
					rstrset.insert( pctszLine );
				}
			}
		}
	}
	catch( ... )
	{
		if( NULL != pctszLine ) delete[] pctszLine;
		throw;
	}
}

static inline void WriteLine( CBaseOutStream * const pos,
	TCHAR const * const pctsz, SApplicationParam const & rcparam )
{
	int const iLen = ::_tcslen( pctsz );

	if( ( ( std::streampos )( rcparam.iMaxOutMega ) * 1024 * 1024 ) <
		( pos->GetOutCount() + ( std::streampos )( iLen + pos->GetCrSize() ) ) )
	{
		std::string str = _T( "Out size limit reached (stop at \"" );
		str += pctsz;
		str += _T( "\")." );
		throw std::exception( str.c_str() );
	}

	if( ( iLen >= rcparam.iMinLength ) && ( iLen <= rcparam.iMaxLength ) )
	{
		pos->WriteLine( pctsz );
	}
}


static inline void _PickupAlphaWord(
	CBaseOutStream * const pos, std::fstream & rfsAlpha,
	CBaseInStream * const pinstream, SApplicationParam const & rcparam )
{
	TCHAR * pctszLine = NULL;
	try
	{
		pctszLine = new TCHAR[ rcparam.iMaxLength + 1 ];
		pctszLine[ rcparam.iMaxLength ] = 0;

		while( !( pinstream->eof() ) )
		{
			pinstream->getline( pctszLine, rcparam.iMaxLength );
			if( pinstream->fail() )
			{
				while( ( !( pinstream->eof() ) ) && pinstream->fail() )
				{
					pinstream->clear();
					pinstream->getline( pctszLine, rcparam.iMaxLength );
				}
			}
			else if( '\0' != pctszLine[ 0 ] )
			{
				if( !HasAlphabet( pctszLine ) )
				{
					WriteLine( pos, pctszLine, rcparam );
				}
				else
				{
					rfsAlpha << pctszLine << _T( '\n' );
				}
			}
		}

		delete[] pctszLine;
	}
	catch( ... )
	{
		if( NULL != pctszLine ) delete[] pctszLine;
		throw;
	}
	rfsAlpha.clear();
	rfsAlpha.seekp( 0 );
	rfsAlpha.seekg( 0 );
	pinstream->clear();
	pinstream->seekg( 0 );
}


static inline void _GenerateQuatedDict_WriteWord(
	TCHAR const * const pctcsz,
	CBaseOutStream * const pos, SApplicationParam const & rcparam )
{
	std::string str;

	str = _T( "<" );
	str += pctcsz;
	str += _T( '>' );
	WriteLine( pos, str.c_str(), rcparam );

	str = _T( "(" );
	str += pctcsz;
	str += _T( ')' );
	WriteLine( pos, str.c_str(), rcparam );

	str = _T( "[" );
	str += pctcsz;
	str += _T( ']' );
	WriteLine( pos, str.c_str(), rcparam );

	str = _T( "{" );
	str += pctcsz;
	str += _T( '}' );
	WriteLine( pos, str.c_str(), rcparam );

	str = _T( "\"" );
	str += pctcsz;
	str += _T( '"' );
	WriteLine( pos, str.c_str(), rcparam );

	str = _T( "'" );
	str += pctcsz;
	str += _T( '\'' );
	WriteLine( pos, str.c_str(), rcparam );

	str = _T( "-" );
	str += pctcsz;
	str += _T( '-' );
	WriteLine( pos, str.c_str(), rcparam );

	str = _T( "+" );
	str += pctcsz;
	str += _T( '+' );
	WriteLine( pos, str.c_str(), rcparam );

	str = _T( "*" );
	str += pctcsz;
	str += _T( '*' );
	WriteLine( pos, str.c_str(), rcparam );

	str = _T( "_" );
	str += pctcsz;
	str += _T( '_' );
	WriteLine( pos, str.c_str(), rcparam );

	str = _T( "$" );
	str += pctcsz;
	str += _T( '$' );
	WriteLine( pos, str.c_str(), rcparam );

	str = _T( "%" );
	str += pctcsz;
	str += _T( '%' );
	WriteLine( pos, str.c_str(), rcparam );

	str = _T( "!" );
	str += pctcsz;
	str += _T( '!' );
	WriteLine( pos, str.c_str(), rcparam );

	str = _T( "?" );
	str += pctcsz;
	str += _T( '?' );
	WriteLine( pos, str.c_str(), rcparam );

	str = _T( "@" );
	str += pctcsz;
	str += _T( '@' );
	WriteLine( pos, str.c_str(), rcparam );
}

static inline void _GenerateQuatedDict_GenWords(
	TCHAR const * const pctcsz,
	CBaseOutStream * const pos, SApplicationParam const & rcparam )
{
	std::set<std::string> strset;
	_GenerateQuatedDict_WriteWord( pctcsz, pos, rcparam );
	strset.insert( pctcsz );

	std::string const strLower = ::ToLower( pctcsz );
	if( strset.end() == strset.find( strLower ) )
	{
		_GenerateQuatedDict_WriteWord( strLower.c_str(), pos, rcparam );
		strset.insert( strLower );
	}

	std::string str = ::ToUpper( pctcsz );
	if( strset.end() == strset.find( str ) )
	{
		_GenerateQuatedDict_WriteWord( str.c_str(), pos, rcparam );
		strset.insert( str );
	}

	str = ::ToInitialUpperFromLower( strLower.c_str() );
	if( strset.end() == strset.find( str ) )
	{
		_GenerateQuatedDict_WriteWord( str.c_str(), pos, rcparam );
		strset.insert( str );
	}

	str = ::ToLastUpperFromLower( strLower.c_str() );
	if( strset.end() == strset.find( str ) )
	{
		_GenerateQuatedDict_WriteWord( str.c_str(), pos, rcparam );
		strset.insert( str );
	}

	str = ::ToInitialAndLastUpperFromLower( strLower.c_str() );
	if( strset.end() == strset.find( str ) )
	{
		_GenerateQuatedDict_WriteWord( str.c_str(), pos, rcparam );
		strset.insert( str );
	}
#if 0
	str = ::ToTrailUpperFromLower( strLower.c_str() );
	if( strset.end() == strset.find( str ) )
	{
		_GenerateQuatedDict_WriteWord( str.c_str(), pos, rcparam );
		strset.insert( str );
	}
#endif
}

static inline void _GenerateQuatedDict(
	CBaseOutStream * const pos,
	SApplicationParam const & rcparam, CBaseInStream * const pinstream )
{
	TCHAR * ptcszLine = NULL;
	try
	{
		ptcszLine = new TCHAR[ rcparam.iMaxLength + 1 ];
		ptcszLine[ rcparam.iMaxLength ] = 0;

		while( !( pinstream->eof() ) )
		{
			pinstream->getline( ptcszLine, rcparam.iMaxLength );
			if( pinstream->fail() )
			{
				while( ( !( pinstream->eof() ) ) && pinstream->fail() )
				{
					pinstream->clear();
					pinstream->getline( ptcszLine, rcparam.iMaxLength );
				}
			}
			else if( '\0' != ptcszLine[ 0 ] )
			{
				_GenerateQuatedDict_GenWords( ptcszLine, pos, rcparam );
			}
		}

		delete[] ptcszLine;
	}
	catch( ... )
	{
		if( NULL != ptcszLine ) delete[] ptcszLine;
		throw;
	}
	pinstream->clear();
	pinstream->seekg( 0 );
}


struct _SContext
{
	SApplicationParam const & rcparam;
	CBaseOutStream * posDestDict;
	std::istream & risSrcDict;
	std::set<std::string> const & rcstrsetInsChar;
	std::set<std::string> const & rcstrsetPrefixChar;
	std::set<std::string> const & rcstrsetSurfixChar;

	_SContext(
		SApplicationParam const & rcparamArg,
		CBaseOutStream * posArgDestDict,
		std::istream & risArgSrcDict,
		std::set<std::string> const & rcstrsetArgInsChar,
		std::set<std::string> const & rcstrsetArgPrefixChar,
		std::set<std::string> const & rcstrsetArgSurfixChar ) :
		rcparam( rcparamArg ),
		posDestDict( posArgDestDict ),
		risSrcDict( risArgSrcDict ),
		rcstrsetInsChar( rcstrsetArgInsChar ),
		rcstrsetPrefixChar( rcstrsetArgPrefixChar ),
		rcstrsetSurfixChar( rcstrsetArgSurfixChar )
	{}
};


static void _Reenter_GenerateWord(
	TCHAR const * const pctcszLead, int const iReenterCount,
	int const iPattern, int const iCol, _SContext const & rctx );

static inline void _Reenter_GenerateWord_AppendChar(
	TCHAR const * const pctcszLead, int const iReenterCount,
	int const iPattern, int const iCol, _SContext const & rctx )
{
	bool const bSelectPrefix = (
		( 0 >= iReenterCount ) &&
		( !( rctx.rcparam.strPrefixCharFile.empty() ) ) );
	bool const bSelectSurfix = (
		( iCol <= ( 1 + iReenterCount ) ) &&
		( !( rctx.rcparam.strSurfixCharFile.empty() ) ) );
	std::set<std::string> const & rcstrsetChar =
		bSelectPrefix ? rctx.rcstrsetPrefixChar :
		( bSelectSurfix ? rctx.rcstrsetSurfixChar : rctx.rcstrsetInsChar );

	for( std::set<std::string>::const_iterator cit = rcstrsetChar.begin();
		rcstrsetChar.end() != cit; cit++ )
	{
		std::string str = pctcszLead;
		str += *cit;
		if( iCol <= ( iReenterCount + 1 ) )
		{
			WriteLine( rctx.posDestDict, str.c_str(), rctx.rcparam );
		}
		else
		{
			_Reenter_GenerateWord(
				str.c_str(), iReenterCount + 1, iPattern, iCol, rctx );
		}
	}
}

static inline void _Reenter_GenerateWord_WriteWord(
	TCHAR const * const pctcszWord,
	TCHAR const * const pctcszLead, int const iReenterCount,
	int const iPattern, int const iCol, _SContext const & rctx )
{
	std::string str = pctcszLead;
	str += pctcszWord;
	if( iCol <= ( iReenterCount + 1 ) )
	{
		WriteLine( rctx.posDestDict, str.c_str(), rctx.rcparam );
	}
	else
	{
		std::streamoff const soff = rctx.risSrcDict.tellg();
		_Reenter_GenerateWord(
			str.c_str(), iReenterCount + 1, iPattern, iCol, rctx );
		if( soff != rctx.risSrcDict.tellg() )
		{
			rctx.risSrcDict.seekg( soff );
		}
	}
}

static inline void _Reenter_GenerateWord_GenWords(
	TCHAR const * const pctcszAppend,
	TCHAR const * const pctcszLead, int const iReenterCount,
	int const iPattern, int const iCol, _SContext const & rctx )
{
	std::set<std::string> strset;
	_Reenter_GenerateWord_WriteWord(
		pctcszAppend, pctcszLead, iReenterCount, iPattern, iCol, rctx );
	strset.insert( pctcszAppend );

	std::string const strLower = ::ToLower( pctcszAppend );
	if( strset.end() == strset.find( strLower ) )
	{
		_Reenter_GenerateWord_WriteWord(
			strLower.c_str(), pctcszLead, iReenterCount, iPattern, iCol, rctx );
		strset.insert( strLower );
	}

	std::string str = ::ToUpper( pctcszAppend );
	if( strset.end() == strset.find( str ) )
	{
		_Reenter_GenerateWord_WriteWord(
			str.c_str(), pctcszLead, iReenterCount, iPattern, iCol, rctx );
		strset.insert( str );
	}

	str = ::ToInitialUpperFromLower( strLower.c_str() );
	if( strset.end() == strset.find( str ) )
	{
		_Reenter_GenerateWord_WriteWord(
			str.c_str(), pctcszLead, iReenterCount, iPattern, iCol, rctx );
		strset.insert( str );
	}

	str = ::ToLastUpperFromLower( strLower.c_str() );
	if( strset.end() == strset.find( str ) )
	{
		_Reenter_GenerateWord_WriteWord(
			str.c_str(), pctcszLead, iReenterCount, iPattern, iCol, rctx );
		strset.insert( str );
	}

	str = ::ToInitialAndLastUpperFromLower( strLower.c_str() );
	if( strset.end() == strset.find( str ) )
	{
		_Reenter_GenerateWord_WriteWord(
			str.c_str(), pctcszLead, iReenterCount, iPattern, iCol, rctx );
		strset.insert( str );
	}
#if 0
	str = ::ToTrailUpperFromLower( strLower.c_str() );
	if( strset.end() == strset.find( str ) )
	{
		_Reenter_GenerateWord_WriteWord(
			str.c_str(), pctcszLead, iReenterCount, iPattern, iCol, rctx );
		strset.insert( str );
	}
#endif
}

static inline void _Reenter_GenerateWord_AppendWord(
	TCHAR const * const pctcszLead, int const iReenterCount,
	int const iPattern, int const iCol, _SContext const & rctx )
{
	rctx.risSrcDict.seekg( 0 );
	TCHAR * ptcszLine = NULL;
	try
	{
		ptcszLine = new TCHAR[ rctx.rcparam.iMaxLength + 1 ];
		ptcszLine[ rctx.rcparam.iMaxLength ] = 0;

		while( !( rctx.risSrcDict.eof() ) )
		{
			rctx.risSrcDict.getline( ptcszLine, rctx.rcparam.iMaxLength );
			if( rctx.risSrcDict.fail() )
			{
				while( ( !( rctx.risSrcDict.eof() ) ) && rctx.risSrcDict.fail() )
				{
					rctx.risSrcDict.clear();
					rctx.risSrcDict.getline( ptcszLine, rctx.rcparam.iMaxLength );
				}
			}
			else if( '\0' != ptcszLine[ 0 ] )
			{
				_Reenter_GenerateWord_GenWords(
					ptcszLine, pctcszLead, iReenterCount, iPattern, iCol, rctx );
			}
		}
	}
	catch( ... )
	{
		if( NULL != ptcszLine ) delete[] ptcszLine;
		throw;
	}
	rctx.risSrcDict.clear();
	rctx.risSrcDict.seekg( 0 );
}

static void _Reenter_GenerateWord(
	TCHAR const * const pctcszLead, int const iReenterCount,
	int const iPattern, int const iCol, _SContext const & rctx )
{
	if( 0 == ( 1 & ( iPattern >> iReenterCount ) ) )
	{
		_Reenter_GenerateWord_AppendChar(
			pctcszLead, iReenterCount, iPattern, iCol, rctx );
	}
	else
	{
		_Reenter_GenerateWord_AppendWord(
			pctcszLead, iReenterCount, iPattern, iCol, rctx );
	}
}

static inline void _GenerateCombinedWord( _SContext const & rctx )
{
	int const iMaxCol = rctx.rcparam.iDictOccCount + rctx.rcparam.iCharOccCount;
	for( int iCol = 1; iMaxCol >= iCol; iCol++ )
	{
		for( int iPattern = 1; ( 1 << iCol ) > iPattern; iPattern++ )
		{
			if( IsValidPattern( iPattern, iCol ) &&
				( rctx.rcparam.iDictOccCount >= CountTrue( iPattern, iCol ) ) )
			{
				TCHAR const tcNull = _T( '\0' );
				_Reenter_GenerateWord( &tcNull, 0, iPattern, iCol, rctx );
			}
		}
	}
}

static inline void _GenerateCombinedDict(
	CBaseOutStream * const poutstream, CBaseInStream * const pinstream,
	SApplicationParam const & rcparam )
{
	std::set<std::string> strsetInsChar;
	std::set<std::string> strsetPrefixChar;
	std::set<std::string> strsetSurfixChar;
	std::ifstream ifsInsChar;
	std::ifstream ifsPrefixChar;
	std::ifstream ifsSurfixChar;
	try
	{
		ifsInsChar.open( rcparam.strInsCharFile, std::ios_base::in );
		if( ifsInsChar.fail() )
		{
			std::string str = _T( "Failed to open \"" );
			str += rcparam.strInsCharFile;
			str += _T( "\"." );
			throw std::exception( str.c_str() );
		}
		ReadAllLine( strsetInsChar, ifsInsChar, rcparam );

		if( !( rcparam.strPrefixCharFile.empty() ) )
		{
			ifsPrefixChar.open( rcparam.strPrefixCharFile, std::ios_base::in );
			if( ifsPrefixChar.fail() )
			{
				std::string str = _T( "Failed to open \"" );
				str += rcparam.strPrefixCharFile;
				str += _T( "\"." );
				throw std::exception( str.c_str() );
			}
		}
		ReadAllLine( strsetPrefixChar, ifsPrefixChar, rcparam );

		if( !( rcparam.strSurfixCharFile.empty() ) )
		{
			ifsSurfixChar.open( rcparam.strSurfixCharFile, std::ios_base::in );
			if( ifsSurfixChar.fail() )
			{
				std::string str = _T( "Failed to open \"" );
				str += rcparam.strSurfixCharFile;
				str += _T( "\"." );
				throw std::exception( str.c_str() );
			}
		}
		ReadAllLine( strsetSurfixChar, ifsSurfixChar, rcparam );

		ifsSurfixChar.close();
		ifsPrefixChar.close();
		ifsInsChar.close();
	}
	catch( ... )
	{
		ifsSurfixChar.close();
		ifsPrefixChar.close();
		ifsInsChar.close();
		throw;
	}

	TCHAR * pctszTmpFileName = NULL;
	std::fstream fsTmp;
	bool bTmpFileCreated = false;
	try
	{
		pctszTmpFileName = new TCHAR[ L_tmpnam + 1 ];
		pctszTmpFileName[ L_tmpnam ] = 0;
		if( 0 != ::tmpnam_s( pctszTmpFileName, L_tmpnam ) )
		{
			throw std::exception( _T( "Failed to decide temporary filename." ) );
		}
		fsTmp.open( pctszTmpFileName,
			std::ios_base::in | std::ios_base::out | std::ios_base::app );
		if( fsTmp.fail() )
		{
			std::string str = _T( "Failed to create \"" );
			str += pctszTmpFileName;
			str += _T( "\"." );
			throw std::exception( str.c_str() );
		}
		bTmpFileCreated = true;
		if( 0 != fsTmp.tellp() )
		{
			std::string str = _T( "Failed to validate \"" );
			str += pctszTmpFileName;
			str += _T( "\"." );
			throw std::exception( str.c_str() );
		}

		_PickupAlphaWord( poutstream, fsTmp, pinstream, rcparam );

		_GenerateQuatedDict( poutstream, rcparam, pinstream );

		_SContext ctx( rcparam, poutstream, fsTmp, strsetInsChar,
			rcparam.strPrefixCharFile.empty() ? strsetInsChar : strsetPrefixChar,
			rcparam.strSurfixCharFile.empty() ? strsetInsChar : strsetSurfixChar );
		_GenerateCombinedWord( ctx );

		// Finalize
		fsTmp.close();
		delete[] pctszTmpFileName;
	}
	catch( ... )
	{
		if( bTmpFileCreated )
		{
			fsTmp.close();
			if( NULL != pctszTmpFileName ) remove( pctszTmpFileName );
		}
		if( NULL != pctszTmpFileName ) delete[] pctszTmpFileName;
		throw;
	}
}



void GenerateDict( SApplicationParam const & rcparam )
{
	std::ifstream ifsSrcDict;
	std::ofstream ofsDestDict;
	CBaseInStream * pinstream = NULL;
	CBaseOutStream * poutstream = NULL;
	try
	{
		if( rcparam.strSrcDictFile.empty() )
		{
			pinstream = new CCinStream( std::cin );
		}
		else
		{
			ifsSrcDict.open( rcparam.strSrcDictFile, std::ios_base::in );
			if( ifsSrcDict.fail() )
			{
				std::string str = _T( "Failed to open \"" );
				str += rcparam.strSrcDictFile;
				str += _T( "\"." );
				throw std::exception( str.c_str() );
			}
			pinstream = new CIfstream( ifsSrcDict );
		}

		if( rcparam.strDestDictFile.empty() )
		{
			poutstream = new CCoutStream( std::cout );
		}
		else
		{
			ofsDestDict.open( rcparam.strDestDictFile,
				std::ios_base::out | std::ios_base::binary | std::ios_base::trunc );
			if( ofsDestDict.fail() )
			{
				std::string str = _T( "Failed to open \"" );
				str += rcparam.strDestDictFile;
				str += _T( "\"." );
				throw std::exception( str.c_str() );
			}
			poutstream = new COfstream( ofsDestDict );
		}

		_GenerateCombinedDict( poutstream, pinstream, rcparam );

		delete poutstream;
		ofsDestDict.close();
		delete pinstream;
		ifsSrcDict.close();
	}
	catch( ... )
	{
		if( NULL != poutstream ) delete poutstream;
		ofsDestDict.close();
		if( NULL != pinstream ) delete pinstream;
		ifsSrcDict.close();
		throw;
	}
}



