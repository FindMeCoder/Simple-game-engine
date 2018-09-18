
#include "EMeshUtil.h"
#include "EUtil.h"
#include "EMaterial.h"

namespace E3D
{

	

	EString ReadString(std::ifstream &in, EInt size)
	{
		EString ss;
		if (size == -1) // 读取一行
		{
			EChar c;
			while (in.read(&c, sizeof(EChar)) && c != '\n' && c != '\r')
				ss += c;
		}
		else
		{
			EChar c[256];
			in.read(c, sizeof(EChar) * size);
			ss.append(c);
		}

		return ss;
	}

	

	EUShort ReadChunk(std::ifstream &in)
	{
		EUShort id;
		in.read((EChar*)&id, sizeof(EUShort));

		EUInt length;
		in.read((EChar*)&length, sizeof(EUInt));

		return id;
	}

	

	EBool ReadBool(std::ifstream &in)
	{
		EBool re = false;
		in.read((EChar*)&re, sizeof(EBool));
		return re;
	}

	

	EUInt ReadInt(std::ifstream &in)
	{
		EUInt re;
		in.read((char*)&re, sizeof(EUInt));
		return re;
	}

	

	EUShort ReadShort(std::ifstream& in)
	{
		EUShort re;
		in.read((EChar*)&re, sizeof(EUShort));
		return re;
	}

	

	EObject4D* LoadOgreMesh(const EString& meshName, const EVector4D &worldPos, const EVector4D &scale)
	{
		std::ifstream in;
		// 模型文件, 首先获取默认路径
		in.open(GetPath(meshName).c_str(), std::ios_base::binary | std::ios_base::in);
		if (in.bad())
		{
			in.close();
			return NULL;
		}

		EString name = GetNameFromPath(meshName);
		// 加载材质
		ParseMaterialScript(name + ".material");

		EObject4D *head = NULL;
		{
			// Chunk--M_HEADER
			EUShort HEADER = ReadShort(in);
			// Version
			EString Version = ReadString(in);

			// Chunk--M_MESH
			EUShort MESH = ReadChunk(in);
			// skeletallyAnimated
			EBool SkeletallyAnimated = ReadBool(in);

			// Chunk--M_SUBMESH
			EUShort SUBMESH = ReadChunk(in);
			EObject4D* curObject = NULL;
			while (!in.eof() && (SUBMESH == M_SUBMESH))
			{
				std::vector<EUInt> indexArray;
				std::vector<EFloat> vertexArray;

				if (head == NULL)
				{
					head		= new EObject4D;
					curObject	= head;
				}
				else
				{
					curObject->nextObject	= new EObject4D;
					curObject				= curObject->nextObject;
				}

				curObject->name				= name;	
				curObject->state			= EOBJECT_STATE_ACTIVE | EOBJECT_STATE_VISIBLE;
				// 设置坐标
				curObject->worldPosition	= worldPos;
				curObject->scale			= scale;

				// Materia
				EString Materia = ReadString(in);
				// 读取多边形的材质
				curObject->materiaName = Materia;

				// bool useSharedVertices
				EBool UseSharedVertices = ReadBool(in); 

				// unsigned int indexCount
				EUInt indexCount = ReadInt(in);
				// 获取模型多边形个数
				curObject->polyonNumber = indexCount / 3;
				curObject->polyonList.reserve(indexCount / 3);

				// bool indexes32Bit
				EBool indexes32Bit = ReadBool(in);
				if(indexes32Bit)
				{
					// unsigned int* faceVertexIndices (indexCount)
					std::vector<EUInt> faceVertexIndices = ReadArray<EUInt>(in, indexCount);
					// 读取模型顶点索引
					indexArray.assign(faceVertexIndices.begin(), faceVertexIndices.end());
				}
				else
				{
					// unsigned short* faceVertexIndices (indexCount)
					std::vector<EUShort> faceVertexIndices = ReadArray<EUShort>(in, indexCount);
					// 读取模型定点索引
					indexArray.assign(faceVertexIndices.begin(), faceVertexIndices.end());
				}

				// Chunk--M_GEOMETRY
				EUShort GEOMETRY = ReadChunk(in);

				// unsigned int vertexCount
				EUInt vertexCount = ReadInt(in);
				// 模型顶点个数
				curObject->vertexNumber = vertexCount;
				curObject->localList.reserve(vertexCount);
				curObject->transformList.reserve(vertexCount);

				// Chunk--M_GEOMETRY_VERTEX_DECLARATION
				EUShort GEOMETRY_VERTEX_DECLARATION = ReadChunk(in);
				// Chunk--M_GEOMETRY_VERTEX_ELEMENT
				EUShort GEOMETRY_VERTEX_ELEMENT = ReadChunk(in);
				while (!in.eof() && GEOMETRY_VERTEX_ELEMENT == M_GEOMETRY_VERTEX_ELEMENT)
				{
					// unsigned short source;  	// buffer bind source
					EUShort source = ReadShort(in);
					// unsigned short type;    	// VertexElementType
					EUShort type = ReadShort(in);
					// unsigned short semantic; // VertexElementSemantic
					EUShort semantic = ReadShort(in);
					// unsigned short offset;	// start offset in buffer in bytes
					EUShort offset = ReadShort(in);
					// unsigned short index;	// index of the semantic (for colours and texture coords)
					EUShort index = ReadShort(in);
					GEOMETRY_VERTEX_ELEMENT = ReadChunk(in);
				}

				//Chunk--M_GEOMETRY_VERTEX_BUFFER
				EUShort GEOMETRY_VERTEX_BUFFER = GEOMETRY_VERTEX_ELEMENT;
				// unsigned short bindIndex;	// Index to bind this buffer to
				EUShort bindIndex = ReadShort(in);

				// unsigned short vertexSize;	// Per-vertex size, must agree with declaration at this index
				EUShort vertexSize = ReadShort(in);

				// Chunk--M_GEOMETRY_VERTEX_BUFFER_DATA
				EUShort GEOMETRY_VERTEX_BUFFER_DATA = ReadChunk(in);

				EInt vertexNum = vertexSize / sizeof(EFloat);
				// data buffer
				EInt bufferSize = vertexCount * vertexNum;

				// 读取顶点数据

				vertexArray = ReadArray<EFloat>(in, bufferSize);
				
				for (EInt i = 0 ; i < (EInt)vertexArray.size(); i += vertexNum)
				{
					EVertex4D vex;
					// 顶点坐标
					vex.x	= vertexArray[i];
					vex.y	= vertexArray[i + 1];
					vex.z	= vertexArray[i + 2];
					// 顶点法线
					//vex.nx	= vertexArray[i + 3];
					//vex.ny	= vertexArray[i + 4];
					//vex.nz	= vertexArray[i + 5];
					// 顶点UV
					vex.u	= vertexArray[i + 6];
					vex.v	= vertexArray[i + 7];

					// 这里对u，v坐标进行限制在[0, 1]之间
					//vex.u	= std::abs(vex.u - (EInt)vex.u);
					//vex.v	= std::abs(vex.v - (EInt)vex.v);

					curObject->localList.push_back(vex);
					curObject->transformList.push_back(vex);
				}

				for (EInt i = 0; i < curObject->polyonNumber; i++)
				{
					EInt index = i * 3;
					EPolyon4D poly;
					poly.state		= EPOLY_STATE_ACTIVE;
					poly.attribute	= EPOLY_ATTR_VERTEX_NORMAL | EPOLY_ATTR_VERTEX_POSITION | EPOLY_ATTR_VERTEX_UV;
					poly.verIndex[0] = indexArray[index];
					poly.verIndex[1] = indexArray[index + 1];
					poly.verIndex[2] = indexArray[index + 2];

					// 这里将变换后的顶点指针交给每个多边形, 是为了防止变换之后定点索引依旧是
					// 最初的顶点坐标
					poly.verList = &curObject->transformList;
					curObject->polyonList.push_back(poly);
				}

				// Chunk--M_SUBMESH_OPERATION
				EUShort SUBMESH_OPERATION = ReadChunk(in);
				// unsigned short operationType
				EUShort operationType = ReadShort(in);	

				SUBMESH = ReadChunk(in);
			}

			// Chunk--M_MESH_BOUNDS
			EUShort MESH_BOUNDS = SUBMESH;
			if (MESH_BOUNDS == M_MESH_BOUNDS)
			{
				// float minx, miny, minz
				// float maxx, maxy, maxz
				// float radius
				std::vector<EFloat> bounds = ReadArray<EFloat>(in, 7);
				head->minBoundingBox = EVector4D(bounds[0], bounds[1], bounds[2]);
				head->maxBoundingBox = EVector4D(bounds[3], bounds[4], bounds[5]);
				head->avgRadius = (head->maxBoundingBox - head->minBoundingBox).length() / 2;

				EFloat minR = head->minBoundingBox.length();
				EFloat maxR = head->maxBoundingBox.length();
				head->maxRadius = maxR > minR ? maxR : minR;

				//Chunk--M_SUBMESH_NAME_TABLE
				EUShort SUBMESH_NAME_TABLE = ReadChunk(in);
				if (SUBMESH_NAME_TABLE == M_SUBMESH_NAME_TABLE)
				{
					// Chunk--M_SUBMESH_NAME_TABLE_ELEMENT
					EUShort SUBMESH_NAME_TABLE_ELEMENT = ReadChunk(in);
					while (!in.eof() && SUBMESH_NAME_TABLE_ELEMENT == M_SUBMESH_NAME_TABLE_ELEMENT)
					{
						// short index
						// char* name
						EShort index = ReadShort(in);
						EString name = ReadString(in);
						SUBMESH_NAME_TABLE_ELEMENT = ReadChunk(in);
					}
				}

			}
		}

		in.close();
		return head;
	}
}