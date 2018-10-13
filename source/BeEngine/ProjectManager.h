#ifndef __PROJECT_MANAGER_H__
#define __PROJECT_MANAGER_H__

#include "ModuleEditor.h"

class ProjectManager : public EditorElement
{
	friend class ModuleEditor;

private:
	void operator delete(void *) {}

public:
	ProjectManager();
	~ProjectManager();

	void Start();
	void CleanUp();
	void DrawEditor();

private:
	void DrawProjectSelector();
	void DrawProjectCreator();

private:
	ImFont * big_font = nullptr;
	ImFont * medium_font = nullptr;
	ImFont * small_font = nullptr;

	int margins = 0;
	float2 window_size = float2::zero;

	bool creating_project = false;
	
	char project_creation_name[100];

	std::string project_creation_location;
};

#endif // !__PROJECT_MANAGER_H__