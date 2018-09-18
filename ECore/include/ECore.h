
#include "ECommon.h"
#include "EVector.h"
#include <list>

#pragma once
namespace E3D
{

	#define EPOLY_ATTR_2SIDE				0x0001	// 双面
	#define EPOLY_ATTR_TRANSPARENT			0x0002	// 透明

	#define EPOLY_ATTR_SHADE_MODE_PURE      0x0004
	#define EPOLY_ATTR_SHADE_MODE_CONSTANT  0x0004 // (alias)
	#define EPOLY_ATTR_SHADE_MODE_FLAT      0x0008
	#define EPOLY_ATTR_SHADE_MODE_GOURAUD   0x0010
	#define EPOLY_ATTR_SHADE_MODE_PHONG     0x0020
	#define EPOLY_ATTR_SHADE_MODE_FASTPHONG 0x0020 // (alias)
	#define EPOLY_ATTR_SHADE_MODE_TEXTURE   0x0040 

	#define EPOLY_ATTR_VERTEX_POSITION		0x1000
	#define EPOLY_ATTR_VERTEX_NORMAL		0x2000
	#define EPOLY_ATTR_VERTEX_UV			0x4000

	#define EPOLY_STATE_ACTIVE				0x0100	// 有效的状态
	#define EPOLY_STATE_CLIPPED				0x0200	// 被裁减状态
	#define EPOLY_STATE_BACKFACE			0x0400	// 背面状态

	#define EOBJECT_STATE_ACTIVE			0x0001
	#define EOBJECT_STATE_VISIBLE			0x0002 
	#define EOBJECT_STATE_CULLED			0x0004

	// 物体剔除类型
	enum CULL_TYPE
	{
		CULL_BOX	= 0,	// 物体包围盒AABB剔除
		CULL_SPHERE	= 1,	// 物体球体积剔除
	};

	struct EVertex4D;
	typedef std::vector<EVertex4D> EVertex4DArray;
	typedef std::vector<EVector4D> EVector4DArray;
	typedef std::vector<EPolyon4D> EPolyon4DArray;

	// 顶点对象,包含顶点,法线, uv
	struct EVertex4D
	{
		EFloat x, y, z, w;
	
		EFloat u, v;

		EVertex4D();
		EVertex4D(const EVector4D &v);
		EVertex4D(EFloat xf, EFloat yf, EFloat zf);
		EVertex4D operator *(const EVector4D &right);
		EVertex4D operator -(const EVertex4D &right);
		EVertex4D& operator =(const EVertex4D &right);
		EVertex4D operator +(const EVector4D &right);
		EVertex4D operator +(const EVertex4D &right);
		EVertex4D operator /(EFloat factor);
		
		EVector4D toVector4D() const;
	};

	// 面
	struct EPlane3D
	{
		EVector4D	point;	// 面上一点
		EVector4D	normal;	// 面的法线

		EPlane3D(){}
		EPlane3D(const EVector4D &p, const EVector4D &nor);
		EPlane3D& operator=(const EPlane3D &right);
	};

	// Poly4D1_TYPE
	// 此多边形结构是基于顶点索引方式的数据结构
	struct EPolyon4D
	{
		EInt state;			// 状态信息
		EInt attribute;		// 多边形物理属性
		EInt color;			// 多边形颜色

		
		EVertex4DArray	*verList;		// 这里的引用是指的transformList
		EInt			verIndex[3];	// 定点索引

		EPolyon4D();
	};

	// POLYF4DV1_TYP
	// 此多边形结构是基于顶点方式的数据结构
	struct EPolyonF4D
	{
		EInt state;			// 状态信息
		EInt attribute;		// 多边形物理属性
		EInt color;			// 多边形光照强度颜色

		EVertex4D localList[3];			// 物体局部坐标
		EVertex4D transformList[3];		// 物体局部坐标经变换之后的坐标

		struct EMaterial *material;

		EPolyonF4D();
	};

	struct CompEPolyonF4D
	{
		EBool operator()(const EPolyonF4D &polyA, const EPolyonF4D &polyB)
		{			
			EFloat zA = Max(polyA.transformList[0].z, Max(polyA.transformList[1].z, polyA.transformList[2].z));
			EFloat zB = Max(polyB.transformList[0].z, Max(polyB.transformList[1].z, polyB.transformList[2].z));
			// 这里写成 < 在debug模式下会崩溃
			if (Abs(zA - zB) < 0.005f)
			{
				zA = (polyA.transformList[0].z + polyA.transformList[1].z + polyA.transformList[2].z) / 3.0f;
				zB = (polyB.transformList[0].z + polyB.transformList[1].z + polyB.transformList[2].z) / 3.0f;
				
				if (zA <= zB)
					return false;
				else
					return true;
			}
			else if (zA < zB)
				return false;
			else
				return true;
		}
	};

	// 基于定点列表和多边形列表的物体
	struct EObject4D
	{
		EString			name;			// 物体名称
		EInt			state;			// 物体状态
		EInt			attribute;		// 物体属性
		EFloat			avgRadius;		// 物体的平均半径, 用于碰撞检测
		EFloat			maxRadius;		// 物体的最大半径

		EBool			needCull;		// 是否需要执行剔除操作

		EString			materiaName;	// 模型材质的名称--for Ogre Mesh

		EVector4D		minBoundingBox;	// 模型正方体包围盒最小点
		EVector4D		maxBoundingBox;	// 模型正方体包围盒做大点

		// 这里不论使评议还是缩放, 均以第一个模型的参数为准
		EVector4D		worldPosition;	// 物体当前在世界坐标中的位置
		EVector4D		scale;			// 物体当前缩放值

		EVector4D		direction;		// 物体的方向向量, 各分量记录延各轴的旋转角度
	

		EInt			vertexNumber;	// 顶点个数

		EVertex4DArray	localList;		// 物体变换前的物体局部坐标数组
		EVertex4DArray	transformList;	// 物体变换后的坐标

		EInt			polyonNumber;	// 物体的所包含多边形的个数
		EPolyon4DArray	polyonList;		// 存储多边形的数组

		// 这个变量是为了处理一个导入的mesh有多个submesh的情况来考虑的, 如果有多个submesh的话
		// nextObject != NULL 否则该指针会指向下一个submesh, 直至nextObject == NULL
		EObject4D		*nextObject;

		EObject4D();
	};

	// 渲染列表
	struct ERenderList4D
	{
		EInt						state;		// 物体状态
		EInt						attribute;	// 物体属性

		std::list<EPolyonF4D>		polyData;	// 具体的多边形数据, 使用双端队列, 删除更有效

		typedef std::list<EPolyonF4D>::iterator Itr;
	};

	// 顶点变换控制标记
	enum OBJ_TRANSFORM_TYPE
	{
		TRANSFORM_LOCAL = 0,			// 只对局部顶点列表进行变换
		TRANSFORM_TRANS = 1,			// 只对变换后的顶点列表进行变换
		TRANSFORM_LOCAL_TO_TRANS = 2,	// 对局部定点列表进行变换, 并将结果存储在变换后的顶点列表中
	};

	// 对EObject进行变换, 可以进行位移变换和旋转变换
	// tansformType指定了对哪一个定点列表进行变换
	// transformBasis 指定了是否要对朝向向量进行变换
	void Tranform_Object4D(	EObject4D* obj, 
							const EMatrix44 &mat, 
							OBJ_TRANSFORM_TYPE tansformType 
							/*,EBool transformBasis = true*/);

	// 对ERenderList4D进行变换, 可以进行位移变换和旋转变换
	// tansformType指定了对哪一个定点列表进行变换
	void Transform_Renderlist4D(ERenderList4D* renderList, 
								const EMatrix44 &mat, 
								OBJ_TRANSFORM_TYPE transformType);

	// 将Object插入到RenderList, 这里LocalList存储的是原始坐标, TransformList存储的是经过
	// 平移和缩放的坐标
	void Inert_Object4D_To_RenderList4D(ERenderList4D *renderList, EObject4D *obj, 
									OBJ_TRANSFORM_TYPE transformType = TRANSFORM_LOCAL_TO_TRANS,
									bool transformScaleAndPosition = false);

	// 局部坐标定编列表变换至世界坐标, 这里是直接使用加法, 而没有使用与矩阵相乘的方法, 因为没有必要
	void Local_To_World_Object4D(	EObject4D* obj, 
									OBJ_TRANSFORM_TYPE transformType = TRANSFORM_LOCAL_TO_TRANS);
	// 局部坐标定编列表变换至世界坐标, 这里是直接使用加法, 而没有使用与矩阵相乘的方法, 因为没有必要
	void Local_To_World_RenderList4D( ERenderList4D *renderList,
									const EVector4D &pos,
									OBJ_TRANSFORM_TYPE transformType = TRANSFORM_LOCAL_TO_TRANS);

	// 对物体进行剔除
	bool Cull_Object4D(EObject4D *object, EFrustum *camera, CULL_TYPE cullType= CULL_SPHERE);

	// 消除背面, 背面消除算法的摄像机视点是基于世界坐标, 所以需要在定点变换成摄像机坐标前进行
	// 背面消除
	bool Remove_Backface(EPolyon4D *poly, EFrustum *camera);
	bool Remove_Backface(EPolyonF4D *poly, EFrustum *camera);
	void Remove_Backface_Object4D(EObject4D *object, EFrustum *camera);
	// light 是否对多边形进行光照处理
	void Remove_Backface_RenderList4D(ERenderList4D *renderList, EFrustum *camera);

	// 光照计算
	// 光照计算只能在世界坐标系消除背面之后或者在摄像机坐标系中进行
	void Light_PolyonF4D(EPolyonF4D *poly, EFrustum *camera);
	void Light_RenderList4D(ERenderList4D *renderList, EFrustum *camera);

	// 世界坐标变换成摄像机坐标系
	void World_To_Camera_Object4D(EObject4D *obj, EFrustum *camera);
	void World_To_Camera_RenderList4D(ERenderList4D *renderList, EFrustum *camera);

	// 对RenderList的多边形进行深度排序, 使用画家算法
	// 这里需要在多边形处于摄像机坐标系时才可以调用, 否则会出现问题
	void Sort_RenderList4D(ERenderList4D *renderList);

	// 摄像机坐标系转换为透视坐标系
	void Camera_To_Perspective_Object4D(EObject4D *obj, EFrustum *camera);
	void Camera_To_Perspective_RenderList4D(ERenderList4D *renderList, EFrustum *camera);

	// 透视坐标系转换至屏幕坐标系
	void Perspective_To_Screen_Object4D(EObject4D *obj, EFrustum *camera);
	void Perspective_To_Screen_RenderList4D(ERenderList4D *renderList, EFrustum *camera);

	// 对RenderList4D进行裁剪操作, 裁剪掉超出摄像机范围内的多边形, 此操作必须在摄像机空间内执行
	void Clip_RenderList4D(ERenderList4D *renderList, EFrustum *camera);

	// 世界坐标变换到屏幕坐标
	// 相当于调用
	// World_To_Camera
	// Perspective_To_Screen的效用
	// 其内部已经执行了剔除, 背面消除, 光照处理等操作
	void World_To_Screen_RenderList4D(ERenderList4D *renderList, EFrustum *camera);


	// 渲染线框模型
	void Draw_Object4D_Wire(EObject4D *obj);
	void Draw_RenderList4D_Wire(ERenderList4D *renderList);

	// 渲染实体模型
	void Draw_Object4D_Solid(EObject4D *object);
	void Draw_RenderList4D_Solid(ERenderList4D *renderList);

	// 绘制带纹理的renderlist
	void Draw_RenderList4D_Teture_Solid(ERenderList4D * renderList);
;}