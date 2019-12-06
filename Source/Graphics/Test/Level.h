#pragma once
#include "..\DX11\Graphics.h"
#include "Serialization\Object.h"
#include <vector>
#include "..\Engine\Core.h"
#include "..\DX11\Primitive\Mesh.h"
#include "boost\serialization\access.hpp"
#include <boost\serialization\shared_ptr.hpp>



class Level
{
public:
	Level(Graphics& gfx)
		:
		gfx(&gfx)
	{

	}

	~Level()
	{
		//for (auto scene : SceneObjects)
		//{
		//	delete scene;
		//	scene = nullptr;
		//}
	}

	std::vector<std::shared_ptr<IWObject>> SceneObjects;

	Graphics* gfx;

	void Init(const std::string& filename);

	void Load(const std::string& filename);

	void Load()
	{
		if (gfx != nullptr)
		{
			std::shared_ptr<IWObject> model(new Model(*gfx, BASE_MODELS_DIR + "sponza.fbx"));

			SceneObjects.push_back(model);
		}
	}

	void Tick(double deltaTime)
	{
		for (auto& item : SceneObjects)
		{
			item->Tick(*gfx, deltaTime);
		}
	}


	void Render(double deltaTime)
	{
		for (auto& item : SceneObjects)
		{
			item->Render(*gfx, deltaTime);
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

