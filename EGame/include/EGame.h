
#pragma once

#include "ECanvas.h"
#include "EInputListener.h"

namespace E3D
{
	// 游戏实体类, EGame相当于一个关卡, 有接受输入控制和绘制的功能
	class EGame : public ECanvas, public EInputListener
	{
	public:
		EGame();

		// 键盘鼠标操作
		virtual EBool keyPress(EInt key);
		virtual EBool keyRlease(EInt key);

		virtual EBool mouseButtonDown(EInt mouseButton){return true;}
		virtual EBool mouseButtonRelease(EInt mouseButton){return true;}
		virtual EBool mouseMove(EInt x, EInt y){return true;}
		virtual EBool mouseWheel(EInt delta){return true;}

		// 更新和绘制
		virtual void onPaint();
		virtual void update();

	protected:
		// 创建场景
		virtual void createScene();
		// 显示相关信息
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