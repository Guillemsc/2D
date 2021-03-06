#ifndef __GUIZMO_H__
#define __GUIZMO_H__

#include "GeometryMath.h"
#include "Globals.h"

class GuizmoHandler;

class Guizmo
{
	friend class ModuleGuizmo;

public:
	Guizmo() {};
	~Guizmo() {};

	virtual void Start() {};
	virtual void Render(float relative_size, const float2& mouse_pos) {};

	void CleanUp();

	uint GetHandlersCount() const;
	GuizmoHandler* GetHandler(uint index) const;

	void SetEnabled(bool set);

protected:
	GuizmoHandler* AddHandler();

	void DestroyAllHandlers();

protected:
	std::vector<GuizmoHandler*> handlers;

private:
	bool	 keep_size = true; 
	bool     visible = true;
	bool     enabled = true;
};

class GuizmoHandler
{
	friend class ModuleGuizmo;

public:
	GuizmoHandler();
	~GuizmoHandler();

	void SetTransfroms(const float2& pos, const float2& size);
	bool CheckRay(const LineSegment& ray);

	float2 GetPos() const;
	float2 GetSize() const;

	bool GetHovered() const;
	bool GetPressed() const;

	void SetActive(bool set);

private:
	float2 pos = float2::zero;
	float2 size = float2::zero;

	bool hovered = false;
	bool pressed = false;

	bool active = true;

	AABB bbox;
};

#endif // !__GUIZMO_H__