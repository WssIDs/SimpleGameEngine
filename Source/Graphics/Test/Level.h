#pragma once
#include "..\DX11\Graphics.h"
#include "Serialization\IWObject.h"
#include <vector>
#include "..\Engine\Core.h"
#include "..\DX11\Primitive\Mesh.h"
#include "boost\serialization\access.hpp"
#include <boost\serialization\shared_ptr.hpp>
#include "WObject.h"
#include "..\Engine\Actors\Camera.h"
#include "..\DX11\Render\PointLight.h"
#include "..\2D\Canvas.h"



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

	std::vector<std::shared_ptr<IWObject>> SceneObjects;

	std::shared_ptr<Canvas> SceneViewportUI;

	void Init(const std::string& filename);

	void Load(const std::string& filename);

	void Load()
	{
		SceneViewportUI = std::make_shared<Canvas>();

		std::shared_ptr<IWObject> camera(new Camera());
		SceneObjects.push_back(camera);

		std::shared_ptr<IWObject> light(new PointLight());
		SceneObjects.push_back(light);

		//std::shared_ptr<IWObject> model(new Model(BASE_MODELS_DIR + "sponza.fbx"));
		//SceneObjects.push_back(model);
	}

	void Tick(double deltaTime) override
	{
		WObject::Tick(deltaTime);
		SceneViewportUI->Tick(deltaTime);

		for (auto& item : SceneObjects)
		{
			item->Tick(deltaTime);
		}
	}


	void Render(double deltaTime) override
	{
		WObject::Render(deltaTime);
		SceneViewportUI->Render(deltaTime);

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

