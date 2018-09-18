
#pragma once

#include "ECanvas.h"
#include "EInputListener.h"

namespace E3D
{
	// ��Ϸʵ����, EGame�൱��һ���ؿ�, �н���������ƺͻ��ƵĹ���
	class EGame : public ECanvas, public EInputListener
	{
	public:
		EGame();

		// ����������
		virtual EBool keyPress(EInt key);
		virtual EBool keyRlease(EInt key);

		virtual EBool mouseButtonDown(EInt mouseButton){return true;}
		virtual EBool mouseButtonRelease(EInt mouseButton){return true;}
		virtual EBool mouseMove(EInt x, EInt y){return true;}
		virtual EBool mouseWheel(EInt delta){return true;}

		// ���ºͻ���
		virtual void onPaint();
		virtual void update();

	protected:
		// ��������
		virtual void createScene();
		// ��ʾ�����Ϣ
		void showInfo();

	protected:
		EInt					currentObjectNum;
		EInt					visiblePolyons;

		EBool					showHelp;

		class ECamera			*camera;
		class EMesh				*plane;
		
		class ESceneManager		*scene;
		class EPlayerController	*player;
		class EGameManager		*gameMgr;

		EInt					mLogicX, mLogicZ;
	};
}