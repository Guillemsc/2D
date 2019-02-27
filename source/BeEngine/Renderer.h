#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "GeometryMath.h"

class Renderer
{
public:
	Renderer() {};
	~Renderer() {};

	virtual void Start() {};
	virtual void CleanUp() {};
	virtual void Render(const float4x4& view, const float4x4& projection) {};

	void SetZPos(float set);

	float z_pos = 0;
};

#endif // !__RENDERER_H__