#ifndef __HIERARCHY_WINDOW_H__
#define __HIERARCHY_WINDOW_H__

#include "ModuleEditor.h"
#include "imgui.h"

class GameObject;

class HierarchyWindow : public EditorWindow
{
	friend class ModuleEditor;

private:
	void operator delete(void *) {}

public:
	HierarchyWindow();
	~HierarchyWindow();

	void Start();
	void CleanUp();
	void DrawEditor();
	ImGuiWindowFlags GetWindowFlags();

private:
	void DrawMenuBar();
	void DrawGameObjectsPopup(GameObject* go, bool left_clicked, bool right_clicked);
	void GameObjectInput(GameObject* go, bool left_clicked, bool right_clicked);
	void DrawGameObjectRecursive(GameObject* go, uint child_index, uint &go_count);
	void DragAndDropBeforeChilds(GameObject* go, uint child_index, uint &go_count);
	void DragAndDropAfterChilds(GameObject* go, uint child_index, uint &go_count);

private:
	ImFont* font = nullptr;

	bool disable_button_up = true;
	bool dragging = false;

	char change_name_tmp[50];
};

#endif // !__HIERARCHY_WINDOW_H__