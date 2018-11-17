#ifndef __MODULE_RESOURCE_H__
#define __MODULE_RESOURCE_H__

#include "Devil\include\il.h"
#include "Devil\include\ilu.h"
#include "Devil\include\ilut.h"

#include <map>

#include "Module.h"
#include "Resource.h"
#include "ModuleTimeSlicedTask.h"

class Event;

class ModuleResource : public Module
{
public:
	ModuleResource();
	~ModuleResource();

	bool Awake();
	bool Start();
	bool PreUpdate();
	bool Update();
	bool PostUpdate();
	bool CleanUp();

	void OnEvent(Event* ev);

	std::string GetNewUID();

	void SetCurrentAssetsPath(const char* path);
	std::string GetCurrentAssetsPath();
	std::string GetLibraryPath();
	std::string GetAssetsPath();

	std::string GetLibraryPathFromResourceType(const ResourceType& type);

	Resource* CreateResource(const ResourceType type);
	Resource* GetResourceFromAssetFile(const char* filepath);

	bool LoadFileToEngine(const char* filepath);
	bool ExportAssetToLibrary(const char* filepath);

	void StartWatchingFolders();
	void StopWatchingFolders();

private:
	void AddAssetExtension(const ResourceType& type, const char* extension);
	void AddLibraryExtension(const ResourceType& type, const char* extension);

	ResourceType GetResourceTypeFromAssetExtension(const char* extension);
	ResourceType GetResourceTypeFromLibraryExtension(const char* extension);

	void AddLibraryName(const ResourceType& type, const char* name);

	void CreateLibraryFolders();

	void DestroyAllResources();

private:
	std::string current_assets_folder;
	std::string assets_folder;
	std::string library_folder;

	std::map<ResourceType, std::vector<std::string>> asset_extensions;
	std::map<ResourceType, std::vector<std::string>> library_extensions;
	std::map<ResourceType, std::string> library_names;

	std::vector<Resource*> resources;

	int watching_folder_index = 0;
};


#endif // !__MODULE_RESOURCE_H__