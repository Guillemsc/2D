#ifndef __VERTEX_BUFFER_H__
#define __VERTEX_BUFFER_H__

#include "ModuleRenderer.h"
#include "MathGeoLib.h"
#include "ModuleShader.h"

class VertexBuffer
{
public:
	VertexBuffer();

	void AddFloat(float set);
	void AddFloat2(const float2& set);
	void AddFloat3(const float3& set);
	void AddFloat4(const float4& set);

	const float* GetBuffer() const;
	const uint GetSize() const;

	void AddSpace(int add);

	void Clear();

private:
	void Resize(int size);
	bool NeedsResize(int new_size);

private:
	float* buffer = nullptr;
	uint buffer_size = 0;
	uint buffer_used = 0;
};

#endif // !__VERTEX_BUFFER_H__