
#pragma once
#include "EGraphics.h"

namespace E3D
{
	// 绘图接口
	class ECanvas
	{
	public:
		virtual ~ECanvas(){}

		// 这里只负责绘制模型
		virtual void onPaint() = 0;
	
		// 这里更新场景
		virtual void update() = 0;
	};
}