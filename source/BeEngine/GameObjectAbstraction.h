#ifndef __GAME_OBJECT_ABSTRACTION_H__
#define __GAME_OBJECT_ABSTRACTION_H__

#include <vector>
#include <map>

#include "Globals.h"
#include "GeometryMath.h"
#include "ModuleJson.h"

class GameObject;
class JSON_Doc;
class DataAbstraction;
class GameObjectAbstraction;

class GameObjectAbstractionRelation
{
	friend class GameObjectAbstraction;

public:
	GameObjectAbstractionRelation();

private:
	std::string uid;
	std::string parent_uid;
	DataAbstraction go_abstraction;
	std::vector<DataAbstraction> components_abstraction;
};

class GameObjectAbstraction
{
	friend class GameObject;
	friend class GameObjectAbstractionRelation;

private:
	void operator delete(void *) {}

public:
	GameObjectAbstraction();
	~GameObjectAbstraction();

	void Abstract(const std::vector<GameObject*>& to_abstract);
	std::vector<GameObject*> DeAbstract();

	bool Serialize(const std::string& path, const std::string& name, const std::string& extension);
	bool DeSerialize(const std::string& filepath);

	void Clear();

private:
	std::vector<GameObjectAbstractionRelation> relations;

	bool abstracted = false;
};

#endif // !__GAME_OBJECT_COMPONENT_H__