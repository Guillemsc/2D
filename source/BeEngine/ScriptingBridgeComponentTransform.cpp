#include "ScriptingBridgeComponentTransform.h"
#include "App.h"
#include "ScriptingCluster.h"
#include "GameObject.h"
#include "ScriptingBridgeGameObject.h"
#include "ComponentTransfrom.h"
#include "ScriptingBridgeBeObject.h"

#include "mmgr\nommgr.h"
#include "mmgr\mmgr.h"

ScriptingBridgeComponentTransform::ScriptingBridgeComponentTransform(ComponentTransform * component_transform)
	: ScriptingBridgeObject(App->scripting->scripting_cluster->component_transform_class)
{
	component_transform_ref = component_transform;
}

ScriptingBridgeComponentTransform::~ScriptingBridgeComponentTransform()
{
}

void ScriptingBridgeComponentTransform::Start()
{
}

void ScriptingBridgeComponentTransform::OnRebuildInstances()
{
	if (class_instance != nullptr)
	{
		ScriptingBridgeBeObject::SetBeObjectRefPointer(class_instance->GetMonoObject(), component_transform_ref);

		ScriptingBridgeGameObject* bridge_go = (ScriptingBridgeGameObject*)component_transform_ref->GetOwner()->GetScriptingBridge();
		MonoObject* owner_go_mono_object = bridge_go->GetInstance()->GetMonoObject();

		void* args2[1] = { owner_go_mono_object };

		MonoObject* ret_obj2 = nullptr;
		class_instance->InvokeMonoMethodOnParentClass(
			*App->scripting->scripting_cluster->component_class, "SetOwner", args2, 1, ret_obj2);

		bridge_go->SetComponentTransform(component_transform_ref);
	}
}

void ScriptingBridgeComponentTransform::CleanUp()
{
	if (class_instance != nullptr)
	{
		ScriptingBridgeBeObject::ClearBeObjectRefPointer(class_instance->GetMonoObject());
	}
}

void ScriptingBridgeComponentTransform::SetPosition(MonoObject * mono_object, MonoObject * mono_float2)
{
	ComponentTransform* component_trans = (ComponentTransform*)ScriptingBridgeBeObject::GetBeObjectRefPointer(mono_object);

	if (component_trans != nullptr)
	{
		float2 new_pos = App->scripting->scripting_cluster->UnboxFloat2(mono_float2);

		component_trans->SetPosition(new_pos);
	}
}

MonoObject * ScriptingBridgeComponentTransform::GetPosition(MonoObject * mono_object)
{
	MonoObject* ret = nullptr;

	ComponentTransform* component_trans = (ComponentTransform*)ScriptingBridgeBeObject::GetBeObjectRefPointer(mono_object);

	if (component_trans != nullptr)
	{
		ret = App->scripting->scripting_cluster->BoxFloat2(component_trans->GetPosition());
	}

	return ret;
}

void ScriptingBridgeComponentTransform::SetRotationDegrees(MonoObject * mono_object, float value)
{
	ComponentTransform* component_trans = (ComponentTransform*)ScriptingBridgeBeObject::GetBeObjectRefPointer(mono_object);

	if (component_trans != nullptr)
	{
		component_trans->SetRotationDegrees(value);
	}
}

float ScriptingBridgeComponentTransform::GetRotationDegrees(MonoObject * mono_object)
{
	float ret = 0.0f;

	ComponentTransform* component_trans = (ComponentTransform*)ScriptingBridgeBeObject::GetBeObjectRefPointer(mono_object);

	if (component_trans != nullptr)
	{
		ret = component_trans->GetRotationDegrees();
	}

	return ret;
}

void ScriptingBridgeComponentTransform::SetScale(MonoObject * mono_object, MonoObject * mono_float2)
{
	ComponentTransform* component_trans = (ComponentTransform*)ScriptingBridgeBeObject::GetBeObjectRefPointer(mono_object);

	if (component_trans != nullptr)
	{
		float2 new_scal = App->scripting->scripting_cluster->UnboxFloat2(mono_float2);

		component_trans->SetScale(new_scal);
	}
}

MonoObject * ScriptingBridgeComponentTransform::GetScale(MonoObject * mono_object)
{
	MonoObject* ret = nullptr;

	ComponentTransform* component_trans = (ComponentTransform*)ScriptingBridgeBeObject::GetBeObjectRefPointer(mono_object);

	if (component_trans != nullptr)
	{
		ret = App->scripting->scripting_cluster->BoxFloat2(component_trans->GetScale());
	}

	return ret;
}
