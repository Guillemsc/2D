#ifndef __RESOURCE_SCRIPT_H__
#define __RESOURCE_SCRIPT_H__

#include "Resource.h"
#include "Globals.h"
#include <vector>
#include "ModuleScripting.h"
#include "ScriptingCluster.h"

class ResourceScript;
class GameObject;
class GameObjectComponent;

class ResourceScriptField
{
	friend class ResourceScript;

public:
	ResourceScriptField();

	ScriptFieldType type = ScriptFieldType::SCRIPT_FIELD_UNDEFINED;
	std::string field_name;
};

class ResourceScriptFieldValue
{
	friend class ResourceScript;

public:
	ResourceScriptFieldValue(const ResourceScriptField& field);

	std::string GetFieldName() const;
	ScriptFieldType GetFieldType() const;

public:
	int int_field = 0;
	float float_field = 0.0f;
	std::string string_field = "";
	bool bool_field = false;
	//GameObject* game_object_field = nullptr;
	//GameObjectComponent* game_object_class_field = nullptr;
	//Resource* resource_field = nullptr;

private:
	ScriptFieldType field;
};

class ResourceScript : public Resource
{
	friend class ResourceScriptLoader;
	friend class ModuleResource;
	friend class ModuleAssets;

private:
	void operator delete(void *) {}

public:
	ResourceScript();

	void CleanUp();

	bool ExistsOnLibrary(std::string uid, std::string& library_filepath = std::string());
	void ExportToLibrary(std::string uid);
	void ImportFromLibrary();

	void OnRemoveAsset();
	void OnRenameAsset(const char* new_name, const char* last_name);
	void OnMoveAsset(const char* new_asset_path, const char* last_asset_path);

	void ClearScriptFields();
	void AddScriptField(const std::string& field_name, const std::string& type_name);
	std::vector<ResourceScriptField> GetFields();

	bool GetInheritsFromBeengineScript() const;
	ScriptingClass GetScriptingClass() const;

	bool DrawEditorExplorer();

private:
	bool compiles = false;

	std::vector<std::string> compile_errors;

	bool inherits_from_component_script = false;

	std::vector<ResourceScriptField> script_fields;

	ScriptingClass script_class;
};

#endif // !__RESOURCE_SCRIPT_H__