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
#include <string>

#include "ParseArgument.h"


static inline int ParseInt( TCHAR const * const pctsz )
{
	TCHAR * ptcCheck = NULL;
	int const iResult = ::strtol( pctsz, &ptcCheck, 10 );
	if( ERANGE == errno )
	{
		std::string str = _T( "Too huge number\"" );
		str += pctsz;
		str += _T( "\"." );
		throw std::exception( str.c_str() );
	}
	else if( _T( '\0' ) != *ptcCheck )
	{
		std::string str = _T( "Specified not number \"" );
		str += pctsz;
		str += _T( "\"." );
		throw std::exception( str.c_str() );
	}
	return iResult;
}


static inline int _ParseArg_TrailingNum(
	int & riPos, int const argc, TCHAR const * const argv[],
	TCHAR const * const pctszArgStr, int const iMin, int const iMax )
{
	if( argc <= riPos )
	{
		std::string str = _T( "Not specified number to argument\"" );
		str += pctszArgStr;
		str += _T( "\"." );
		throw std::exception( str.c_str() );
	}

	int const iResult = ParseInt( argv[ riPos ] );
	if( iMin > iResult )
	{
		std::string str = _T( "Number\"" );
		str += std::to_string( iResult );
		str += _T( "\" to argument\"" );
		str += pctszArgStr;
		str += _T( "\" is smaller than \"" );
		str += std::to_string( iMin );
		str += _T( "\"." );
		throw std::exception( str.c_str() );
	}
	if( iMax < iResult )
	{
		std::string str = _T( "Number\"" );
		str += std::to_string( iResult );
		str += _T( "\" to argument\"" );
		str += pctszArgStr;
		str += _T( "\" is greater than \"" );
		str += std::to_string( iMax );
		str += _T( "\"." );
		throw std::exception( str.c_str() );
	}
	riPos++;
	return iResult;
}

static inline std::string _ParseArg_TrailingFileName(
	int & riPos, int const argc, TCHAR const * const argv[],
	TCHAR const * const pctszArgStr )
{
	if( argc <= riPos )
	{
		std::string str = _T( "Not specified filename to argument\"" );
		str += pctszArgStr;
		str += _T( "\"." );
		throw std::exception( str.c_str() );
	}

	std::string const strResult = argv[ riPos ];
	riPos++;
	return strResult;
}

static inline void _ParseArg_Switch( bool & rbEnd, int & riPos,
	SApplicationParam & rparam, int const argc, TCHAR const * const argv[] )
{
	TCHAR const * const pctsz = argv[ riPos ];
	int const iLen = _tcslen( pctsz );
	if( 0 >= iLen )
	{
		throw std::exception( _T( "Not specified option charactor(s)." ) );
	}
	if( ( 0 == _tcscmp( &( pctsz[ 1 ] ), _T( "?" ) ) ) ||
		( 0 == _tcscmp( &( pctsz[ 1 ] ), _T( "h" ) ) ) )
	{
		rbEnd = true;
	}
	else if( 0 == _tcscmp( &( pctsz[ 1 ] ), _T( "c" ) ) )
	{
		riPos++;
		rparam.strInsCharFile =
			_ParseArg_TrailingFileName( riPos, argc, argv, pctsz );
	}
	else if( 0 == _tcscmp( &( pctsz[ 1 ] ), _T( "o" ) ) )
	{
		riPos++;
		rparam.strDestDictFile =
			_ParseArg_TrailingFileName( riPos, argc, argv, pctsz );
	}
	else if( 0 == _tcscmp( &( pctsz[ 1 ] ), _T( "i" ) ) )
	{
		riPos++;
		rparam.strSrcDictFile =
			_ParseArg_TrailingFileName( riPos, argc, argv, pctsz );
	}
	else if( 0 == _tcscmp( &( pctsz[ 1 ] ), _T( "cp" ) ) )
	{
		riPos++;
		rparam.strPrefixCharFile =
			_ParseArg_TrailingFileName( riPos, argc, argv, pctsz );
	}
	else if( 0 == _tcscmp( &( pctsz[ 1 ] ), _T( "cs" ) ) )
	{
		riPos++;
		rparam.strSurfixCharFile =
			_ParseArg_TrailingFileName( riPos, argc, argv, pctsz );
	}
	else if( 0 == _tcscmp( &( pctsz[ 1 ] ), _T( "omax" ) ) )
	{
		riPos++;
		rparam.iMaxOutMega =
			_ParseArg_TrailingNum( riPos, argc, argv, pctsz, 1, INT_MAX );
	}
	else if( 0 == _tcscmp( &( pctsz[ 1 ] ), _T( "wmin" ) ) )
	{
		riPos++;
		rparam.iMinLength =
			_ParseArg_TrailingNum( riPos, argc, argv, pctsz, 1, 64 );
	}
	else if( 0 == _tcscmp( &( pctsz[ 1 ] ), _T( "wmax" ) ) )
	{
		riPos++;
		rparam.iMaxLength =
			_ParseArg_TrailingNum( riPos, argc, argv, pctsz, 1, 64 );
	}
	else if( 0 == _tcscmp( &( pctsz[ 1 ] ), _T( "in" ) ) )
	{
		riPos++;
		rparam.iDictOccCount =
			_ParseArg_TrailingNum( riPos, argc, argv, pctsz, 1, 5 );
	}
	else if( 0 == _tcscmp( &( pctsz[ 1 ] ), _T( "cn" ) ) )
	{
		riPos++;
		rparam.iCharOccCount =
			_ParseArg_TrailingNum( riPos, argc, argv, pctsz, 1, 5 );
	}
	else
	{
		std::string str = _T( "Invalid argument \"" );
		str += pctsz;
		str += _T( "\" (-? to show usage)." );
		throw std::exception( str.c_str() );
	}
}


TCHAR const tctszUsage[] = _T( "\n"
	"  usage: worddictgen [option]\n\n"
	"  required\n"
	"    -c <insert charactor(s) file>\n"
	"\n  option\n"
	"    -? -h                        // Help\n"
	"    -o <output word list file>   // (default stdout)\n"
	"    -i <input word list file>    // (default stdin)\n"
	"    -cp <prefix charactor(s) file>\n"
	"    -cs <surfix charactor(s) file>\n"
	"    -omax <max out size(mega)>               // 1-? (default 4096)\n"
	"    -wmin <min length>                       // 1-? (default 8)\n"
	"    -wmax <max length>                       // 1-? (default 14)\n"
	"    -in <max occurrences of input word>      // 1-? (default 1)\n"
	"    -cn <max occurrences of insert char>     // 1-? (default 1)\n"
	"\n" );

void ParseArgument( bool & rbEnd,
	SApplicationParam & rparam, int const argc, TCHAR const * const argv[] )
{
	rbEnd = false;
	if( 1 >= argc )
	{
		rbEnd = true;
	}
	else
	{
		int iPos = 1;
		while( argc > iPos )
		{
			TCHAR const * const pctsz = argv[ iPos ];
			int const iLen = _tcslen( pctsz );
			if( 0 >= iLen )
			{
				std::string str = _T( "Argument " );
				str += std::to_string( argc );
				str += _T( " have not entity." );
				throw std::exception( str.c_str() );
			}
			if( ( _T( '-' ) == pctsz[ 0 ] ) || ( _T( '/' ) == pctsz[ 0 ] ) )
			{
				_ParseArg_Switch( rbEnd, iPos, rparam, argc, argv );
			}
			else
			{
				std::string str = _T( "Invalid argument \"" );
				str += pctsz;
				str += _T( "\" (-? to show usage)." );
				throw std::exception( str.c_str() );
			}
		}
		if( ! rbEnd )
		{
			if( rparam.strInsCharFile.empty() )
			{
				throw std::exception( _T( "Argument insert charactor(s) list file is not specified." ) );
			}
			if( rparam.iMinLength > rparam.iMaxLength )
			{
				throw std::exception( _T( "Argument minimum length is greater than max length." ) );
			}
		}
	}
}
