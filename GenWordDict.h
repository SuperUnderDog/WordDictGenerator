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
	std::string strDestDictFile;	// �o�̓t�@�C��
	std::string strSrcDictFile;		// ���̓t�@�C��
	std::string strInsCharFile;		// �������镶���i��j�̃t�@�C��

	std::string strPrefixCharFile;	// �擪�ɍ������镶���i��j�̃t�@�C��
	std::string strSurfixCharFile;	// �����ɍ������镶���i��j�̃t�@�C��
	int iMaxOutMega;				// �o�̓t�@�C���̃T�C�Y�̃��~�b�g
	int iMinLength;
	int iMaxLength;
	int iDictOccCount;				// ���͎����̒P��̍ő�o����
	int iCharOccCount;				// �������镶���i��j�̍ő�o����

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

