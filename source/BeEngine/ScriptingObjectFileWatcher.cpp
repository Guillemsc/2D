#include "ScriptingObjectFileWatcher.h"
#include "App.h"
#include "ModuleFileSystem.h"
#include "ModuleEvent.h"

ScriptingObjectFileWatcher::ScriptingObjectFileWatcher()
{
}

ScriptingObjectFileWatcher::~ScriptingObjectFileWatcher()
{
}

void ScriptingObjectFileWatcher::RegisterInternalCalls()
{
	//mono_add_internal_call("BeEngine.Internal.FileWatcher::FileFolderChangedCallback", this->Start);
}

void ScriptingObjectFileWatcher::Start()
{
	if (App->scripting->scripting_internal_assembly != nullptr && App->scripting->scripting_internal_assembly->GetAssemblyLoaded())
	{
		ScriptingClass compiler_class;
		if (App->scripting->scripting_internal_assembly->GetClass("BeEngine.Internal", "FileWatcher", compiler_class))
		{
			script_file_watcher_instance = compiler_class.CreateInstance();

			if (script_file_watcher_instance != nullptr)
			{
				ready_to_use = true;
			}
		}
	}
}

void ScriptingObjectFileWatcher::Update()
{
	GetChangesStack();
}

void ScriptingObjectFileWatcher::CleanUp()
{
	if (script_file_watcher_instance != nullptr)
	{
		script_file_watcher_instance->CleanUp();
		RELEASE(script_file_watcher_instance);
	}
}

bool ScriptingObjectFileWatcher::Watch(const char * path)
{
	bool ret = false;

	if (script_file_watcher_instance != nullptr)
	{
		if (App->file_system->FolderExists(path) || App->file_system->FileExists(path))
		{
			MonoObject* boxed_path = (MonoObject*)App->scripting->BoxString(path);

			void *args[1];
			args[0] = boxed_path;

			MonoObject* ret_obj = nullptr;
			script_file_watcher_instance->InvokeMonoMethod("Watch", args, 1, ret_obj);

			ret = App->scripting->UnboxBool(ret_obj);
		}
	}

	return ret;
}

bool ScriptingObjectFileWatcher::StopWatch(const char * path)
{
	bool ret = false;

	if (script_file_watcher_instance != nullptr)
	{
		if (App->file_system->FolderExists(path) || App->file_system->FileExists(path))
		{
			MonoObject* boxed_path = (MonoObject*)App->scripting->BoxString(path);

			void *args[1];
			args[0] = boxed_path;

			MonoObject* ret_obj = nullptr;
			script_file_watcher_instance->InvokeMonoMethod("StopWatch", args, 1, ret_obj);

			ret = true;
		}
	}

	return ret;
}

void ScriptingObjectFileWatcher::SetRiseEvents(bool set)
{
	if (script_file_watcher_instance != nullptr)
	{		
		if (set)
		{
			MonoObject* ret_obj = nullptr;
			script_file_watcher_instance->InvokeMonoMethod("StartRisingEvents", nullptr, 0, ret_obj);
		}
		else
		{
			MonoObject* ret_obj = nullptr;
			script_file_watcher_instance->InvokeMonoMethod("StopRisingEvents", nullptr, 0, ret_obj);

			std::string response = App->scripting->UnboxString((MonoString*)ret_obj);

			int i = 0;
		}
	}
}

void ScriptingObjectFileWatcher::GetChangesStack()
{
	if (script_file_watcher_instance != nullptr)
	{
		MonoObject* ret_obj = nullptr;
		script_file_watcher_instance->InvokeMonoMethod("GetChangesStack", nullptr, 0, ret_obj);

		std::vector<MonoObject*> unboxed = App->scripting->UnboxArray((MonoArray*)ret_obj);

		for (std::vector<MonoObject*>::iterator it = unboxed.begin(); it != unboxed.end(); ++it)
		{
			std::string file = App->scripting->UnboxString((MonoString*)(*it));

			EventWatchFileFolderChanged* new_ev = new EventWatchFileFolderChanged(file);
			App->event->SendEvent(new_ev);
		}
	}
}
