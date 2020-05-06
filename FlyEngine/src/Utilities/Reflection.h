/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.

Author: Fenil Shingala, fenil.shingala, 60003118
- End Header --------------------------------------------------------*/
#pragma once

using namespace rapidjson;

class Entity;
class Component;
class MapListStructure;
class Component;

class Reflection
{
public:
	Reflection();
	~Reflection();

	void Deserialize(Document* pDoc, bool isPauseMenu=false);
	void Serialize(unsigned int i);
	std::string GetNameFromSlot(unsigned int i);

	Entity* LoadObject(std::string _filename);
	void EntityInsideEntity(GenericObject<false, Value::ValueType>, Entity*);
	MapListStructure* getPoolObject(std::string component_name);
	Component* Reflect(Value::MemberIterator& ComponentItr, Component* pComp, Entity* pEntity);

public:
	std::string lastSerializedFile;

private:
	void set_property(
		rttr::property& prop, Component* pComp, GenericObject<false, rapidjson::Value::ValueType>& DataDoc, Value::MemberIterator& DataItr
	);
};