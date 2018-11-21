#include "ExplorerWindow.h"
#include "Event.h"
#include "App.h"
#include "ModuleResource.h"
#include "ModuleEvent.h"
#include "ModuleProject.h"
#include "ModuleInput.h"
#include "ModuleScripting.h"
#include "ScriptingObjectSolutionManager.h"
#include "ScriptingObjectCompiler.h"

ExplorerWindow::ExplorerWindow()
{
}

ExplorerWindow::~ExplorerWindow()
{
}

void ExplorerWindow::OnEvent(Event * ev)
{
	switch (ev->GetType())
	{
	case EventType::WATCH_FILE_FOLDER:

		update_folders = true;
		update_files = true;

		break;
	case EventType::PROJECT_SELECTED:

		update_folders = true;
		update_files = true;

		break;
	default:
		break;
	}
}

void ExplorerWindow::Start()
{
	App->event->Suscribe(std::bind(&ExplorerWindow::OnEvent, this, std::placeholders::_1), EventType::WATCH_FILE_FOLDER);
	App->event->Suscribe(std::bind(&ExplorerWindow::OnEvent, this, std::placeholders::_1), EventType::PROJECT_SELECTED);
}

void ExplorerWindow::CleanUp()
{
	ClearFolders();
	ClearFiles();
}

void ExplorerWindow::DrawEditor()
{
	float2 win_size = GetWindowSize();

	if (update_folders)
	{
		UpdateFolders();

		update_folders = false;
	}

	if (update_files)
	{
		UpdateFiles();

		update_files = false;
	}

	if (App->project->GetCurrProjectIsSelected())
	{
		ImGui::BeginChild("FoldersChild", ImVec2(win_size.x * 0.3f, 0), true, ImGuiWindowFlags_HorizontalScrollbar);

		DrawFoldersColumn();

		DrawFoldersPopupExtern();

		ImGui::EndChild();

		ImGui::SameLine();

		ImGui::BeginChild("FilesChild", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);

		DrawFilesColumn();

		DrawFilesPopupExtern();

		ImGui::EndChild();
	}
}

ImGuiWindowFlags ExplorerWindow::GetWindowFlags()
{
	return ImGuiWindowFlags_MenuBar;
}

void ExplorerWindow::UpdateFolders()
{
	ClearFolders();

	Folder folder_tree = App->file_system->GetFilesAndFoldersTree(App->resource->GetAssetsPath().c_str());

	CreateExplorerFolderRecursive(folder_tree, nullptr);
}

void ExplorerWindow::ClearFolders()
{
	RemoveAllFromSelectedFolders();
	DestroyAllExplorerFolders();
}

void ExplorerWindow::UpdateFiles()
{
	ClearFiles();

	std::vector<std::string> selected_folder_files_paths = App->file_system->GetFilesAndFoldersInPath(App->resource->GetCurrentAssetsPath().c_str());

	for (std::vector<std::string>::iterator it = selected_folder_files_paths.begin(); it != selected_folder_files_paths.end(); ++it)
	{
		ExplorerFile* ef = new ExplorerFile();

		if (!App->resource->IsMeta((*it).c_str()))
		{
			ef->dfp = App->file_system->DecomposeFilePath((*it).c_str());

			ef->selected = false;

			cur_files_folders.push_back(ef);
		}
	}
}

void ExplorerWindow::ClearFiles()
{
	RemoveAllFromSelectedFiles();

	for (std::vector<ExplorerFile*>::iterator it = cur_files_folders.begin(); it != cur_files_folders.end(); ++it)
	{
		RELEASE(*it);
	}

	cur_files_folders.clear();
}

void ExplorerWindow::DrawFoldersColumn()
{
	for (std::vector<ExplorerFolder*>::iterator it = curr_folders.begin(); it != curr_folders.end(); ++it)
	{
		DrawFoldersRecursive((*it));
	}
}

void ExplorerWindow::DrawFilesColumn()
{
	uint selected_files_count = selected_files.size();

	for (std::vector<ExplorerFile*>::iterator it = cur_files_folders.begin(); it != cur_files_folders.end(); ++it)
	{
		ExplorerFile* curr_file = (*it);

		std::string name = "";
		
		if (!curr_file->dfp.its_folder)
			name = curr_file->dfp.file_name + "." + curr_file->dfp.file_extension_lower_case;
		else
			name = curr_file->dfp.folder_name;

		uint flags = ImGuiTreeNodeFlags_Leaf;

		if (curr_file->selected)
			flags |= ImGuiTreeNodeFlags_Selected;

		bool opened = ImGui::TreeNodeEx(name.c_str(), flags);

		bool left_clicked = false;
		bool right_clicked = false;

		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
			left_clicked = true;

		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) && App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_DOWN)
			right_clicked = true;

		FilesInput(curr_file, left_clicked, right_clicked);

		DrawFilesPopupIntern(left_clicked, right_clicked);

		if (opened)
		{
			ImGui::TreePop();
		}
	}
}

void ExplorerWindow::CreateExplorerFolderRecursive(const Folder & to_create, ExplorerFolder * parent)
{
	ExplorerFolder* ef = new ExplorerFolder();
	ef->dfp = App->file_system->DecomposeFilePath(to_create.folder_path);
	ef->selected = false;
	ef->folder_name = to_create.folder_name;

	if (parent != nullptr)
	{
		parent->childs.push_back(ef);
	}
	else
	{
		curr_folders.push_back(ef);
	}

	for (std::vector<Folder>::const_iterator it = to_create.folders.begin(); it != to_create.folders.end(); ++it)
	{
		CreateExplorerFolderRecursive((*it), ef);
	}
}

void ExplorerWindow::DestroyAllExplorerFolders()
{
	std::vector<ExplorerFolder*> to_destroy;

	for (std::vector<ExplorerFolder*>::iterator it = curr_folders.begin(); it != curr_folders.end(); ++it)
	{
		to_destroy.push_back((*it));
	}

	curr_folders.clear();

	while (!to_destroy.empty())
	{
		ExplorerFolder* curr_folder = *to_destroy.begin();

		to_destroy.erase(to_destroy.begin());

		for (std::vector<ExplorerFolder*>::iterator it = curr_folder->childs.begin(); it != curr_folder->childs.end(); ++it)
		{
			to_destroy.push_back((*it));
		}

		RELEASE(curr_folder);
	}
}

void ExplorerWindow::DrawFoldersRecursive(ExplorerFolder* folder)
{
	uint flags = ImGuiTreeNodeFlags_OpenOnArrow;

	if (folder->childs.size() == 0)
		flags |= ImGuiTreeNodeFlags_Leaf;

	if (folder->selected)
		flags |= ImGuiTreeNodeFlags_Selected;

	bool draw = true;

	if (App->resource->GetAssetsPath().compare(folder->dfp.path) == 0)
	{
		ImGui::SetNextTreeNodeOpen(true);
	}

	ImGui::PushID(folder->dfp.path.c_str());
	bool opened = ImGui::TreeNodeEx(folder->folder_name.c_str(), flags);

	bool left_clicked = false;
	bool right_clicked = false;

	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
		left_clicked = true;

	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) && App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_DOWN)
		right_clicked = true;

	FoldersInput(folder, left_clicked, right_clicked);

	DrawFoldersPopupIntern(left_clicked, right_clicked);

	ImGui::PopID();

	if (opened)
	{
		for (std::vector<ExplorerFolder*>::const_iterator it = folder->childs.begin(); it != folder->childs.end(); ++it)
		{
			DrawFoldersRecursive(*it);
		}

		ImGui::TreePop();
	}
}

void ExplorerWindow::SetSelectedFolderTree(const char * path)
{
	if (App->file_system->FolderExists(path))
	{
		App->resource->SetCurrentAssetsPath(path);

		update_files = true;
	}
}

void ExplorerWindow::FoldersInput(ExplorerFolder* folder, bool left_clicked, bool right_clicked)
{
	uint selected_files_count = selected_files.size();

	// Input
	if ((App->input->GetKeyRepeat(SDL_SCANCODE_LCTRL) || App->input->GetKeyRepeat(SDL_SCANCODE_RCTRL)) && ImGui::IsItemClicked(0))
	{
		if (!folder->selected)
			AddToSelectedFolders(folder);
		else
			RemoveFromSelectedFolders(folder);

		disable_button_up = true;
	}

	// If shift is pressed do fill gap selection
	else if ((App->input->GetKeyRepeat(SDL_SCANCODE_LSHIFT) || App->input->GetKeyRepeat(SDL_SCANCODE_RSHIFT)) && ImGui::IsItemClicked(0))
	{

	}
	// Monoselection
	else
	{
		if ((left_clicked || right_clicked) && !folder->selected)
		{
			RemoveAllFromSelectedFolders();
			AddToSelectedFolders(folder);

			SetSelectedFolderTree(folder->dfp.path.c_str());

			disable_button_up = true;
		}
		else if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP && ImGui::IsItemHovered() && folder->selected && selected_files_count == 1)
		{
			if (!disable_button_up)
			{
				RemoveFromSelectedFolders(folder);
			}
			else
				disable_button_up = false;
		}
		else if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP && ImGui::IsItemHovered())
		{
			if (!disable_button_up)
			{
				if (selected_files_count > 1)
				{
					RemoveAllFromSelectedFolders();
					AddToSelectedFolders(folder);
				}
			}
			else
				disable_button_up = false;
		}

		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN && !ImGui::IsAnyItemHovered() && ImGui::IsMouseHoveringWindow())
		{
			RemoveAllFromSelectedFolders();
		}
	}
}

void ExplorerWindow::DrawFoldersPopupIntern(bool left_clicked, bool right_clicked)
{
	if (right_clicked)
	{
		ImGui::OpenPopupOnItemClick("FoldersPopup", 1);
	}

	bool open_rename = false;

	if (ImGui::BeginPopupContextItem("FoldersPopup"))
	{
		if (ImGui::Button("Delete"))
		{

		}

		ImGui::EndPopup();
	}
}

void ExplorerWindow::DrawFoldersPopupExtern()
{
}

void ExplorerWindow::AddToSelectedFolders(ExplorerFolder * folder)
{
	bool already_selected = false;
	std::vector<ExplorerFolder*> folders = selected_folders;
	for (std::vector<ExplorerFolder*>::iterator it = folders.begin(); it != folders.end(); ++it)
	{
		if (folder->dfp.path.compare((*it)->dfp.path) == 0)
		{
			already_selected = true;
			break;
		}
	}

	if (!already_selected)
	{
		folder->selected = true;
		selected_folders.push_back(folder);
	}
}

void ExplorerWindow::RemoveFromSelectedFolders(ExplorerFolder * folder)
{
	for (std::vector<ExplorerFolder*>::iterator it = selected_folders.begin(); it != selected_folders.end(); ++it)
	{
		if (folder->dfp.path.compare((*it)->dfp.path) == 0)
		{
			folder->selected = false;
			selected_folders.erase(it);
			break;
		}
	}
}

void ExplorerWindow::RemoveAllFromSelectedFolders()
{
	for (std::vector<ExplorerFolder*>::iterator it = selected_folders.begin(); it != selected_folders.end(); ++it)
	{
		(*it)->selected = false;
	}

	selected_folders.clear();
}

void ExplorerWindow::FilesInput(ExplorerFile * file, bool left_clicked, bool right_clicked)
{
	uint selected_files_count = selected_files.size();

	// Input
	if ((App->input->GetKeyRepeat(SDL_SCANCODE_LCTRL) || App->input->GetKeyRepeat(SDL_SCANCODE_RCTRL)) && ImGui::IsItemClicked(0))
	{
		if (!file->selected)
			AddToSelectedFiles(file);
		else
			RemoveFromSelectedFiles(file);

		disable_button_up = true;
	}

	// If shift is pressed do fill gap selection
	else if ((App->input->GetKeyRepeat(SDL_SCANCODE_LSHIFT) || App->input->GetKeyRepeat(SDL_SCANCODE_RSHIFT)) && ImGui::IsItemClicked(0))
	{

	}
	// Monoselection
	else
	{
		if ((left_clicked || right_clicked) && !file->selected)
		{
			RemoveAllFromSelectedFiles();
			AddToSelectedFiles(file);

			disable_button_up = true;
		}
		else if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP && ImGui::IsItemHovered() && file->selected && selected_files_count == 1)
		{
			if (!disable_button_up)
			{
				RemoveFromSelectedFiles(file);
			}
			else
				disable_button_up = false;
		}
		else if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP && ImGui::IsItemHovered())
		{
			if (!disable_button_up)
			{
				if (selected_files_count > 1)
				{
					RemoveAllFromSelectedFiles();
					AddToSelectedFiles(file);
				}
			}
			else
				disable_button_up = false;
		}

		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN && !ImGui::IsAnyItemHovered() && ImGui::IsMouseHoveringWindow())
		{
			RemoveAllFromSelectedFiles();
		}
	}
}

void ExplorerWindow::DrawFilesPopupIntern(bool left_clicked, bool right_clicked)
{
	std::vector<ExplorerFile*> selected = GetSelectedFiles();

	if (right_clicked)
	{
		ImGui::OpenPopupOnItemClick("FilesPopup", 1);
	}

	bool open_rename = false;

	if (ImGui::BeginPopupContextItem("FilesPopup"))
	{
		if (selected.size() == 1)
		{
			DecomposedFilePath selected_file = selected[0]->dfp;

			if (selected_file.file_extension_lower_case == "cs")
			{
				if (ImGui::Button("Edit Script"))
				{
					ImGui::CloseCurrentPopup();
					App->scripting->solution_manager->OpenSolutionWithExternalProgram();
				}
			}

			if (ImGui::Button("Show in Explorer"))
			{
				App->OpenFolder(selected_file.path.c_str());
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::Button("Rename"))
			{
				open_rename = true;
				ImGui::CloseCurrentPopup();
			}
		}

		if (ImGui::Button("Delete"))
		{
			for (std::vector<ExplorerFile*>::iterator it = selected.begin(); it != selected.end(); ++it)
			{
				App->resource->UnloadAssetFromEngine((*it)->dfp.file_path.c_str());

				update_files = true;
				update_folders = true;
			}
		}

		ImGui::EndPopup();
	}

	if (open_rename)
	{
		ImGui::OpenPopup("RenamePopup");

		if (selected.size() > 0)
		{
			std::string name = (*selected.begin())->dfp.file_name;

			int size = name.size();

			if (size > 50)
				size = 50;

			memset(change_name_tmp, 0, sizeof(char) * 50);

			strcpy_s(change_name_tmp, sizeof(char) * size + 1, name.c_str());
		}
	}

	if (ImGui::BeginPopup("RenamePopup"))
	{
		ImGui::Text("Name: ");

		ImGui::SameLine();

		ImGui::InputText("", change_name_tmp, sizeof(char) * 50, ImGuiInputTextFlags_AutoSelectAll);

		if (ImGui::Button("Accept"))
		{
			if (selected.size() > 0)
			{
				App->resource->RenameAsset(selected[0]->dfp.file_path.c_str(), change_name_tmp);
				
				update_files = true;
				update_folders = true;
			}

			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel"))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

void ExplorerWindow::DrawFilesPopupExtern()
{
	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_DOWN && !ImGui::IsAnyItemHovered() 
		&& ImGui::IsMouseHoveringWindow() && selected_files.size() == 0)
	{
		ImGui::OpenPopup("CreatePopup");
	}

	if (ImGui::BeginPopup("CreatePopup"))
	{
		if (ImGui::Button("Create Script"))
		{
			App->resource->CreateScript(App->resource->GetCurrentAssetsPath().c_str(), "NewScript");
			ImGui::CloseCurrentPopup();

			update_files = true;
		}

		if (ImGui::Button("Import Asset"))
		{
			bool canceled = false;

			const char* filter[1] = { "" };
			std::string file = App->file_system->SelectFileDilog(canceled, filter);

			if (!canceled)
			{
				App->resource->LoadFileToEngine(file.c_str());
			}

			ImGui::CloseCurrentPopup();

			update_files = true;
			update_folders = true;
		}

		ImGui::EndPopup();
	}
}

void ExplorerWindow::AddToSelectedFiles(ExplorerFile* add)
{	
	bool already_selected = false;
	std::vector<ExplorerFile*> files = selected_files;
	for (std::vector<ExplorerFile*>::iterator it = files.begin(); it != files.end(); ++it)
	{
		if (add->dfp.file_path.compare((*it)->dfp.file_path) == 0)
		{
			already_selected = true;
			break;
		}
	}

	if (!already_selected)
	{
		add->selected = true;
		selected_files.push_back(add);
	}
}

void ExplorerWindow::RemoveFromSelectedFiles(ExplorerFile* add)
{
	for (std::vector<ExplorerFile*>::iterator it = selected_files.begin(); it != selected_files.end(); ++it)
	{
		if (add->dfp.file_path.compare((*it)->dfp.file_path) == 0)
		{
			add->selected = false;
			selected_files.erase(it);
			break;
		}
	}
}

void ExplorerWindow::RemoveAllFromSelectedFiles()
{
	for (std::vector<ExplorerFile*>::iterator it = selected_files.begin(); it != selected_files.end(); ++it)
	{
		(*it)->selected = false;
	}

	selected_files.clear();
}

std::vector<ExplorerFile*> ExplorerWindow::GetSelectedFiles()
{
	return selected_files;
}

ExplorerFile::ExplorerFile()
{
}

ExplorerFolder::ExplorerFolder()
{
}
