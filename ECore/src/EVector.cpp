
#include "EVector.h"

namespace E3D
{
	EVector2D EVector2D::ZERO(0, 0);
	EVector2D EVector2D::UNIT_X(1, 0);
	EVector2D EVector2D::UNIT_Y(0, 1);

	EVector3D EVector3D::ZERO(0, 0, 0);
	EVector3D EVector3D::UNIT_X(1, 0, 0);
	EVector3D EVector3D::UNIT_Y(0, 1, 0);
	EVector3D EVector3D::UNIT_Z(0, 0, 1);

	EVector4D EVector4D::ZERO(0, 0, 0);
	EVector4D EVector4D::UNIT_X(1, 0, 0);
	EVector4D EVector4D::UNIT_Y(0, 1, 0);
	EVector4D EVector4D::UNIT_Z(0, 0, 1);
}