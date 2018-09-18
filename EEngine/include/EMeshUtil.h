
#pragma once

#include "ECommon.h"
#include "EOgreMeshInfo.h"
#include "ECore.h"
#include <fstream>

namespace E3D
{
	// 从stream中读取字符串
	// size表示要读取字符串的个数, 如果size==-1,则读取一行字符串
	EString ReadString(std::ifstream &in, EInt size = -1);

	// 读取Ogre mesh文件时使用
	// 从stream中读取下一个标识符, 对应OGREMESHID
	EUShort ReadChunk(std::ifstream &in);

	// 从stream中读取一个bool型数值
	EBool ReadBool(std::ifstream &in);

	// 从stream中读取一个unsined int类型的数值
	EUInt ReadInt(std::ifstream &in);

	// 从strean中读取一个unsigned short类型的数值
	EUShort ReadShort(std::ifstream& in);

	// 从stream中读取T类型的一组数据
	// size标示数组的大小
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