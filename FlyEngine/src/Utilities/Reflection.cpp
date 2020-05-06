/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.

Author: Fenil Shingala, fenil.shingala, 60003118
- End Header --------------------------------------------------------*/
#include "../pch.h"
#include "Reflection.h"

////////////////////////// EXTERNAL DEPENDENCIES /////////////////////////////
#include "../InitData.h"
#include "../Components/Shape.h"
#include "../Components/Transform.h"

#include "../Managers/EntityManager.h"
extern EntityManager* gpEntityManager;

#include "../Managers/ComponentManager.h"
extern ComponentManager* gpComponentManager;

#include "../Managers/ResourceManager.h"
extern ResourceManager* gpResourceManager;

#include "../Managers/CameraManager.h"
extern CameraManager* gpCameraManager;

#include "../Systems/PlayerSystem.h"
#include "../Systems/BossSystem.h"
extern SystemManager* gpSystemManager;



#include "../Managers/SceneManager.h"
extern SceneManager* gpSceneManager;

#include "MapListStructure.h"
#include "ShapeStructure.h"


extern AudioManager *gpAudioManager;
///////////////////////////////////////////////////////////////////////////////

rapidjson::Value& get_component_object(rapidjson::Document::AllocatorType& allocator,
	rttr::instance& ol, rttr::property& prop, rapidjson::Value& component_object
);

static unsigned int file_no = 0;
//TODO review LOD
#define _2F(val) ((int)(val*10000 + .5) / 10000.0)

Reflection::Reflection()
{
	/*rttr::type t = rttr::type::get<Transform>();
	for (auto& prop : t.get_properties())
		std::cout << "name: " << prop.get_name();*/

	//rttr::type t = rttr::type::get_by_name("Transform");


	//rttr::variant var = t.create();    // will invoke the previously registered ctor


	//auto dtor = t.get_destructor();
	//dtor.invoke(var);

	//auto& obj = var.get_wrapped_value<Transform>();
	
	/*for (auto& property : t.get_properties()) {
		std::string propertyName = property.get_type().get_name().to_string();
		std::cout << propertyName << "\n";

		if (propertyName == "structglm::tvec3<float,0>") {
			glm::vec3 tempVec;
			tempVec.x = 1;
			tempVec.y = 2;
			tempVec.z = 3;

			property.set_value(var, tempVec);
		}
	}*/

	//rttr::constructor ctor = t.get_constructor();  // 2nd way with the constructor class
	//var = ctor.invoke();
	//std::cout << var.get_type().get_name();  // prints 'MyStruct'
	//getchar();
}

Reflection::~Reflection() {}

/* ------ helper class for Serialiaze ------ */

void myfunction(Entity* pEntity, Value& player, rapidjson::Document::AllocatorType& allocator) {
	rttr::enumeration type_enum = rttr::type::get_by_name("EntityType").get_enumeration();
	std::string type_name = type_enum.value_to_name(pEntity->mType).to_string();

	MapListStructure* pMLS = nullptr;

	// Get instance through global gpComponentManager
	rttr::instance ol = *gpComponentManager;
	if (ol.is_valid()) {

		player.AddMember("file", Value((type_name + ".json").c_str(), allocator).Move(), allocator);

		for (auto& compon : pEntity->mComponents) {
			std::string component_name = compon->mComponentName;

			if ("Transform" == component_name || "Oscillation" == component_name
				|| "EnemyAI" == component_name || "Launcher" == component_name||"Light" == component_name || "Model" == component_name
				|| "Stencil_Toon_Model" == component_name || "Particle" == component_name || "Camera" == component_name
				|| "Key" == component_name || "Door" == component_name || "Text" == component_name) {

				rapidjson::Value component_object(kObjectType);
				rttr::type component_type = rttr::type::get_by_name(component_name);

				// traverse component properties to write
				for (auto prop : component_type.get_properties())
				{
					std::string prop_name = prop.get_name().to_string();
					std::string prop_type = prop.get_type().get_name().to_string();
					rttr::instance ol = compon;

					component_object = get_component_object(allocator, ol, prop, component_object);
				}

				player.AddMember(Value(component_name.c_str(), allocator).Move(),
					component_object, allocator);
			}
		}

	}

}


rapidjson::Value& get_component_object(rapidjson::Document::AllocatorType& allocator,
	rttr::instance& ol, rttr::property& prop, rapidjson::Value& component_object
)
{
	rttr::variant var = prop.get_value(ol);

	// INT
	if (prop.get_type() == rttr::type::get<int>()) {
		int data;
		if (var.can_convert<int>()) {
			data = var.convert<int>();
		}
		component_object.AddMember(
			Value(prop.get_name().to_string().c_str(), allocator).Move(),
			data,
			allocator
		);
	}
	
	// BOOL
	if (prop.get_type() == rttr::type::get<bool>()) {
		bool data;
		if (var.can_convert<bool>()) {
			data = var.convert<bool>();
		}
		component_object.AddMember(
			Value(prop.get_name().to_string().c_str(), allocator).Move(),
			data,
			allocator
		);
	}

	// FLOAT
	else if (prop.get_type() == rttr::type::get<float>()) {
		float data;
		if (var.can_convert<float>()) {
			data = var.convert<float>();
		}
		component_object.AddMember(
			Value(prop.get_name().to_string().c_str(), allocator).Move(),
			_2F(data),
			allocator
		);
	}

	// STRING
	else if (prop.get_type() == rttr::type::get<std::string>()) {
		std::string data;
		if (var.can_convert<std::string>()) {
			data = var.convert<std::string>();
		}
		component_object.AddMember(
			Value(prop.get_name().to_string().c_str(), allocator).Move(),
			Value(data.c_str(), allocator).Move(),
			allocator
		);
	}

	// VEC3
	else if (prop.get_type() == rttr::type::get<glm::vec3>()) {
		glm::vec3 data;
		if (var.can_convert<glm::vec3>()) {
			data = var.convert<glm::vec3>();
		}

		rapidjson::Value array(rapidjson::kArrayType);
		array.PushBack(_2F(data.x), allocator)
			.PushBack(_2F(data.y), allocator)
			.PushBack(_2F(data.z), allocator);
		component_object.AddMember(Value(prop.get_name().to_string().c_str(), allocator).Move(), array, allocator);
	}
	// VEC4
	/*else if (prop.get_type() == rttr::type::get<glm::vec4>()) {
		glm::vec4 data;
		if (var.can_convert<glm::vec4>()) {
			data = var.convert<glm::vec4>();
		}

		rapidjson::Value array(rapidjson::kArrayType);
		array.PushBack(_2F(data.x), allocator)
			.PushBack(_2F(data.y), allocator)
			.PushBack(_2F(data.z), allocator)
		.PushBack(_2F(data.w), allocator);
		component_object.AddMember(Value(prop.get_name().to_string().c_str(), allocator).Move(), array, allocator);
	}*/

	// VEC2
	else if (prop.get_type() == rttr::type::get<glm::vec2>()) {
		glm::vec2 data;
		if (var.can_convert<glm::vec2>()) {
			data = var.convert<glm::vec2>();
		}

		rapidjson::Value array(rapidjson::kArrayType);
		array.PushBack(_2F(data.x), allocator)
			.PushBack(_2F(data.y), allocator);
		component_object.AddMember(Value(prop.get_name().to_string().c_str(), allocator).Move(), array, allocator);
	}

	// enum DoorType
	else if (prop.get_type() == rttr::type::get<DoorType>()) {
		std::string data;
		if (var.can_convert<DoorType>()) {
			DoorType temp = var.convert<DoorType>();
			rttr::enumeration type_enum = rttr::type::get_by_name("DoorType").get_enumeration();
			data = type_enum.value_to_name(temp).to_string();
		}
		component_object.AddMember(
			Value(prop.get_name().to_string().c_str(), allocator).Move(),
			Value(data.c_str(), allocator).Move(),
			allocator
		);
	}

	// enum MissileType
	else if (prop.get_type() == rttr::type::get<MissileType>()) {
		std::string data;
		if (var.can_convert<MissileType>()) {
			MissileType temp = var.convert<MissileType>();
			rttr::enumeration type_enum = rttr::type::get_by_name("MissileType").get_enumeration();
			data = type_enum.value_to_name(temp).to_string();
		}
		component_object.AddMember(
			Value(prop.get_name().to_string().c_str(), allocator).Move(),
			Value(data.c_str(), allocator).Move(),
			allocator
		);
	}

	// struct SHAPE 
	else if (prop.get_type() == rttr::type::get<Shape*>()) {
		Shape* data = nullptr;
		Value shape(kObjectType);

		if (var.can_convert<Shape*>()) {
			data = var.convert<Shape*>();
		}

		if (data->mType == CIRCLE) {
			shape.AddMember("Name", "Circle", allocator);
			ShapeCircle* circle = static_cast<ShapeCircle*>(data);
			shape.AddMember("Radius", _2F(circle->mRadius), allocator);
		}
		else if (data->mType == AABB) {
			shape.AddMember("Name", "AABB", allocator);
			ShapeAABB* aabb = static_cast<ShapeAABB*>(data);
			shape.AddMember("Top", _2F(aabb->mTop), allocator)
				.AddMember("Bottom", _2F(aabb->mBottom), allocator)
				.AddMember("Left", _2F(aabb->mLeft), allocator)
				.AddMember("Right", _2F(aabb->mRight), allocator);
		}


		component_object.AddMember(Value(prop.get_name().to_string().c_str(), allocator).Move(), shape, allocator);

	}

	// Entity
	else if (prop.get_type() == rttr::type::get<Entity*>()) {
		Entity* data = nullptr;
		if (var.can_convert<Entity*>()) {
			data = var.convert<Entity*>();
		}

		if (data) {
			Value entity(kObjectType);
			myfunction(data, entity, allocator);

			component_object.AddMember(Value(prop.get_name().to_string().c_str(), allocator).Move(), entity, allocator);
		}
	}

	// Entity Vector
	else if (prop.get_type() == rttr::type::get<std::vector<Entity*>>()) {
		
		std::vector<Entity*> data;
		if (var.can_convert< std::vector<Entity*> >()) {
			data = var.convert< std::vector<Entity*> >();
		}

		if (0 != data.size()) {
			rapidjson::Value array(rapidjson::kArrayType);

			for (auto pEntity : data) {
				Value entity(kObjectType);
				myfunction(pEntity, entity, allocator);
				array.PushBack(entity, allocator);
			}
			component_object.AddMember(Value(prop.get_name().to_string().c_str(), allocator).Move(), array, allocator);
		}
	}

	return component_object;
}

std::string Reflection::GetNameFromSlot(unsigned int i) {
	std::string output = "Serialized/Output" + std::to_string(i) + ".json";
	return output;
}


// WRITE TO JSON
void Reflection::Serialize(unsigned int i)
{
	std::string myfilename = directory::serialize + "Output" + std::to_string(i) +".json";
	Document doc;

	FILE* fp;
	fopen_s(&fp, myfilename.c_str(), "wb"); // non-Windows use "w"
	char writeBuffer[65536];
	rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
	Writer<FileWriteStream> writer(os);
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

	int n = 0;

	doc.SetObject();

	std::vector<MapListStructure*> component_list;

	rttr::type component_pool = rttr::type::get_by_name("ComponentManager");


	for (auto itr = gpEntityManager->mEntity.begin(); itr != gpEntityManager->mEntity.end(); ++itr) {

		if (itr->second->mParent || itr->second->mOwnerEntity || itr->second->mType == DOOR)
			continue;

		Value player(kObjectType);

		Entity* pEntity = itr->second;
		myfunction(pEntity, player, allocator);

		rttr::enumeration type_enum = rttr::type::get_by_name("EntityType").get_enumeration();
		std::string type_name = type_enum.value_to_name(pEntity->mType).to_string();

		std::string player_name = type_name + std::to_string(n++);
		doc.AddMember(Value(player_name.c_str(), allocator).Move(), player, allocator);
	}

	Value audio(kObjectType);

	SoundMap* list = gpAudioManager->GetSoundMap();
	auto list_begin = list->begin();

	rapidjson::Value loopArray(rapidjson::kArrayType);
	rapidjson::Value loadLoopArray(rapidjson::kArrayType);

	for (auto itr = list_begin; itr != list->end(); ++itr) {
		FMOD_MODE mode;
		FMOD_RESULT res = itr->second->getMode(&mode);
		std::string loadString = itr->first;
		
		if (mode == 9) {
			loopArray.PushBack(Value(loadString.c_str(), allocator).Move(), allocator);
		}
		else if (mode == 10) {
			loadLoopArray.PushBack(Value(loadString.c_str(), allocator).Move(), allocator);
		}
	}
	audio.AddMember("Load", loopArray, allocator);
	audio.AddMember("LoadLoop", loadLoopArray, allocator);

	doc.AddMember("Audios", audio, allocator);

	doc.Accept(writer);
	fclose(fp);
}

/* ------ helper class for Deserialiaze ------ */
void Reflection::set_property(
	rttr::property& prop, Component* pComp, GenericObject<false, rapidjson::Value::ValueType>& DataDoc, Value::MemberIterator& DataItr
	)
{
	// read values from JSON and set it in class variables(property)
	if (prop.get_type() == rttr::type::get<int>()) {
		prop.set_value(pComp, DataDoc[DataItr->name.GetString()].GetInt());
	}
	else if (prop.get_type() == rttr::type::get<bool>()) {
		prop.set_value(pComp, DataDoc[DataItr->name.GetString()].GetBool());
	}
	else if (prop.get_type() == rttr::type::get<float>()) {
		prop.set_value(pComp, DataDoc[DataItr->name.GetString()].GetFloat());
	}
	else if (prop.get_type() == rttr::type::get<std::string>()) {
		prop.set_value(pComp, std::string(DataDoc[DataItr->name.GetString()].GetString()));
	}
	else if (prop.get_type() == rttr::type::get<glm::vec3>()) {
		auto arr = DataDoc[DataItr->name.GetString()].GetArray();
		glm::vec3 data = glm::vec3(arr[0].GetFloat(), arr[1].GetFloat(), arr[2].GetFloat());
		prop.set_value(pComp, data);
	}
	else if (prop.get_type() == rttr::type::get<glm::vec2>()) {
		auto arr = DataDoc[DataItr->name.GetString()].GetArray();
		glm::vec2 data = glm::vec2(arr[0].GetFloat(), arr[1].GetFloat());
		prop.set_value(pComp, data);
	}
	else if (prop.get_type() == rttr::type::get<Shape*>()) {
		Shape* data;
		if (DataDoc["Shape"]["Name"].GetString() == "CIRCLE") {
			ShapeCircle * d1 = static_cast<ShapeCircle *>(gpComponentManager->mShapeCircle->Add(pComp->mpOwner->id));
			d1->SetValue(DataDoc["Shape"]["Radius"].GetFloat());
			//data = new ShapeCircle(DataDoc["Shape"]["Radius"].GetFloat());
			d1->mpOwnerCollider = static_cast<Collider*>(pComp);
			data = d1;
		}

		if (DataDoc["Shape"]["Name"].GetString() == std::string("AABB")) {
			ShapeAABB * d2 = static_cast<ShapeAABB *>(gpComponentManager->mShapeAABB->Add(pComp->mpOwner->id));
				d2->SetValue(
				DataDoc["Shape"]["Top"].GetFloat(),
				DataDoc["Shape"]["Bottom"].GetFloat(),
				DataDoc["Shape"]["Left"].GetFloat(),
				DataDoc["Shape"]["Right"].GetFloat()
			);
			d2->mpOwnerCollider = static_cast<Collider*>(pComp);
			data = d2;
		}
		prop.set_value(pComp, data);
	}
	else if (prop.get_type() == rttr::type::get<DoorType>()) {
		rttr::enumeration type_enum = rttr::type::get_by_name("DoorType").get_enumeration();
		DoorType data = type_enum.name_to_value(DataDoc["DoorType"].GetString()).convert<DoorType>();
		prop.set_value(pComp, data);
	}
	else if (prop.get_type() == rttr::type::get<MissileType>()) {
		rttr::enumeration type_enum = rttr::type::get_by_name("MissileType").get_enumeration();
		MissileType data = type_enum.name_to_value(DataDoc["MissileType"].GetString()).convert<MissileType>();
		prop.set_value(pComp, data);
	}
	else if (prop.get_type() == rttr::type::get<Entity*>()) {
		GenericObject<false, Value::ValueType> EntityDoc = DataDoc[DataItr->name.GetString()].GetObject();
		Entity* data = LoadObject(EntityDoc["file"].GetString());
		EntityInsideEntity(EntityDoc, data);
		for (auto comp : data->mComponents) {
			comp->Init();
		}
		prop.set_value(pComp, data);
	}
	else if (prop.get_type() == rttr::type::get<std::vector<Entity*>>()) {
		std::vector<Entity*> data;
		auto arr = DataDoc[DataItr->name.GetString()].GetArray();
		for (SizeType i = 0; i < arr.Size(); i++) // Uses SizeType instead of size_t
		{
			GenericObject<false, Value::ValueType> EntityDoc = arr[i].GetObject();
			Entity* pEntity = LoadObject(EntityDoc["file"].GetString());
			EntityInsideEntity(EntityDoc, pEntity);
			for (auto comp : pEntity->mComponents) {
				comp->Init();
			}
			data.push_back(pEntity);
		}
		prop.set_value(pComp, data);
	}
}

/* ------ helper class for Deserialiaze ------ */
MapListStructure* Reflection::getPoolObject(std::string component_name)
{
	rttr::type component_pool = rttr::type::get_by_name("ComponentManager");


	// Get registered component pool
	rttr::property mls = component_pool.get_property("m" + component_name);

	MapListStructure* pMLS = nullptr;

	// Get instance through global gpComponentManager
	rttr::instance ol = *gpComponentManager;
	if (ol.is_valid()) {

		// get component pool(property) from instance as a variant
		rttr::variant val = mls.get_value(ol);

		// convert it to user defined type
		if (val.can_convert<MapListStructure*>())
			pMLS = val.convert<MapListStructure*>();
	}
	return pMLS;
}

/* ------ helper class for Deserialiaze ------ */
Component* Reflection::Reflect(Value::MemberIterator& ComponentItr, Component* pComp, Entity* pEntity)
{
	std::string component_name = ComponentItr->name.GetString();
	MapListStructure* pMLS = getPoolObject(component_name);

	if (pComp == nullptr)
	{
		pComp = pMLS->Add(pEntity->id);
		pComp->mpOwner = pEntity;
	}

	// Get registered component
	rttr::type component = rttr::type::get_by_name(component_name);

	GenericObject<false, rapidjson::Value::ValueType> DataDoc = ComponentItr->value.GetObject();

	// loop through all variables in JSON
	for (Value::MemberIterator DataItr = DataDoc.MemberBegin(); DataItr != DataDoc.MemberEnd(); ++DataItr)
	{
		// find mapped variable in component class
		rttr::property prop = component.get_property(DataItr->name.GetString());

		//std::string data_name = prop.get_name().to_string();
		//std::string type_name = prop.get_type().get_name().to_string();
		
		set_property(prop, pComp, DataDoc, DataItr);
	}
	return pComp;
}

/* ------ helper class for Deserialiaze ------ */
Entity* Reflection::LoadObject(std::string _filename)
{
	Document* pPrefabDoc = gpResourceManager->LoadPrefab(_filename);
	Entity* pEntity = gpEntityManager->CreateEntity();

	GenericObject<false, Value::ValueType> PrefabDoc = pPrefabDoc->GetObject();

	// Right Now I have set the mType in the JSON as Numbers but will change
	// it to string once reflection is implemented for Type similar 
	// to the way done in components
	
	// Prefab "Type"
	/* added */
	rttr::enumeration type_enum = rttr::type::get_by_name("EntityType").get_enumeration();
	pEntity->mType = type_enum.name_to_value(PrefabDoc["Type"].GetString()).convert<EntityType>();

	for (Value::MemberIterator ComponentItr = PrefabDoc.MemberBegin();
		ComponentItr != PrefabDoc.MemberEnd(); ++ComponentItr)
	{
		if (ComponentItr->value.IsObject()) {
			Component* pComp = nullptr;
			pEntity->AddComponent(Reflect(ComponentItr, pComp, pEntity));
		}
	}

	//pEntity->mType = (EntityType)PrefabDoc["Type"].GetInt();
	if (pEntity->mType == PLAYER) {
		gpSystemManager->mPlayerSystem->SetPlayerEntity(pEntity);
		gpCameraManager->SetLookAtPoint(pEntity->GetComponent<Body>());
	}
	if (pEntity->mType == FIRE_GUN) {
		gpSystemManager->mPlayerSystem->SetFireGun(pEntity);
		
	}
	if (pEntity->mType == ICE_GUN) {
		gpSystemManager->mPlayerSystem->SetIceGun(pEntity);
	}

	return pEntity;
}

// READ FROM JSON
void Reflection::Deserialize(Document* _pDoc, bool isPauseMenu)
{
	GenericObject<false, Value::ValueType> LevelDoc = _pDoc->GetObject();
	//gpSceneManager->mMenuEntity.clear();
	for (Value::MemberIterator ObjectItr = LevelDoc.MemberBegin(); ObjectItr != LevelDoc.MemberEnd(); ++ObjectItr)
	{
		GenericObject<false, Value::ValueType> ObjectDoc = ObjectItr->value.GetObject();

		if (ObjectDoc.HasMember("file")) {
			// LOAD OBJECT
			Entity* pEntity = LoadObject(ObjectDoc["file"].GetString());
			if (isPauseMenu) {
				gpSceneManager->mMenuEntity.push_back(pEntity);
			}

			// OVERRIDE COMPONENTS
			if (ObjectDoc.HasMember("Type")) {
				rttr::enumeration type_enum = rttr::type::get_by_name("EntityType").get_enumeration();
				pEntity->mType = type_enum.name_to_value(ObjectDoc["Type"].GetString()).convert<EntityType>();
			}

			EntityInsideEntity(ObjectDoc, pEntity);

			if (isPauseMenu) {
				rttr::type component_manager = rttr::type::get_by_name("ComponentManager");

				for (auto prop : component_manager.get_properties()) {
					std::string component_name = prop.get_name().to_string();
					component_name.erase(0, 1);

					for (auto component : pEntity->mComponents) {
						if (component->mComponentName == component_name) {
							component->Init();
						}
					}
				}
			}
		}
	}

	if (LevelDoc.HasMember("Audios")) {
		GenericObject<false, Value::ValueType> AudioDoc = LevelDoc["Audios"].GetObject();
		if (AudioDoc.HasMember("Load")) {
			auto loadArray = AudioDoc["Load"].GetArray();
			for (SizeType i = 0; i < loadArray.Size(); i++)
			{
				std::string audio = loadArray[i].GetString();
				gpAudioManager->Load(audio);
			}
		}

		if (AudioDoc.HasMember("LoadLoop")) {
			auto loadLoopArray = AudioDoc["LoadLoop"].GetArray();
			for (SizeType i = 0; i < loadLoopArray.Size(); i++)
			{
				std::string audio = loadLoopArray[i].GetString();
				gpAudioManager->LoadLoop(audio);
				int channelID = gpAudioManager->Play(audio, -1);
				gpAudioManager->mLoadLoopChannels.push_back(channelID);
				//gpAudioManager->Volume(vol, 1.0f);
			}
		}
	}
	
	if (!isPauseMenu)
		gpComponentManager->invoke("Init");

	if (LevelDoc.HasMember("CameraBounds")) {
		glm::vec3 Min, Max;
		GenericObject<false, Value::ValueType> CameraDoc = LevelDoc["CameraBounds"].GetObject();
		if (CameraDoc.HasMember("Min")) {
			 auto MinArray = CameraDoc["Min"].GetArray();
			 Min = glm::vec3(MinArray[0].GetFloat(), MinArray[1].GetFloat(), MinArray[2].GetFloat());
		}
		if (CameraDoc.HasMember("Max")) {
			auto MaxArray = CameraDoc["Max"].GetArray();
			Max = glm::vec3(MaxArray[0].GetFloat(), MaxArray[1].GetFloat(), MaxArray[2].GetFloat());
		}
		gpCameraManager->SetBounds(Min, Max);
	}
}

void Reflection::EntityInsideEntity(GenericObject<false, Value::ValueType> ObjectDoc, Entity* pEntity)
{
	for (Value::MemberIterator ComponentItr = ObjectDoc.MemberBegin();
		ComponentItr != ObjectDoc.MemberEnd(); ++ComponentItr)
	{
		Component* child_pComp = nullptr;
		std::string CompoString = ComponentItr->name.GetString();

		if (!ComponentItr->value.IsObject()) continue;

		//Registering Child
		if (CompoString == "Child") {
			GenericObject<false, Value::ValueType> number_of_entities = ObjectDoc["Child"].GetObject();

			for (Value::MemberIterator ChildItr = number_of_entities.MemberBegin();
				ChildItr != number_of_entities.MemberEnd(); ++ChildItr)
			{
				GenericObject<false, Value::ValueType> ChildObj = ChildItr->value.GetObject();
				Entity* pChildEntity = LoadObject(ChildObj["file"].GetString());
				pChildEntity->mParent = pEntity;
				pEntity->mChild = pChildEntity;

				EntityInsideEntity(ChildObj, pChildEntity);
			}
		}

		//Registering Door/Rope/Oscillate
		else if (CompoString == "DeleteEntity") {
			GenericObject<false, Value::ValueType> number_of_entities = ObjectDoc[CompoString.c_str()].GetObject();
			Key* kEy = pEntity->GetComponent<Key>();
			kEy->mEntity.clear();

			//Iterating Through all doors/oscillate/rope
			for (Value::MemberIterator DoorEntityItr = number_of_entities.MemberBegin();
				DoorEntityItr != number_of_entities.MemberEnd(); ++DoorEntityItr)
			{
				GenericObject<false, Value::ValueType> DoorEntityDoc = DoorEntityItr->value.GetObject();
				Entity* doorEntity = LoadObject(DoorEntityDoc["file"].GetString());
				kEy->mEntity.push_back(doorEntity);

				//Overriding through all components of Doorentity
				EntityInsideEntity(DoorEntityDoc, doorEntity);
			}
		}

		//Normal Iterations for Components
		else {
			MapListStructure* pMLS = getPoolObject(ComponentItr->name.GetString());
			if (pMLS) {
				child_pComp = pMLS->GetComponent(pEntity->id);
			}

			if (nullptr == child_pComp)
				pEntity->AddComponent(Reflect(ComponentItr, child_pComp, pEntity));
			else
				Reflect(ComponentItr, child_pComp, pEntity);
		}
	}
	if (pEntity->mType == CHECKPOINT) {
		gpSystemManager->mPlayerSystem->AddCheckpoint(pEntity);
	}
	else if (pEntity->mType == PLAYER) {
		gpSystemManager->mPlayerSystem->mCheckpoint = pEntity->GetComponent<Transform>()->mPos;
		gpSystemManager->mPlayerSystem->AddCheckpoint(pEntity);
	}
}