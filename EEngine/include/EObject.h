
#pragma once
#include "ECommon.h"

namespace E3D
{
	class EObject
	{
	public:
		virtual ~EObject(){}

		virtual EString getName() const { return mName; }
		virtual void update() {}

		// 当前物体是否还存活着
		virtual EBool isAlive() const { return mCurrentLive < mMaxLive; }
	
	protected:
		EObject() : mMaxLive(1), mCurrentLive(0){}

		EString	mName;

		EInt			mMaxLive;		// 最大生命时间
		EInt			mCurrentLive;	// 当前已经经过的时间
	};
}