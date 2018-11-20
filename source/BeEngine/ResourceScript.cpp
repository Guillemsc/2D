#include "ResourceScript.h"
#include "App.h"
#include "ModuleResource.h"
#include "ModuleFileSystem.h"
#include "ModuleScripting.h"
#include "ScriptingObjectCompiler.h"
#include "ScriptingObjectSolutionManager.h"
#include "Functions.h"
#include "ModuleJson.h"

ResourceScript::ResourceScript() : Resource(ResourceType::SCRIPT)
{
}

void ResourceScript::CleanUp()
{

}

bool ResourceScript::ExistsOnLibrary(std::string uid, std::string & library_filepath)
{
	bool ret = false;

	return true;

	return ret;
}

void ResourceScript::ExportToLibrary(std::string uid)
{
	App->scripting->compiler->AddScript(GetAssetFilepath().c_str());
	App->scripting->solution_manager->AddScript(GetAssetFilepath().c_str());
}

void ResourceScript::ImportFromLibrary()
{
	App->scripting->solution_manager->AddScript(GetAssetFilepath().c_str());
	App->scripting->compiler->AddScript(GetAssetFilepath().c_str());
}

void ResourceScript::OnRemoveAsset()
{
	App->scripting->solution_manager->RemoveScript(GetAssetFilepath().c_str());
	App->scripting->compiler->RemoveScript(GetLibraryFilepath().c_str());
}

void ResourceScript::OnRenameAsset(const char * new_name, const char * last_name)
{
	std::string code = App->scripting->compiler->GetScriptCode(GetAssetFilepath().c_str());

	code = TextReplace(code, last_name, new_name);

	App->scripting->compiler->SetScriptCode(GetAssetFilepath().c_str(), code.c_str());

	DecomposedFilePath dfp = App->file_system->DecomposeFilePath(GetAssetFilepath().c_str());
	std::string last_path = dfp.path + last_name + "." + dfp.file_extension;

	App->scripting->compiler->AddScript(GetAssetFilepath().c_str());
	App->scripting->compiler->RemoveScript(last_path.c_str());

	App->scripting->solution_manager->RemoveScript(last_path.c_str());
	App->scripting->solution_manager->AddScript(GetAssetFilepath().c_str());
}
