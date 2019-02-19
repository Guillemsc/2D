#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include <vector>

#include "Globals.h"
#include "GameObjectComponent.h"

class DataAbstraction;
class ComponentTransform;
class ScriptingBridgeGameObject;
class ResourcePrefab;
class Event;
class Scene;

class GameObject 
{
	friend class ModuleGameObject;
	friend class ModuleScene;
	friend class ModuleAssets;
	friend class ScriptingItemGameObject;
	friend class ResourcePrefab;
	friend class Scene;

public:
	GameObject(const std::string& uid);
	~GameObject() {};

	void OnSaveAbstraction(DataAbstraction& abs);
	void OnLoadAbstraction(DataAbstraction& abs);

	void Start();
	void Update();
	void CleanUp();
	void OnEvent(Event* ev);

	void SetName(const char* set);
	std::string GetName();

	std::string GetUID();

	Scene* GetScene() const;

	void SetParent(GameObject* set); 
	void RemoveParent();
	GameObject* GetParent() const;
	uint GetChildsCount() const;
	GameObject* GetChild(uint index) const;
	bool IsChild(GameObject* go);
	bool IsInChildTree(GameObject* go);
	std::vector<GameObject*> GetChilds() const;
	uint GetChildDeepness();

	GameObjectComponent* CreateComponent(const ComponentType& type);
	void DestroyComponent(GameObjectComponent* component, bool check_can_destroy = true);
	bool GetHasComponent(const ComponentType& type) const;
	std::vector<GameObjectComponent*> GetComponents() const;

	bool GetSelected() const;

	bool GetHasPrefab() const;
	ResourcePrefab* GetPrefab() const;

	ScriptingBridgeGameObject* GetScriptingBridge() const;

private:
	void DestroyAllComponentsNow();
	void ActuallyDestroyComponents();

public:
	ComponentTransform* transform = nullptr;

private:
	GameObject* parent = nullptr;
	std::vector<GameObject*> childs;

	Scene* scene = nullptr;

	std::vector<GameObjectComponent*> components;
	std::vector<GameObjectComponent*> components_to_destroy;

	std::string name;
	std::string uid;

	ResourcePrefab* resource_prefab = nullptr;

	bool selected = false;

	ScriptingBridgeGameObject* scripting_bridge = nullptr;
};

#endif // !__GAME_OBJECT_H__