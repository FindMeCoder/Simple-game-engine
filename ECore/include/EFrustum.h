
#pragma once
#include "ECommon.h"
#include "EVector.h"
#include "EMatrix.h"
#include "ECore.h"

namespace E3D
{

	enum CAMERA_TYPE
	{
		CAMERA_MODEL_ELUA	= 0x0001,	// 欧拉摄像机模型
		CAMERA_MODEL_UVN	= 0x0002,	// UVN摄像机模型
	};

	struct EFrustum
	{
		EInt		state;		// 摄像机状态
		EInt		attribute;	// 摄像机属性
		CAMERA_TYPE	camMode;

		EVector4D	position;	// 摄像机在世界坐标系的位置
		EVector4D	direction;	// 摄像机注视的方向--朝向

		// UVN模型使用, 用于定义UVN摄像机
		EVector4D	camUp;		// UVN摄像机模型的朝向上向量
		EVector4D	camRight;	// UVN摄像机模型的朝向右向量
		EVector4D	camLook;	// UVN摄像机模型的朝向注视方向
		EVector4D	camTarget;		// UVN摄像机模型的注视点

		EFloat		view_dist;	// 视点到投影面的距离

		EFloat		fov;			// 水平方向和垂直方向的视野

		EFloat		clip_z_near;	// 近裁减面
		EFloat		clip_z_far;		// 远裁减面

		EPlane3D	clip_plane_R;	// 右裁减面
		EPlane3D	clip_plane_L;	// 左裁减面
		EPlane3D	clip_plane_T;	// 上裁减面
		EPlane3D	clip_plane_B;	// 下裁减面

		EFloat		viewplane_width;		// 投影面宽高
		EFloat		viewplane_height;		// 对于归一化投影, 为2X2， 否则大小与屏幕窗口大小相同

		EFloat		viewport_width;			// 屏幕/视口的大小
		EFloat		viewport_height;
		EFloat		viewport_center_X;		// 屏幕中心坐标
		EFloat		viewprot_center_Y;

		EFloat		aspect_ratio;			// 屏幕的高宽比

		EMatrix44	mWorldToCamera;				// 世界坐标到相机坐标变换矩阵
		EMatrix44	mCameraToPerspective;	// 相机坐标到透视坐标系变换矩阵
		EMatrix44	mPerspectiveToScreen;	// 透视坐标系到屏幕坐标系变换矩阵

		EFrustum(CAMERA_TYPE mode, const EVector4D &pos, const EVector4D &dir,
				const EVector4D& target, EFloat nearZ, EFloat farZ, EFloat ffov, 
				EFloat viewportWidth, EFloat viewportHeight);
	};
}