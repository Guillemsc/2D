#ifndef __SCRIPTING_BRIDGE_COMPONENT_BUTTON_H__
#define __SCRIPTING_BRIDGE_COMPONENT_BUTTON_H__

#include "ScriptingBridgeObject.h"
#include "ModuleScripting.h"

class GameObject;
class GameObjectComponent;
class ComponentButton;
class Event;

class ScriptingBridgeComponentButton : public ScriptingBridgeObject
{
	friend class ScriptingCluster;
	friend class ModuleScripting;
	friend class ComponentButton;

private:
	void operator delete(void *) {}

public:
	ScriptingBridgeComponentButton(ComponentButton* component_button_ref);
	~ScriptingBridgeComponentButton();

	void Start();
	void OnRebuildInstances();
	void CleanUp();

	void CallOnClick();

private:
	//static GameObjectComponent* CreateGameObjectComponentFromComponentType(GameObject* go, MonoType* type);

	// Internal Calls

	//static void AddComponent(MonoObject* mono_object, MonoType* component_type);

	// --------------

private:
	ComponentButton* component_button_ref = nullptr;
};

#endif // !__SCRIPTING_BRIDGE_COMPONENT_BUTTON_H__