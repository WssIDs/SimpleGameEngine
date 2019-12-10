#pragma once
#include "..\..\Core.h"
#include <vector>
#include "boost\serialization\access.hpp"
#include <boost\serialization\shared_ptr.hpp>


#include "Graphics\2D\Canvas.h"
#include "..\..\Actors\Camera.h"
#include "Graphics\DX11\Render\PointLight.h"
#include "Graphics\DX11\Primitive\Mesh.h"
#include "..\..\Actors\Light\TestPointLight.h"
#include "..\..\Actors\Actor.h"



class Level : public WObject
{
public:
	Level() = default;

	~Level()
	{
		//for (auto scene : SceneObjects)
		//{
		//	delete scene;
		//	scene = nullptr;
		//}
	}

	std::vector<std::shared_ptr<WObject>> SceneObjects;
	std::vector<std::shared_ptr<Actor>> ActorObjects;
	std::shared_ptr<Canvas> SceneViewportUI;

	void Init(const std::string& filename);

	void Load(const std::string& filename);

	void Load()
	{
		SceneViewportUI = std::make_shared<Canvas>();

		std::shared_ptr<WObject> camera(new Camera());
		SceneObjects.push_back(camera);

		std::shared_ptr<Actor> light(new TestPointLight());
		ActorObjects.push_back(light);

		//std::shared_ptr<WObject> model(new Model(BASE_MODELS_DIR + "sponza.fbx"));
		//SceneObjects.push_back(model);
	}

	void Tick(double deltaTime) override
	{
		WObject::Tick(deltaTime);
		SceneViewportUI->Tick(deltaTime);

		for (auto& item : ActorObjects)
		{
			item->Tick(deltaTime);
		}

		for (auto& item : SceneObjects)
		{
			item->Tick(deltaTime);
		}
	}


	void Render(double deltaTime) override
	{
		WObject::Render(deltaTime);
		SceneViewportUI->Render(deltaTime);

		for (auto& item : ActorObjects)
		{
			item->Render(deltaTime);
		}

		for (auto& item : SceneObjects)
		{
			item->Render(deltaTime);
		}
	}

private :
	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& SceneObjects;
	}
};

