
#pragma once

namespace E3D
{

	// Ogre Mesh文件相关信息

	// Ogre Mesh二进制文件中的ID枚举, 来源--Ogre
	enum OGREMESHID
	{
		M_HEADER              = 0x1000,
		M_MESH                = 0x3000,
			M_SUBMESH             = 0x4000, 
				
				M_GEOMETRY          = 0x5000, // NB this chunk is embedded within M_MESH and M_SUBMESH
					M_GEOMETRY_VERTEX_DECLARATION = 0x5100,
						M_GEOMETRY_VERTEX_ELEMENT = 0x5110, // Repeating section
							
					M_GEOMETRY_VERTEX_BUFFER = 0x5200, // Repeating section
						
						M_GEOMETRY_VERTEX_BUFFER_DATA = 0x5210,
							

				M_SUBMESH_OPERATION = 0x4010, // optional, trilist assumed if missing

			M_MESH_BOUNDS = 0x9000,
				
			M_SUBMESH_NAME_TABLE = 0xA000,
				M_SUBMESH_NAME_TABLE_ELEMENT = 0xA100,
					
	};

	enum OperationType {
		OT_POINT_LIST = 1,
		OT_LINE_LIST = 2,
		OT_LINE_STRIP = 3,
		OT_TRIANGLE_LIST = 4,
		OT_TRIANGLE_STRIP = 5,
		OT_TRIANGLE_FAN = 6
	};
}