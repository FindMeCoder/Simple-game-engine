
#pragma once

#include "ETank.h"

namespace E3D
{
	enum DIRECTION
	{
		DIR_UP		= 0x000001 << 1,	// ��
		DIR_DOWN	= 0x000001 << 2,	// ��
		DIR_LEFT	= 0x000001 << 4,	// ��
		DIR_RIGHT	= 0x000001 << 8,	// ��
	};

	// ��ɫ������, ���ƽ�ɫģ�ͺͰ������
	class ESceneManager;
	class EGameManager;
	class EPlayerController
	{
	public:
		virtual ~EPlayerController();

		// ����/��ȡ��ǰ��ɫ������
		void setPosition(const EVector3D &pos);
		EVector3D getPosition() const { return mTank->getPosition();}

		// ��ȡ���Ƶ�Tank
		ETank* getTank() { return mTank; }

		void update();

		void keyPress(EInt key);
		void keyRlease(EInt key);
		void mouseWheel(EInt delta){}
		void mouseButtonDown(EInt mouseButton){}
		void mouseButtonRelease(EInt mouseButton){}
		void mouseMove(EInt x, EInt y){}

	private:
		friend class EGameManager;
		EPlayerController(EGameManager *gameMgr);

	protected:
		ESceneManager	*mScene;
		EGameManager	*mGameMgr;
		ETank			*mTank;
		ECamera			*mCamera;
		EInt			mCurDir;

		EInt			mFireDir;
	};
}