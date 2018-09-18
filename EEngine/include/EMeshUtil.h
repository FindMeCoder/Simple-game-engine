
#pragma once

#include "ECommon.h"
#include "EOgreMeshInfo.h"
#include "ECore.h"
#include <fstream>

namespace E3D
{
	// ��stream�ж�ȡ�ַ���
	// size��ʾҪ��ȡ�ַ����ĸ���, ���size==-1,���ȡһ���ַ���
	EString ReadString(std::ifstream &in, EInt size = -1);

	// ��ȡOgre mesh�ļ�ʱʹ��
	// ��stream�ж�ȡ��һ����ʶ��, ��ӦOGREMESHID
	EUShort ReadChunk(std::ifstream &in);

	// ��stream�ж�ȡһ��bool����ֵ
	EBool ReadBool(std::ifstream &in);

	// ��stream�ж�ȡһ��unsined int���͵���ֵ
	EUInt ReadInt(std::ifstream &in);

	// ��strean�ж�ȡһ��unsigned short���͵���ֵ
	EUShort ReadShort(std::ifstream& in);

	// ��stream�ж�ȡT���͵�һ������
	// size��ʾ����Ĵ�С
	template<typename T>
	std::vector<T> ReadArray(std::ifstream &in, EInt size)
	{
		std::vector<T> v(size, 0);
		in.read((char*)(&v[0]),sizeof(T) * size);
		return v;
	}

	EObject4D* LoadOgreMesh(const EString& meshName, const EVector4D &worldPos = EVector4D::ZERO, 
							const EVector4D &scale = EVector4D(1, 1, 1));
}