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


struct SApplicationParam
{
	std::string strDestDictFile;	// 出力ファイル
	std::string strSrcDictFile;		// 入力ファイル
	std::string strInsCharFile;		// 混合する文字（列）のファイル

	std::string strPrefixCharFile;	// 先頭に混合する文字（列）のファイル
	std::string strSurfixCharFile;	// 末尾に混合する文字（列）のファイル
	int iMaxOutMega;				// 出力ファイルのサイズのリミット
	int iMinLength;
	int iMaxLength;
	int iDictOccCount;				// 入力辞書の単語の最大出現回数
	int iCharOccCount;				// 混合する文字（列）の最大出現回数

	SApplicationParam( void )
	{
		iMaxOutMega = 4096;
		iMinLength = 8;
		iMaxLength = 16;
		iDictOccCount = 1;
		iCharOccCount = 1;
	}
};


extern void GenerateDict( SApplicationParam const & rcparam );

