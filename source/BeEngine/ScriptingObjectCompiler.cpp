#include "ScriptingObjectCompiler.h"
#include "App.h"
#include "ModuleFileSystem.h"

ScriptingObjectCompiler::ScriptingObjectCompiler() : ScriptingObject()
{

}

ScriptingObjectCompiler::~ScriptingObjectCompiler()
{
}

void ScriptingObjectCompiler::Start()
{
	if (App->scripting->scripting_internal_assembly != nullptr && App->scripting->scripting_internal_assembly->GetAssemblyLoaded())
	{
		ScriptingClass compiler_class = App->scripting->scripting_internal_assembly->GetClass("BeEngine.Internal", "ScriptCompiler");
	
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
			script_compiler_instance->InvokeMonoMethod("Init", args, 1, ret_obj);

			bool succes = App->scripting->UnboxBool(ret_obj);

			ready_to_use = succes;
		}
	}
}

void ScriptingObjectCompiler::CleanUp()
{
	if (script_compiler_instance != nullptr)
	{
		script_compiler_instance->CleanUp();
		RELEASE(script_compiler_instance);
	}
}

bool ScriptingObjectCompiler::CompileScript(const char * script_filepath, const char* dll_output_path, std::vector<std::string>& compile_errors)
{	
	bool ret = false;

	if (App->scripting->scripting_internal_assembly != nullptr && App->scripting->scripting_internal_assembly->GetAssemblyLoaded())
	{
		if (script_compiler_instance != nullptr)
		{
			MonoObject* script_path_boxed = (MonoObject*)App->scripting->BoxString(script_filepath);
			MonoObject* dll_output_boxed = (MonoObject*)App->scripting->BoxString(dll_output_path);

			void* args[2];
			args[0] = script_path_boxed;
			args[1] = dll_output_boxed;

			MonoObject* ret_obj = nullptr;
			if (script_compiler_instance->InvokeMonoMethod("CompileScript", args, 2, ret_obj))
			{
				if (ret_obj != nullptr)
				{
					std::vector<MonoObject*> rest_vector = App->scripting->UnboxArray(mono_get_string_class(), (MonoArray*)ret_obj);

					for (std::vector<MonoObject*>::iterator it = rest_vector.begin(); it != rest_vector.end(); ++it)
					{
						std::string error = App->scripting->UnboxString((MonoString*)(*it));

						compile_errors.push_back(error);
					}

					ret = rest_vector.size() == 0;
				}
			}
		}
	}

	return ret;
}

bool ScriptingObjectCompiler::CreateScript(const char * script_filepath, const char * code)
{
	bool ret = false;

	if (App->scripting->scripting_internal_assembly != nullptr && App->scripting->scripting_internal_assembly->GetAssemblyLoaded())
	{
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
	}

	return ret;
}

std::string ScriptingObjectCompiler::GetScriptCode(const char * script_filepath)
{
	std::string ret = "";

	if (App->scripting->scripting_internal_assembly != nullptr && App->scripting->scripting_internal_assembly->GetAssemblyLoaded())
	{
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
	}

	return ret;
}
