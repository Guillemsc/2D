#ifndef __RESOURCE_TEXTURE_H__
#define __RESOURCE_TEXTURE_H__

#include "Resource.h"
#include "Globals.h"

class ResourceTexture : public Resource
{
	friend class ModuleResource;

private:
	void operator delete(void *) {}

public:
	ResourceTexture();

	void CleanUp();

	bool OnExistsOnLibrary(std::string uid, std::string& library_filepath = std::string());
	void OnExportToLibrary(std::string uid);
	void OnImportFromLibrary();

	uint GetTextureId();

	uint GetDataId() const;
	byte* GetData() const;
	uint GetDataSize() const;

private:
	byte*  texture_data = nullptr;
	uint   texture_data_size = 0;
	uint   texture_data_id = 0;

	uint   texture_id = 0;

	bool   flipped = false;
};

#endif // !__RESOURCE_TEXTURE_H__