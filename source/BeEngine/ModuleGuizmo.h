#ifndef __MODULE_GUIZMO_H__
#define __MODULE_GUIZMO_H__

#include "Module.h"
#include "Guizmo.h"

class PositionGuizmo;
class PhysicsPolygonGuizmo;
class CanvasItemGuizmo;

class ModuleGuizmo : public Module
{
public:
	ModuleGuizmo();
	~ModuleGuizmo();

	bool Awake();
	bool Start();
	bool PreUpdate();
	bool Update();
	bool PostUpdate();
	bool CleanUp();

	void OnLoadConfig(JSON_Doc* config);
	void OnSaveConfig(JSON_Doc* config);

	void RenderGuizmos();
	void RenderGameObjectGuizmos(float relative_size);
	void RenderAllPhysicsGuizmos(float relative_size);
	void RenderAllUIHandlerGuizmos(float relative_size);

	void SetRenderHandlers(bool set);
	bool GetRenderHandlers() const;

	void SetRenderAllPhysicsGuizmos(bool set);
	bool GetRenderAllPhysicsGuizmos() const;

	void SetRenderUIHandlers(bool set);
	bool GetRenderUIHandlers() const;

private:
	Guizmo* AddGuizmo(Guizmo* add);
	void DestroyAllGuizmos();

public:
	PositionGuizmo* position_guizmo = nullptr;
	PhysicsPolygonGuizmo* physics_polygon_guizmo = nullptr;
	CanvasItemGuizmo* canvas_item_guizmo = nullptr;

private:
	std::vector<Guizmo*> guizmos;

	bool render_handlers = false;
	bool render_ui_handlers = false;
	bool render_all_physics_guizmos = false;
};

#endif // !__MODULE_GUIZMO_H__