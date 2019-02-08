#ifndef __EXPLORER_WINDOW_H__
#define __EXPLORER_WINDOW_H__

#include "ModuleEditor.h"
#include "imgui.h"
#include "ModuleFileSystem.h"

class GameObject;
class Resource;
class Event;

class ExplorerFile
{
public:
	ExplorerFile();

	DecomposedFilePath dfp;
	bool selected = false;
	Resource* resource = nullptr;
};

class ExplorerFolder
{
public:
	ExplorerFolder();

	DecomposedFilePath dfp;
	std::string folder_name;
	std::vector<ExplorerFolder*> childs;
	bool selected = false;
};

class ExplorerWindow : public EditorWindow
{
	friend class ModuleEditor;

private:
	void operator delete(void *) {}

public:
	ExplorerWindow();
	~ExplorerWindow();

	void OnEvent(Event* ev);

	void Start();
	void CleanUp();
	void DrawEditor();
	ImGuiWindowFlags GetWindowFlags();

public:
	void UpdateFolders();
	void UpdateFiles();

private:
	void ClearFolders();
	void ClearFiles();

	void DrawFilesMenuBar();

	void DrawFoldersColumn();
	void DrawFilesColumn();

	// Folders Column
	void CreateExplorerFolderRecursive(const Folder& to_create, ExplorerFolder* parent);
	void DestroyAllExplorerFolders();
	void DrawFoldersRecursive(ExplorerFolder* folder);
	void SetSelectedFolderTree(const char* path);
	void FoldersInput(ExplorerFolder* folder, bool left_clicked, bool right_clicked);
	void DrawFoldersPopupIntern(ExplorerFolder* folder, bool left_clicked, bool right_clicked);
	void DrawFoldersPopupExtern();

	void FoldersDragAndDrop(ExplorerFolder* folder);

	void AddToSelectedFolders(ExplorerFolder* folder);
	void RemoveFromSelectedFolders(ExplorerFolder* folder);
	void RemoveAllFromSelectedFolders();

	// Files column
	void FilesInput(ExplorerFile* file, bool left_clicked, bool right_clicked);
	void DrawFilesPopupIntern(bool left_clicked, bool right_clicked);
	void DrawFilesPopupExtern();

	void FilesDragAndDrop(ExplorerFile* file);

	void AddToSelectedFiles(ExplorerFile* add);
	void RemoveFromSelectedFiles(ExplorerFile* add);
	void RemoveAllFromSelectedFiles();
	std::vector<ExplorerFile*> GetSelectedFiles();

private:
	ImFont* font = nullptr;
	
	std::vector<ExplorerFolder*> curr_folders;
	std::vector<ExplorerFolder*> selected_folders;

	std::vector<ExplorerFile*> curr_files;
	std::vector<ExplorerFile*> selected_files;

	bool update_folders = false;
	bool update_files = false;

	bool disable_button_up = true;
	bool dragging = false;

	std::string files_curr_path = "";

	char name_tmp[50];
};

#endif // !__HIERARCHY_WINDOW_H__