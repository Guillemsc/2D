#ifndef __BUILD_WINDOW_H__
#define __BUILD_WINDOW_H__

#include "ModuleEditor.h"
#include "imgui.h"
#include "ResourceSelectorWidget.h"

class BuildWindow : public EditorWindow
{
	friend class ModuleEditor;

private:
	void operator delete(void *) {}

public:
	BuildWindow();
	~BuildWindow();

	void Start();
	void CleanUp();
	void DrawEditor();
	ImGuiWindowFlags GetWindowFlags();

private:
	void Save();

private:
	ResourceSelectorWidget resource_selector;

};

#endif // !__BUILD_WINDOW_H__