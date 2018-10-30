#ifndef __RESOURCE_LOADER_H__
#define __RESOURCE_LOADER_H__

#include <map>
#include <vector>

#include "Resource.h"
#include "ModuleFileSystem.h"

class ResourceLoader 
{
public:
	ResourceLoader(ResourceType resources_to_load_type, std::string resources_to_load_name);

	virtual void Start() {};
	virtual void CleanUp() {};

	ResourceType GetResourcesToLoadType() const;
	std::string GetResourcesToLoadName() const;

	void AddAssetExtensionToLoad(const char* extension);
	void AddLibraryExtensionToLoad(const char* extension);

	bool CanLoadExtensionAsset(const char* extension);
	bool CanLoadExtensionLibrary(const char* extension);

	void AddResource(Resource* resource);
	bool DestroyResource(std::string unique_id);
	bool DestroyResource(Resource* res);
	void DestroyAllResources();
	Resource* GetResource(std::string unique_id);
	std::map<std::string, Resource*> GetResources() const;

	virtual Resource* CreateResource(std::string new_uid) { return nullptr; };

	std::string GetLibraryPath();

	virtual void CreateLibraryFolder() {};

	// Exports an asset file into the library folder
	virtual bool ExportAssetToLibrary(DecomposedFilePath decomposed_file_path, std::string new_uid) { return false; };

	// Unloads all the asset engine files
	virtual bool ClearAssetDataFromEngine(DecomposedFilePath decomposed_file_path) { return false; };

	// Deletes all resources related to an assets
	virtual bool DeleteAssetResources(DecomposedFilePath decomposed_file_path) { return false; };

	// Renames the library resource and all other necessary files
	virtual bool RenameAsset(DecomposedFilePath decomposed_file_path, const char* new_name) { return false; };

	// Returns true if the resource can be found on the library folder
	virtual bool IsAssetOnLibrary(DecomposedFilePath decomposed_file_path, std::vector<std::string>& library_files_used) { return false; };

	// Imports the resource into memory from the library folder
	virtual bool ImportAssetFromLibrary(DecomposedFilePath decomposed_file_path, std::vector<Resource*>& resources = std::vector<Resource*>()) { return false; };

	// Exports resource into the library folder
	virtual bool ExportResourceToLibrary(Resource* resource) { return false; };

private:
	virtual void OnDestroyResource(Resource* res) {};

protected:
	ResourceType resources_to_load_type;
	std::string  resources_to_load_name;

	std::map<std::string, Resource*> resources;

	std::vector<std::string> assets_extensions_to_load;
	std::vector<std::string> library_extensions_to_load;

	std::string library_path;
};

#endif // !__RESOURCE_LOADER_H__