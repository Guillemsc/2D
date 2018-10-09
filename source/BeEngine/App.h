#pragma once

#include <list>
#include <string>
#include "Globals.h"
#include "Profiler.h"
#include "Module.h"

class ModuleJson;
class ModuleWindow;
class ModuleInput;
class ModuleAudio;
class ModuleRenderer3D;
class ModuleCamera3D;
class ModuleShader;

class Profiler;
class JSON_Doc;

class Application
{
public:
	Application(int argc, char* args[]);
	~Application();

	bool Awake();
	bool Start();
	bool Update();
	bool CleanUp();

	void CreateProfiles();

	int GetArgc() const;
	const char* GetArgv(int index) const;

	void LoadConfig();
	void SaveConfig(Module* module = nullptr);

	void Quit();

	void SetAppName(const char* name);
	const char* GetAppName() const;
	void SetAppOrganization(const char* name);
	const char* GetAppOrganization();
	void SetVersion(const char* version);
	const char* GetVersion() const;
	SDL_version GetSDLVersion();
	const char* GetBasePath();

	float GetDT() const;

	void SetMaxFps(int const &set);
	int GetMaxFps() const;

	void OpenBrowser(const char* url);
	void OpenFolder(const char* folder);

	void AddInternalLog(const char* log);
	std::vector<const char*> GetInternalLogs() const;
	void ClearInternalLogs();
	void AddConsoleLog(ConsoleLogLine text);
	std::list<ConsoleLogLine> GetConsoleLogs() const;
	void ClearConsoleLogs();

private:
	void AddModule(Module* mod, const char* name);

public:
	//Modules
	ModuleJson *	   json	= nullptr;
	ModuleWindow*      window = nullptr;
	ModuleInput*       input = nullptr;
	ModuleAudio*       audio = nullptr;
	ModuleRenderer3D*  renderer3D = nullptr;
	ModuleCamera3D*    camera = nullptr;
	ModuleShader*	   shader = nullptr;

	Profiler*		   profiler = nullptr;

private:
	int				   argc;
	char**			   args;

	std::string		   title = "";
	std::string		   organization = "";
	std::string		   version = "";

	std::list<Module*> modules;

	std::vector<const char*>  internal_logs;
	std::list<ConsoleLogLine> console_logs;

	bool		       to_quit = false;

	JSON_Doc*          config = nullptr;

	// Profiler
	Profile* prof_app_awake = nullptr;
	Profile* prof_app_start = nullptr;
	Profile* prof_app_total_update = nullptr;
	Profile* prof_app_preupdate = nullptr;
	Profile* prof_app_update = nullptr;
	Profile* prof_app_postupdate = nullptr;
};

extern Application* App;