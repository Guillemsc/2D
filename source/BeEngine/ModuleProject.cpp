#include "ModuleProject.h"
#include "App.h"
#include "ModuleFileSystem.h"
#include "ModuleJson.h"
#include "ModuleWindow.h"
#include "ModuleEvent.h"
#include "Event.h"

ModuleProject::ModuleProject()
{
	projects_json_filepath = App->GetBasePath() + std::string("Projects.json");
}

ModuleProject::~ModuleProject()
{
}

bool ModuleProject::Awake()
{
	bool ret = true;

	App->event->Suscribe(std::bind(&ModuleProject::OnEvent, this, std::placeholders::_1), EventType::THREAD_TASK_FINISHED);

	App->window->GetWindowNamer()->AddNamePart("project_name", "");

	//task = new LoadProjectsThreadTask(this);
	//App->thread->StartThread(task);

	LoadProjects();

	return ret;
}

bool ModuleProject::Start()
{
	bool ret = true;

	return ret;
}

bool ModuleProject::PreUpdate()
{
	bool ret = true;

	return ret;
}

bool ModuleProject::Update()
{
	bool ret = true;

	return ret;
}

bool ModuleProject::PostUpdate()
{
	bool ret = true;

	return ret;
}

bool ModuleProject::CleanUp()
{
	bool ret = true;

	return ret;
}

std::string ModuleProject::GetCurrProjectBasePath() const
{
	std::string ret;

	if (curr_project != nullptr)
		ret = curr_project->GetPath();
	
	return ret;
}

bool ModuleProject::CreateNewProject(const char* path, const char * name)
{
	bool ret = false;

	if (App->file_system->FolderExists(path))
	{
		std::string full_path = path + std::string(name) + "\\";

		if (!App->file_system->FolderExists(full_path.c_str()))
		{
			full_path = App->file_system->CreateFolder(path, name);

			Project* proj = new Project();

			proj->SetPath(full_path.c_str());
			proj->SetName(name);

			projects.push_back(proj);

			JSON_Doc* doc = App->json->CreateJSON(full_path.c_str(), "project", "beproject");

			if (doc != nullptr)
			{
				doc->SetString("name", name);

				doc->Save();

				ret = true;
			}

			SerializeProjects();
		}
	}

	return ret;
}

bool ModuleProject::LoadProject(const char * project_folder)
{
	bool ret = false;

	if (!ProjectExists(project_folder))
	{
		std::string filepath = project_folder + std::string("project.beproject");

		JSON_Doc* doc = App->json->LoadJSON(filepath.c_str());

		if (doc != nullptr)
		{
			std::string name = doc->GetString("name");

			Project* proj = new Project();
			proj->SetPath(project_folder);
			proj->SetName(name.c_str());

			projects.push_back(proj);

			ret = true;
		}

		SerializeProjects();
	}

	return ret;
}

bool ModuleProject::RemoveProject(const char * project_folder)
{
	bool ret = false;

	std::string path = project_folder;

	for (std::vector<Project*>::iterator it = projects.begin(); it != projects.end(); ++it)
	{
		if ((*it)->GetPath().compare(path) == 0)
		{
			ret = true;
			projects.erase(it);
			break;
		}
	}

	SerializeProjects();

	return ret;
}

void ModuleProject::SetCurrProject(Project * set)
{
	curr_project = set;

	if (curr_project != nullptr)
	{
		App->window->GetWindowNamer()->UpdateNamePart("project_name", curr_project->GetName());
	}
}

bool ModuleProject::GetProjectsLoaded() const
{
	return projects_loaded;
}

std::vector<Project*> ModuleProject::GetProjects() const
{
	return projects;
}

void ModuleProject::LoadProjects()
{
	JSON_Doc* doc = App->json->LoadJSON(projects_json_filepath.c_str());

	if (doc == nullptr)
		doc = App->json->CreateJSON(projects_json_filepath.c_str());

	if (doc != nullptr)
	{
		int projects_count = doc->GetNumber("projects_count");

		for (int i = 0; i < projects_count; ++i)
		{
			JSON_Doc section_node = doc->GetNode();

			std::string proj_section = "project_" + std::to_string(i);

			if (section_node.MoveToSection(proj_section))
			{
				std::string name = section_node.GetString("name");
				std::string path = section_node.GetString("path");

				if (App->file_system->FolderExists(path.c_str()))
				{
					std::string project_doc_path = path + "project.beproject";
					if (App->file_system->FileExists(project_doc_path.c_str()))
					{
						Project* proj = new Project();
						proj->SetName(name.c_str());
						proj->SetPath(path.c_str());

						projects.push_back(proj);
					}
				}
			}
		}

		App->json->UnloadJSON(doc);

		SerializeProjects();
	}
}

void ModuleProject::SerializeProjects()
{
	JSON_Doc* doc = App->json->LoadJSON(projects_json_filepath.c_str());

	if (doc == nullptr)
		doc = App->json->CreateJSON(projects_json_filepath.c_str());

	if (doc != nullptr)
	{
		doc->Clear();

		doc->SetNumber("projects_count", projects.size());

		int count = 0;
		for (std::vector<Project*>::iterator it = projects.begin(); it != projects.end(); ++it)
		{
			JSON_Doc section_node = doc->GetNode();

			std::string proj_section = "project_" + std::to_string(count);
			section_node.AddSection(proj_section);

			if (section_node.MoveToSection(proj_section))
			{
				section_node.SetString("name", (*it)->GetName().c_str());
				section_node.SetString("path", (*it)->GetPath().c_str());
			}

			++count;
		}

		doc->Save();

		App->json->UnloadJSON(doc);
	}
}

bool ModuleProject::ProjectExists(const char* project_path)
{
	bool ret = false;

	std::string path = project_path;

	for (std::vector<Project*>::iterator it = projects.begin(); it != projects.end(); ++it)
	{
		if ((*it)->GetPath().compare(path) == 0)
		{
			ret = true;
			break;
		}
	}

	return ret;
}

void ModuleProject::OnEvent(Event* ev)
{
	if (ev->GetType() == EventType::THREAD_TASK_FINISHED)
	{
		EventThreadTaskFinished* th_event = (EventThreadTaskFinished*)ev;

		if (task == th_event->GetTask())
		{
			projects_loaded = true;
		}
	}
}

Project::Project()
{
}

Project::~Project()
{
}

void Project::SetPath(const char* _path)
{
	path = _path;
}

void Project::SetName(const char* _name)
{
	name = _name;
}

void Project::SetLastTimeOpened(const char * _date)
{
	last_time_opened = _date;
}

std::string Project::GetPath() const
{
	return path;
}

std::string Project::GetName() const
{
	return name;
}

std::string Project::GetLastTimeOpened() const
{
	return last_time_opened;
}

LoadProjectsThreadTask::LoadProjectsThreadTask(ModuleProject * module)
{
	module_proj = module;
}

void LoadProjectsThreadTask::Start()
{
}

void LoadProjectsThreadTask::Update()
{
	module_proj->projects_json_filepath = App->GetBasePath() + std::string("Projects.json");

	JSON_Doc* doc = App->json->LoadJSON(module_proj->projects_json_filepath.c_str());

	if (doc == nullptr)
		doc = App->json->CreateJSON(module_proj->projects_json_filepath.c_str());

	if (doc != nullptr)
	{
		int projects_count = doc->GetNumber("projects_count");

		for (int i = 0; i < projects_count; ++i)
		{
			JSON_Doc section_node = doc->GetNode();

			std::string proj_section = "project_" + std::to_string(i);

			if (section_node.MoveToSection(proj_section))
			{
				std::string name = section_node.GetString("name");
				std::string path = section_node.GetString("path");

				if (App->file_system->FolderExists(path.c_str()))
				{
					std::string project_doc_path = path + "project.beproject";
					if (App->file_system->FileExists(project_doc_path.c_str()))
					{
						Project* proj = new Project();
						proj->SetName(name.c_str());
						proj->SetPath(path.c_str());
						
						module_proj->projects_lock.lock();
						module_proj->projects.push_back(proj);
						module_proj->projects_lock.unlock();
					}
				}
			}
		}

		App->json->UnloadJSON(doc);
	}

	FinishTask();
}

void LoadProjectsThreadTask::Finish()
{
}
