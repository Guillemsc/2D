#include "ScriptingObjectCompiler.h"
#include "App.h"
#include "ModuleFileSystem.h"
#include "Functions.h"
#include "ModuleInput.h"
#include "ModuleResource.h"

ScriptingObjectCompiler::ScriptingObjectCompiler() : ScriptingObject()
{

}

ScriptingObjectCompiler::~ScriptingObjectCompiler()
{
}

void ScriptingObjectCompiler::Start()
{
	script_template_filepath = "templates\\c_sharp_script_template\\template_script.cs";

	if (App->scripting->scripting_internal_assembly != nullptr && App->scripting->scripting_internal_assembly->GetAssemblyLoaded())
	{
		ScriptingClass compiler_class;
		if (App->scripting->scripting_internal_assembly->GetClass("BeEngine.Internal", "ScriptCompiler", compiler_class))
		{
			script_compiler_instance = compiler_class.CreateInstance();

			if (script_compiler_instance != nullptr)
			{
				std::vector<ScriptingAssembly*> assemblys = App->scripting->GetScriptingAssemblys();

				std::vector<MonoObject*> assembly_objects;

				for (std::vector<ScriptingAssembly*>::iterator it = assemblys.begin(); it != assemblys.end(); ++it)
				{
					const char* path = (*it)->GetPath();

					MonoObject* obj = (MonoObject*)App->scripting->BoxString(path);

					assembly_objects.push_back(obj);
				}

				MonoArray* assemblys_array = App->scripting->BoxArray(mono_get_string_class(), assembly_objects);

				void *args[1];
				args[0] = assemblys_array;

				MonoObject* ret_obj = nullptr;
				if (script_compiler_instance->InvokeMonoMethod("Init", args, 1, ret_obj))
				{
					bool succes = App->scripting->UnboxBool(ret_obj);

					ready_to_use = succes;
				}
			}
		}
	}


}

void ScriptingObjectCompiler::Update()
{

}

void ScriptingObjectCompiler::CleanUp()
{
	if (script_compiler_instance != nullptr)
	{
		script_compiler_instance->CleanUp();
		RELEASE(script_compiler_instance);
	}
}

bool ScriptingObjectCompiler::AddScript(const char* script_filepath)
{
	bool ret = false;

	if (App->file_system->FileExists(script_filepath))
	{
		if (script_compiler_instance != nullptr)
		{
			if (script_compiler_instance != nullptr)
			{
				MonoObject* path_boxed = (MonoObject*)App->scripting->BoxString(script_filepath);

				void* args[1];
				args[0] = path_boxed;

				MonoObject* ret_obj = nullptr;
				if (script_compiler_instance->InvokeMonoMethod("AddScript", args, 1, ret_obj))
				{
					ret = App->scripting->UnboxBool(ret_obj);
				}
			}
		}
	}

	return ret;
}

bool ScriptingObjectCompiler::RemoveScript(const char* script_filepath)
{
	bool ret = false;
		
	if (script_compiler_instance != nullptr)
	{
		MonoObject* path_boxed = (MonoObject*)App->scripting->BoxString(script_filepath);

		void* args[1];
		args[0] = path_boxed;

		MonoObject* ret_obj = nullptr;
		if (script_compiler_instance->InvokeMonoMethod("RemoveScript", args, 1, ret_obj))
		{
			ret = App->scripting->UnboxBool(ret_obj);
		}
	}
	
	return ret;
}

std::vector<std::string> ScriptingObjectCompiler::GetScripts()
{
	std::vector<std::string> ret;

	if (script_compiler_instance != nullptr)
	{
		MonoObject* ret_obj = nullptr;
		if (script_compiler_instance->InvokeMonoMethod("GetScripts", nullptr, 0, ret_obj))
		{
			std::vector<MonoObject*> objects_ret = App->scripting->UnboxArray(mono_get_string_class(), (MonoArray*)ret_obj);

			for (std::vector<MonoObject*>::iterator it = objects_ret.begin(); it != objects_ret.end(); ++it)
			{
				std::string str = App->scripting->UnboxString((MonoString*)(*it));

				ret.push_back(str);
			}
		}
	}

	return ret;
}

bool ScriptingObjectCompiler::CompileScripts(const std::string& output_assembly_filepath, std::vector<std::string>& compile_errors, 
	std::vector<std::string>& compile_warnings)
{	
	bool ret = false;
	
	if (script_compiler_instance != nullptr)
	{
		if (App->file_system->FileExists(output_assembly_filepath))
			App->file_system->FileDelete(output_assembly_filepath);

		MonoObject* dll_output_boxed = (MonoObject*)App->scripting->BoxString(output_assembly_filepath.c_str());

		void* args[1];
		args[0] = dll_output_boxed;

		MonoObject* ret_obj = nullptr;
		if (script_compiler_instance->InvokeMonoMethod("CompileScripts", args, 1, ret_obj))
		{
			if (ret_obj != nullptr)
			{
				ret = App->scripting->UnboxBool(ret_obj);
			}

			MonoObject* ret_errors = nullptr;
			if (script_compiler_instance->InvokeMonoMethod("GetCompileErrors", nullptr, 0, ret_errors))
			{
				std::vector<MonoObject*> errors_vector = App->scripting->UnboxArray(mono_get_string_class(), (MonoArray*)ret_errors);

				for (std::vector<MonoObject*>::iterator it = errors_vector.begin(); it != errors_vector.end(); ++it)
				{
					std::string error = App->scripting->UnboxString((MonoString*)(*it));

					compile_errors.push_back(error);
				}
			}

			MonoObject* ret_warnings = nullptr;
			if (script_compiler_instance->InvokeMonoMethod("GetCompileWarnings", nullptr, 0, ret_warnings))
			{
				std::vector<MonoObject*> warnings_vector = App->scripting->UnboxArray(mono_get_string_class(), (MonoArray*)ret_warnings);

				for (std::vector<MonoObject*>::iterator it = warnings_vector.begin(); it != warnings_vector.end(); ++it)
				{
					std::string warning = App->scripting->UnboxString((MonoString*)(*it));

					compile_warnings.push_back(warning);
				}
			}
		}
	}
	
	return ret;
}

bool ScriptingObjectCompiler::CreateScript(const char * script_filepath, const char * code)
{
	bool ret = false;
	
	if (script_compiler_instance != nullptr)
	{
		DecomposedFilePath d_filepath = App->file_system->DecomposeFilePath(script_filepath);

		if (App->file_system->FolderExists(d_filepath.path.c_str()))
		{
			if (App->file_system->FileExists(script_filepath))
				App->file_system->FileDelete(script_filepath);

			MonoObject* script_filepath_boxed = (MonoObject*)App->scripting->BoxString(script_filepath);
			MonoObject* code_boxed = (MonoObject*)App->scripting->BoxString(code);

			void* args[2];
			args[0] = script_filepath_boxed;
			args[1] = code_boxed;

			MonoObject* ret_obj = nullptr;
			if (script_compiler_instance->InvokeMonoMethod("CreateCSScriptFile", args, 2, ret_obj))
			{
				ret = App->scripting->UnboxBool(ret_obj);
			}
		}
	}
	
	return ret;
}

std::string ScriptingObjectCompiler::GetScriptCode(const char * script_filepath)
{
	std::string ret = "";
	
	if (script_compiler_instance != nullptr)
	{
		DecomposedFilePath d_filepath = App->file_system->DecomposeFilePath(script_filepath);

		if (App->file_system->FileExists(script_filepath))
		{
			MonoObject* script_filepath_boxed = (MonoObject*)App->scripting->BoxString(script_filepath);

			void* args[1];
			args[0] = script_filepath_boxed;

			MonoObject* ret_obj = nullptr;
			if (script_compiler_instance->InvokeMonoMethod("ReadCSScriptFile", args, 1, ret_obj))
			{
				ret = App->scripting->UnboxString((MonoString*)ret_obj);
			}
		}
	}
	
	return ret;
}

bool ScriptingObjectCompiler::SetScriptCode(const char * script_filepath, std::string code)
{
	bool ret = false;
	
	if (script_compiler_instance != nullptr)
	{
		MonoObject* script_filepath_boxed = (MonoObject*)App->scripting->BoxString(script_filepath);
		MonoObject* script_code_boxed = (MonoObject*)App->scripting->BoxString(code.c_str());

		void* args[2];
		args[0] = script_filepath_boxed;
		args[1] = script_code_boxed;

		MonoObject* ret_obj = nullptr;
		if (script_compiler_instance->InvokeMonoMethod("WriteCSScriptFile", args, 2, ret_obj))
		{
			ret = App->scripting->UnboxBool(ret_obj);
		}
	}
	
	return ret;
}

bool ScriptingObjectCompiler::CreateScriptFromTemplate(const char* save_path, const char* name, std::string& created_asset_filepath)
{
	bool ret = false;

	if (App->file_system->FileExists(script_template_filepath.c_str()))
	{
		std::string resultant_path;
		if (App->file_system->FileCopyPaste(script_template_filepath.c_str(), save_path, false, resultant_path))
		{
			std::string new_filepath;
			if (App->file_system->FileRename(resultant_path.c_str(), name, true, new_filepath))
			{
				DecomposedFilePath dp = App->file_system->DecomposeFilePath(new_filepath.c_str());

				std::string code = GetScriptCode(new_filepath.c_str());

				code = TextReplace(code, "#SCRIPTNAME#", dp.file_name.c_str());

				SetScriptCode(new_filepath.c_str(), code.c_str());

				created_asset_filepath = new_filepath;

				ret = true;
			}
		}
	}

	return ret;
}

bool ScriptingObjectCompiler::ClassIsSubclassOf(const char * class_to_check, const char * class_parent)
{
	bool ret = false;

	if (script_compiler_instance != nullptr)
	{
		MonoObject* class_to_check_boxed = (MonoObject*)App->scripting->BoxString(class_to_check);
		MonoObject* class_parent_boxed = (MonoObject*)App->scripting->BoxString(class_parent);

		void* args[2];
		args[0] = class_to_check_boxed;
		args[1] = class_parent_boxed;

		MonoObject* ret_obj = nullptr;
		if (script_compiler_instance->InvokeMonoMethod("ClassIsSubclassOf", args, 2, ret_obj))
		{
			ret = App->scripting->UnboxBool(ret_obj);
		}
	}

	return ret;
}
