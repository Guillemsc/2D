#include "ResourceTextureLoader.h"
#include "App.h"
#include "ModuleProject.h"
#include "ResourceTexture.h"
#include "ModuleResource.h"
#include "ModuleJson.h"

#include "Devil\include\il.h"
#include "Devil\include\ilu.h"
#include "Devil\include\ilut.h"

#pragma comment (lib, "Devil/libx86/DevIL.lib")
#pragma comment (lib, "Devil/libx86/ILU.lib")
#pragma comment (lib, "Devil/libx86/ILUT.lib")

ResourceTextureLoader::ResourceTextureLoader() : ResourceLoader(ResourceType::TEXTURE)
{
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
}

void ResourceTextureLoader::Start()
{
}

void ResourceTextureLoader::CleanUp()
{
	ilShutDown();
}

Resource* ResourceTextureLoader::CreateResource(std::string new_uid)
{
	return new ResourceTexture(new_uid);
}

void ResourceTextureLoader::CreateLibraryFolder()
{
	library_path = App->file_system->CreateFolder(App->resource->GetLibraryPath().c_str(), "textures");
}

bool ResourceTextureLoader::ImportAssetToEngine(DecomposedFilePath d_filepath, std::vector<Resource*>& resources)
{
	bool ret = false;

	ret = ilLoad(IL_TYPE_UNKNOWN, d_filepath.file_path.c_str());

	if (ret)
	{
		// Get texture info
		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);

		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
		{
			iluFlipImage();
		}

		// Convert image to rgb and a byte chain
		ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

		ILubyte* data = ilGetData();
		uint data_size = ilGetInteger(IL_IMAGE_SIZE_OF_DATA);
		uint image_width = ilGetInteger(IL_IMAGE_WIDTH);
		uint image_height = ilGetInteger(IL_IMAGE_HEIGHT);
		uint format = ilGetInteger(IL_IMAGE_FORMAT);

		if (data_size > 0)
		{
			ResourceTexture* r_tex = (ResourceTexture*)App->resource->CreateResource(ResourceType::TEXTURE);

			resources.push_back(r_tex);

			// Crate meta
			std::string assets_meta_path = d_filepath.file_path + ".meta";

			if (App->file_system->FileExists(assets_meta_path.c_str()))
				App->file_system->FileDelete(assets_meta_path.c_str());

			JSON_Doc* doc = App->json->CreateJSON(assets_meta_path.c_str());
			if (doc != nullptr)
			{
				doc->SetString("resource", r_tex->GetUID().c_str());

				doc->Save();
				App->json->UnloadJSON(doc);
			}
		}
		else
			ret = false;

		ilDeleteImages(1, &ImageInfo.Id);
	}

	return ret;
}

bool ResourceTextureLoader::ClearAssetDataFromEngine(DecomposedFilePath decomposed_file_path)
{
	return false;
}

bool ResourceTextureLoader::RenameAsset(DecomposedFilePath decomposed_file_path, const char * new_name)
{
	return false;
}

bool ResourceTextureLoader::IsAssetOnLibrary(DecomposedFilePath decomposed_file_path, std::vector<std::string>& library_files_used)
{
	return false;
}

bool ResourceTextureLoader::ImportResourceFromLibrary(DecomposedFilePath decomposed_file_path)
{
	return false;
}

bool ResourceTextureLoader::ExportResourceToLibrary(Resource * resource)
{
	return false;
}
