
#include "ECommon.h"
#include "EVector.h"
#include <list>

#pragma once
namespace E3D
{

	#define EPOLY_ATTR_2SIDE				0x0001	// ˫��
	#define EPOLY_ATTR_TRANSPARENT			0x0002	// ͸��

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

	#define EPOLY_STATE_ACTIVE				0x0100	// ��Ч��״̬
	#define EPOLY_STATE_CLIPPED				0x0200	// ���ü�״̬
	#define EPOLY_STATE_BACKFACE			0x0400	// ����״̬

	#define EOBJECT_STATE_ACTIVE			0x0001
	#define EOBJECT_STATE_VISIBLE			0x0002 
	#define EOBJECT_STATE_CULLED			0x0004

	// �����޳�����
	enum CULL_TYPE
	{
		CULL_BOX	= 0,	// �����Χ��AABB�޳�
		CULL_SPHERE	= 1,	// ����������޳�
	};

	struct EVertex4D;
	typedef std::vector<EVertex4D> EVertex4DArray;
	typedef std::vector<EVector4D> EVector4DArray;
	typedef std::vector<EPolyon4D> EPolyon4DArray;

	// �������,��������,����, uv
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

	// ��
	struct EPlane3D
	{
		EVector4D	point;	// ����һ��
		EVector4D	normal;	// ��ķ���

		EPlane3D(){}
		EPlane3D(const EVector4D &p, const EVector4D &nor);
		EPlane3D& operator=(const EPlane3D &right);
	};

	// Poly4D1_TYPE
	// �˶���νṹ�ǻ��ڶ���������ʽ�����ݽṹ
	struct EPolyon4D
	{
		EInt state;			// ״̬��Ϣ
		EInt attribute;		// �������������
		EInt color;			// �������ɫ

		
		EVertex4DArray	*verList;		// �����������ָ��transformList
		EInt			verIndex[3];	// ��������

		EPolyon4D();
	};

	// POLYF4DV1_TYP
	// �˶���νṹ�ǻ��ڶ��㷽ʽ�����ݽṹ
	struct EPolyonF4D
	{
		EInt state;			// ״̬��Ϣ
		EInt attribute;		// �������������
		EInt color;			// ����ι���ǿ����ɫ

		EVertex4D localList[3];			// ����ֲ�����
		EVertex4D transformList[3];		// ����ֲ����꾭�任֮�������

		struct EMaterial *material;

		EPolyonF4D();
	};

	struct CompEPolyonF4D
	{
		EBool operator()(const EPolyonF4D &polyA, const EPolyonF4D &polyB)
		{			
			EFloat zA = Max(polyA.transformList[0].z, Max(polyA.transformList[1].z, polyA.transformList[2].z));
			EFloat zB = Max(polyB.transformList[0].z, Max(polyB.transformList[1].z, polyB.transformList[2].z));
			// ����д�� < ��debugģʽ�»����
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

	// ���ڶ����б�Ͷ�����б������
	struct EObject4D
	{
		EString			name;			// ��������
		EInt			state;			// ����״̬
		EInt			attribute;		// ��������
		EFloat			avgRadius;		// �����ƽ���뾶, ������ײ���
		EFloat			maxRadius;		// ��������뾶

		EBool			needCull;		// �Ƿ���Ҫִ���޳�����

		EString			materiaName;	// ģ�Ͳ��ʵ�����--for Ogre Mesh

		EVector4D		minBoundingBox;	// ģ���������Χ����С��
		EVector4D		maxBoundingBox;	// ģ���������Χ�������

		// ���ﲻ��ʹ���黹������, ���Ե�һ��ģ�͵Ĳ���Ϊ׼
		EVector4D		worldPosition;	// ���嵱ǰ�����������е�λ��
		EVector4D		scale;			// ���嵱ǰ����ֵ

		EVector4D		direction;		// ����ķ�������, ��������¼�Ӹ������ת�Ƕ�
	

		EInt			vertexNumber;	// �������

		EVertex4DArray	localList;		// ����任ǰ������ֲ���������
		EVertex4DArray	transformList;	// ����任�������

		EInt			polyonNumber;	// ���������������εĸ���
		EPolyon4DArray	polyonList;		// �洢����ε�����

		// ���������Ϊ�˴���һ�������mesh�ж��submesh����������ǵ�, ����ж��submesh�Ļ�
		// nextObject != NULL �����ָ���ָ����һ��submesh, ֱ��nextObject == NULL
		EObject4D		*nextObject;

		EObject4D();
	};

	// ��Ⱦ�б�
	struct ERenderList4D
	{
		EInt						state;		// ����״̬
		EInt						attribute;	// ��������

		std::list<EPolyonF4D>		polyData;	// ����Ķ��������, ʹ��˫�˶���, ɾ������Ч

		typedef std::list<EPolyonF4D>::iterator Itr;
	};

	// ����任���Ʊ��
	enum OBJ_TRANSFORM_TYPE
	{
		TRANSFORM_LOCAL = 0,			// ֻ�Ծֲ������б���б任
		TRANSFORM_TRANS = 1,			// ֻ�Ա任��Ķ����б���б任
		TRANSFORM_LOCAL_TO_TRANS = 2,	// �Ծֲ������б���б任, ��������洢�ڱ任��Ķ����б���
	};

	// ��EObject���б任, ���Խ���λ�Ʊ任����ת�任
	// tansformTypeָ���˶���һ�������б���б任
	// transformBasis ָ�����Ƿ�Ҫ�Գ����������б任
	void Tranform_Object4D(	EObject4D* obj, 
							const EMatrix44 &mat, 
							OBJ_TRANSFORM_TYPE tansformType 
							/*,EBool transformBasis = true*/);

	// ��ERenderList4D���б任, ���Խ���λ�Ʊ任����ת�任
	// tansformTypeָ���˶���һ�������б���б任
	void Transform_Renderlist4D(ERenderList4D* renderList, 
								const EMatrix44 &mat, 
								OBJ_TRANSFORM_TYPE transformType);

	// ��Object���뵽RenderList, ����LocalList�洢����ԭʼ����, TransformList�洢���Ǿ���
	// ƽ�ƺ����ŵ�����
	void Inert_Object4D_To_RenderList4D(ERenderList4D *renderList, EObject4D *obj, 
									OBJ_TRANSFORM_TYPE transformType = TRANSFORM_LOCAL_TO_TRANS,
									bool transformScaleAndPosition = false);

	// �ֲ����궨���б�任����������, ������ֱ��ʹ�üӷ�, ��û��ʹ���������˵ķ���, ��Ϊû�б�Ҫ
	void Local_To_World_Object4D(	EObject4D* obj, 
									OBJ_TRANSFORM_TYPE transformType = TRANSFORM_LOCAL_TO_TRANS);
	// �ֲ����궨���б�任����������, ������ֱ��ʹ�üӷ�, ��û��ʹ���������˵ķ���, ��Ϊû�б�Ҫ
	void Local_To_World_RenderList4D( ERenderList4D *renderList,
									const EVector4D &pos,
									OBJ_TRANSFORM_TYPE transformType = TRANSFORM_LOCAL_TO_TRANS);

	// ����������޳�
	bool Cull_Object4D(EObject4D *object, EFrustum *camera, CULL_TYPE cullType= CULL_SPHERE);

	// ��������, ���������㷨��������ӵ��ǻ�����������, ������Ҫ�ڶ���任�����������ǰ����
	// ��������
	bool Remove_Backface(EPolyon4D *poly, EFrustum *camera);
	bool Remove_Backface(EPolyonF4D *poly, EFrustum *camera);
	void Remove_Backface_Object4D(EObject4D *object, EFrustum *camera);
	// light �Ƿ�Զ���ν��й��մ���
	void Remove_Backface_RenderList4D(ERenderList4D *renderList, EFrustum *camera);

	// ���ռ���
	// ���ռ���ֻ������������ϵ��������֮����������������ϵ�н���
	void Light_PolyonF4D(EPolyonF4D *poly, EFrustum *camera);
	void Light_RenderList4D(ERenderList4D *renderList, EFrustum *camera);

	// ��������任�����������ϵ
	void World_To_Camera_Object4D(EObject4D *obj, EFrustum *camera);
	void World_To_Camera_RenderList4D(ERenderList4D *renderList, EFrustum *camera);

	// ��RenderList�Ķ���ν����������, ʹ�û����㷨
	// ������Ҫ�ڶ���δ������������ϵʱ�ſ��Ե���, ������������
	void Sort_RenderList4D(ERenderList4D *renderList);

	// ���������ϵת��Ϊ͸������ϵ
	void Camera_To_Perspective_Object4D(EObject4D *obj, EFrustum *camera);
	void Camera_To_Perspective_RenderList4D(ERenderList4D *renderList, EFrustum *camera);

	// ͸������ϵת������Ļ����ϵ
	void Perspective_To_Screen_Object4D(EObject4D *obj, EFrustum *camera);
	void Perspective_To_Screen_RenderList4D(ERenderList4D *renderList, EFrustum *camera);

	// ��RenderList4D���вü�����, �ü��������������Χ�ڵĶ����, �˲���������������ռ���ִ��
	void Clip_RenderList4D(ERenderList4D *renderList, EFrustum *camera);

	// ��������任����Ļ����
	// �൱�ڵ���
	// World_To_Camera
	// Perspective_To_Screen��Ч��
	// ���ڲ��Ѿ�ִ�����޳�, ��������, ���մ���Ȳ���
	void World_To_Screen_RenderList4D(ERenderList4D *renderList, EFrustum *camera);


	// ��Ⱦ�߿�ģ��
	void Draw_Object4D_Wire(EObject4D *obj);
	void Draw_RenderList4D_Wire(ERenderList4D *renderList);

	// ��Ⱦʵ��ģ��
	void Draw_Object4D_Solid(EObject4D *object);
	void Draw_RenderList4D_Solid(ERenderList4D *renderList);

	// ���ƴ������renderlist
	void Draw_RenderList4D_Teture_Solid(ERenderList4D * renderList);
;}