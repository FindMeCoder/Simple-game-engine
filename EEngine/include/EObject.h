
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

		// ��ǰ�����Ƿ񻹴����
		virtual EBool isAlive() const { return mCurrentLive < mMaxLive; }
	
	protected:
		EObject() : mMaxLive(1), mCurrentLive(0){}

		EString	mName;

		EInt			mMaxLive;		// �������ʱ��
		EInt			mCurrentLive;	// ��ǰ�Ѿ�������ʱ��
	};
}