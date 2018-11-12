#ifndef __GAME_OBJECT_COMPONENT_H__
#define __GAME_OBJECT_COMPONENT_H__

#include <vector>

#include "Globals.h"

class GameObject;

enum ComponentType
{
	TRANSFORM,
};

enum ComponentGroup
{
	TRANSFORMATIONS,
	RENDERING,
};

class GameObjectComponent
{
	friend class GameObject;

private:
	void operator delete(void *) {}

public:
	GameObjectComponent(std::string name, const ComponentType& type, const ComponentGroup& group, bool unique_per_game_object = false, bool can_destroy = true);
	~GameObjectComponent() {};

	virtual void OnEditorDraw() {};

	virtual void Start() {};
	virtual void CleanUp() {};

	virtual void OnChildAdded(GameObject* child) {};
	virtual void OnChildRemoved(GameObject* child) {};
	virtual void OnParentChanged(GameObject* new_parent) {};

	ComponentType GetType() const;
	std::string GetName() const;

protected:
	GameObject* owner = nullptr;

private:
	std::string name;
	ComponentType type;
	ComponentGroup group;
	bool can_destroy = true;
	bool unique_per_game_object = false;
};

#endif // !__GAME_OBJECT_COMPONENT_H__