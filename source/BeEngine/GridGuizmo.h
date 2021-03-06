#ifndef __GRID_GUIZMO_H__
#define __GRID_GUIZMO_H__

#include "GeometryMath.h"
#include "Guizmo.h"

class GridGuizmo : public Guizmo
{
public:
	GridGuizmo() {};
	~GridGuizmo() {};

	void Start();
	void Render(float relative_size, const float2& mouse_pos);

private:

};

#endif // !__POSITION_GUIZMO_H__