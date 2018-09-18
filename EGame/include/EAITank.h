
#pragma once

#include "ETank.h"

namespace E3D
{
	// 实现了简单AI的tank
	class EAITank : public ETank
	{
	public:
		virtual ~EAITank(){}

		virtual void onHited();
		// 开火
		virtual void fire();
		// 更新tank
		virtual void update();

	protected:
		// 更新AI
		void updateAI();

	protected:
		friend class EGameManager;
		friend class EPlayerController;
		EAITank(const EString &name, const EString &meshName, EGameManager *gameMgr);
	};
}